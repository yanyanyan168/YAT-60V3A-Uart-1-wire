/**
  ******************************************************************************
  * @file    uart_1_wire.c
  * @brief   P30 单线 UART 通信协议实现，充电器专用版。
  *
  * 本版根据 5 轮讨论后的边界整理：
  * 1. 只实现 PDF 6.1 充电器流程：A0 -> A1 -> A4 -> A6 -> A7 -> B1 -> B3 -> B4 -> B6。
  * 2. 进入正常充电后，只循环 B1/B3/B4/B6。
  * 3. CHK 只使用普通累加和：CHK = CMD + DATA1 + ... + DATAN；不实现 ZTR PDU 专用校验。
  * 4. 默认按电池包通讯字节序处理 16bit 数据，并保留 U1W_U16_LSB_FIRST 供抓包后修正。
  * 5. 从机回复采用 FIFO 字节数连续两次不变来模拟接收空闲，然后统一取帧校验。
  * 6. 发送间隔由主充电流程控制，本模块不再自动空转 gap，也不在 10ms 任务中自动发送。
  * 7. 单帧超时或帧错误只重发当前命令，不推进 step；连续失败超过上限后只上报 retry_over，主流程处理。
  * 8. 不处理 BT+ 拔包判断；拔包回待机由主充电流程完成，需要重连时调用 uart_1_wire_reset_link()。
  ******************************************************************************
  */

#include "uart_1_wire.h"
#include "uart.h"
#include "fifo.h"
#include "gpio.h"
#include "timer.h"

/*============================== 可配置参数 ==================================*/

#ifndef U1W_RETRY_MAX
#define U1W_RETRY_MAX                       (10U)      /* 当前命令连续失败超过该次数后上报主流程 */
#endif

#ifndef U1W_BUS_LOW_10MS_TH
#define U1W_BUS_LOW_10MS_TH                 (30U / TASK_10MS)  /* 总线持续低电平 30ms 判异常 */
#endif

#ifndef U1W_PROTO_TIMEOUT_MS
#define U1W_PROTO_TIMEOUT_MS                (50U)      /* 4800bps + 10ms 空闲检测，不建议小于 50ms */
#endif

#ifndef U1W_POWER_ON_DELAY_10MS
#define U1W_POWER_ON_DELAY_10MS             (50U / TASK_10MS)   /* 插包/重连后等待从机初始化 */
#endif

#ifndef U1W_U16_LSB_FIRST
#define U1W_U16_LSB_FIRST                   (1U)       /* 1: 低字节先到；0: 高字节先到，按实测抓包确认 */
#endif

#ifndef U1W_FULL_DISPLAY_10MS
#define U1W_FULL_DISPLAY_10MS               (180000UL / TASK_10MS) /* 满电后 B6 03 SOC 周期发送窗口：3 分钟 */
#endif

#define U1W_LIMIT_VOLTAGE(v)                (((v) == 0U) ? SET_vMAX : (((v) > SET_vMAX) ? SET_vMAX : (v)))
#define U1W_LIMIT_CURRENT(i)                (((i) == 0U) ? iMAX     : (((i) > iMAX)     ? iMAX     : (i)))

/*============================== 本地变量 ====================================*/

static u8 idata s_tx_buf[4];                /* 最长发送帧：CMD + DATA1 + DATA2 + CHK */
static u8 xdata s_rx_buf[COM_FRAME_LEN];    /* 当前最长回复为 6 字节 */

static u8 idata s_wait_cmd;                 /* 当前等待的回复命令 */
static u8 idata s_wait_len;                 /* 当前等待的回复长度 */
static u8 idata s_rx_len;                   /* 本次实际收到的长度 */
static u8 idata s_bus_low_count_10ms;        /* 总线低电平持续计数 */
static u8 idata s_start_delay_10ms;          /* 上电/重连等待计数 */
static u8 idata s_last_mos_data;            /* 最近一次已发送的 MOS 控制数据 */

static bit s_wait_reply;                    /* 已发送命令，正在等回复 */
static bit s_frame_ready;                   /* FIFO 空闲后已取出完整帧 */
static bit s_charge_enable;                 /* 外部是否允许打开电池包充电 MOS */
static bit s_need_mos_update;               /* 外部状态变化后，下一次 B6 优先同步 MOS */

static u16 idata s_wait_start_ms;            /* 等待回复起点，单位 ms */
static u16 idata s_full_display_10ms;        /* 满电后显示电量倒计时，单位 10ms */

UART_1WIRE_INFO_Types xdata uart_1_wire;

/*============================== 基础工具 ====================================*/

/**
  * @brief  计算 CMD + DATA 的 8bit 累加和。
  */
static u8 u1w_sum(u8 *buf, u8 len)
{
    u8 i;
    u8 sum = 0U;

    for(i = 0U; i < len; i++)
    {
        sum = (u8)(sum + buf[i]);
    }

    return sum;
}

/**
  * @brief  判断 ms 延时是否到期，使用 time_after 处理 16bit tick 回绕。
  */
static bit u1w_ms_elapsed(u16 start_ms, u16 delay_ms)
{
    u16 now_ms;
    u16 end_ms;

    now_ms = timer_get_tick_ms();
    end_ms = (u16)(start_ms + delay_ms);

    if((now_ms == end_ms) || time_after(now_ms, end_ms))
    {
        return 1;
    }

    return 0;
}

/**
  * @brief  从总线收到的两个字节还原 u16。
  * @note   默认低字节先到；若抓包确认高字节先到，把 U1W_U16_LSB_FIRST 置 0。
  */
static u16 u1w_get_u16_bus(u8 byte0, u8 byte1)
{
#if (U1W_U16_LSB_FIRST != 0U)
    return (u16)((u16)byte0 | ((u16)byte1 << 8));
#else
    return (u16)(((u16)byte0 << 8) | (u16)byte1);
#endif
}

/**
  * @brief  协议 0.01V 转 mV，并做基本限幅。
  */
static u16 u1w_001v_to_mv(u16 raw_001v)
{
    if(raw_001v > 6500U)
    {
        raw_001v = 6500U;
    }

    return (u16)(raw_001v * 10U);
}

/**
  * @brief  单电芯 0.1A 转整包 mA。
  * @note   A6/A7 描述的是单电芯电流，整包允许电流需要乘并联数。
  */
static u16 u1w_cell_01a_to_pack_ma(u16 raw_cell_01a)
{
    u32 ma;
    u8 parallel;

    parallel = uart_1_wire.cell_parallel;
    if((parallel == 0U) || (parallel > 16U))
    {
        parallel = 1U;
    }

    ma = (u32)raw_cell_01a * 100UL * (u32)parallel;
    if(ma > 0xFFFFUL)
    {
        ma = 0xFFFFUL;
    }

    return (u16)ma;
}

/**
  * @brief  清除当前等待回复状态，不清已经解析出的电池包参数。
  */
static void u1w_clear_wait(void)
{
    s_wait_reply = 0;
    s_frame_ready = 0;
    s_wait_cmd = 0U;
    s_wait_len = 0U;
    s_rx_len = 0U;
}

/**
  * @brief  记录一次通讯失败。
  * @note   不推进 step；超过上限后只置位 retry_over，由主流程决定是否 reset_link。
  */
static void u1w_fail_once(u8 err)
{
    uart_1_wire.last_error = err;

    if(uart_1_wire.retry_count < 0xFFU)
    {
        uart_1_wire.retry_count++;
    }

    if(uart_1_wire.retry_count > U1W_RETRY_MAX)
    {
        uart_1_wire.last_error = U1W_ERR_RETRY_OVER;
        uart_1_wire.retry_over = 1U;
        uart_1_wire.link_state = U1W_LINK_OFFLINE;
        uart_1_wire.charge_status = U1W_B4_FAIL;
        u1w_clear_wait();
        com_fifo_clear();
    }
}

/*============================== 发送与接收 ==================================*/

/**
  * @brief  根据命令字返回固定回复长度。
  */
static u8 u1w_reply_len(u8 cmd)
{
    switch(cmd)
    {
    case U1W_CMD_A4:
    case U1W_CMD_A6:
    case U1W_CMD_A7:
    case U1W_CMD_B1:
        return 6U;

    case U1W_CMD_A0:
    case U1W_CMD_A1:
    case U1W_CMD_B3:
    case U1W_CMD_B4:
    case U1W_CMD_B6:
        return 4U;

    default:
        return 0U;
    }
}

/**
  * @brief  发送一条命令，并进入等待回复状态。
  * @param  cmd: 命令字。
  * @param  d1/d2: 两个数据字节。无数据命令时忽略。
  * @param  data_len: 0 或 2。当前充电器协议命令只用到这两种长度。
  */
static bit u1w_send_cmd(u8 cmd, u8 d1, u8 d2, u8 data_len)
{
    u8 tx_len;

    s_wait_len = u1w_reply_len(cmd);
    if(s_wait_len == 0U)
    {
        uart_1_wire.last_error = U1W_ERR_NOT_READY;
        return 0;
    }

    com_fifo_clear();

    s_tx_buf[0] = cmd;
    if(data_len == 0U)
    {
        s_tx_buf[1] = cmd;
        tx_len = 2U;
    }
    else
    {
        s_tx_buf[1] = d1;
        s_tx_buf[2] = d2;
        s_tx_buf[3] = u1w_sum(s_tx_buf, 3U);
        tx_len = 4U;
    }

    s_wait_cmd = cmd;
    s_rx_len = 0U;
    s_frame_ready = 0;
    s_wait_reply = 1;
    s_wait_start_ms = timer_get_tick_ms();

    if(com_uart_send_buf(s_tx_buf, tx_len) != BSP_OK)
    {
        u1w_clear_wait();
        u1w_fail_once(U1W_ERR_SEND);
        return 0;
    }

    return 1;
}

/**
  * @brief  按当前协议阶段发送一条主机命令。
  */
bit uart_1_wire_send_next(void)
{
    u8 mos_data;
    u8 soc;

    if(uart_1_wire_can_send() == 0)
    {
        uart_1_wire.last_error = U1W_ERR_NOT_READY;
        return 0;
    }

    switch(uart_1_wire.step)
    {
    default:
        uart_1_wire.step = U1W_STEP_A0;
        /* fall through */

    case U1W_STEP_A0:
        return u1w_send_cmd(U1W_CMD_A0, 0x00U, U1W_MASTER, 2U);

    case U1W_STEP_A1:
        return u1w_send_cmd(U1W_CMD_A1, 0U, 0U, 0U);

    case U1W_STEP_A4:
        return u1w_send_cmd(U1W_CMD_A4, 0U, 0U, 0U);

    case U1W_STEP_A6:
        return u1w_send_cmd(U1W_CMD_A6, 0U, 0U, 0U);

    case U1W_STEP_A7:
        return u1w_send_cmd(U1W_CMD_A7, 0U, 0U, 0U);

    case U1W_STEP_B1:
        return u1w_send_cmd(U1W_CMD_B1, 0U, 0U, 0U);

    case U1W_STEP_B3:
        return u1w_send_cmd(U1W_CMD_B3, 0U, 0U, 0U);

    case U1W_STEP_B4:
        return u1w_send_cmd(U1W_CMD_B4, 0U, 0U, 0U);

    case U1W_STEP_B6:
        /*
         * B6 发送策略：
         * 1. MOS 状态变化、异常或满电刚停充时，优先发送 B6 01，同步电池包充电 MOS 状态。
         * 2. 满电显示倒计时有效时，每次轮到 B6 都发送 B6 03 SOC，让电池包持续显示电量。
         * 3. 满电显示窗口结束后，恢复发送 B6 01 MOS，继续维持当前充电 MOS 状态。
         *
         * 注意：满电显示不是只发一次，而是在 3 分钟窗口内随 B6 轮询周期重复发送。
         */
        mos_data = 0U;
        if((s_charge_enable != 0) && (uart_1_wire_has_fault() == 0))
        {
            mos_data = U1W_MOS_CHG_ON;
        }

        if((s_need_mos_update != 0) || (mos_data != s_last_mos_data) || (s_full_display_10ms == 0U))
        {
            s_need_mos_update = 0;
            s_last_mos_data = mos_data;
            return u1w_send_cmd(U1W_CMD_B6, U1W_B6_TYPE_MOS, mos_data, 2U);
        }

        soc = uart_1_wire.soc_percent;
        if(soc > 100U)
        {
            soc = 100U;
        }
        return u1w_send_cmd(U1W_CMD_B6, U1W_B6_TYPE_SOC, soc, 2U);
    }
}

/**
  * @brief  10ms 周期内检查 RX FIFO，模拟接收空闲后取出完整帧。
  */
static void u1w_rx_poll(void)
{
    u8 i;
    u8 len;
    static u8 len_bk = 0U;

    if((s_wait_reply == 0) || (s_frame_ready != 0))
    {
        len_bk = 0U;
        return;
    }

    len = s_com_fifo.cnt;
    if(len == 0U)
    {
        len_bk = 0U;
        return;
    }

    if(len != len_bk)
    {
        len_bk = len;
        return;
    }

    if(len > COM_FRAME_LEN)
    {
        len = COM_FRAME_LEN;
    }

    s_rx_len = len;
    for(i = 0U; i < s_rx_len; i++)
    {
        s_rx_buf[i] = com_fifo_pop();
    }

    com_fifo_clear();

    len_bk = 0U;
    s_wait_reply = 0;
    s_frame_ready = 1;
}

/**
  * @brief  检查当前回复帧长度、命令回显和 CHK。
  */
static bit u1w_check_frame(void)
{
    if((s_wait_len == 0U) || (s_rx_len != s_wait_len))
    {
        return 0;
    }

    if(s_rx_buf[0] != s_wait_cmd)
    {
        return 0;
    }

    if(u1w_sum(s_rx_buf, (u8)(s_wait_len - 1U)) != s_rx_buf[s_wait_len - 1U])
    {
        return 0;
    }

    return 1;
}

/*============================== 解析与状态推进 ===============================*/

/**
  * @brief  解析有效回复帧，并推进到下一阶段。
  */
static void u1w_parse_frame(void)
{
    u8 xy;
    u32 pack_mv;

    uart_1_wire.last_error = U1W_ERR_NONE;

    switch(s_wait_cmd)
    {
    case U1W_CMD_A0:
        /* A0 ZZ XY CHK：ZZ=识别码，X=串数编码，Y=并数编码。 */
        uart_1_wire.pack_id = s_rx_buf[1];
        xy = s_rx_buf[2];
        uart_1_wire.cell_series = (u8)(((xy >> 4) & 0x0FU) + 5U);
        uart_1_wire.cell_parallel = (u8)((xy & 0x0FU) + 1U);

        if((uart_1_wire.cell_series < 5U) || (uart_1_wire.cell_series > 20U))
        {
            uart_1_wire.cell_series = BAT_SERIES;
        }
        if((uart_1_wire.cell_parallel == 0U) || (uart_1_wire.cell_parallel > 16U))
        {
            uart_1_wire.cell_parallel = 1U;
        }

        uart_1_wire.link_state = U1W_LINK_HANDSHAKE;
        uart_1_wire.step = U1W_STEP_A1;
        break;

    case U1W_CMD_A1:
        /* A1 XX YY CHK：YY=容量 0.1Ah。XX 电芯类型当前充电控制不用，不保存。 */
        uart_1_wire.cell_cap_01ah = s_rx_buf[2];
        uart_1_wire.step = U1W_STEP_A4;
        break;

    case U1W_CMD_A4:
        /* A4 xxxx yyyy CHK：xxxx=单节预充截止，yyyy=单节满充，单位 0.01V。 */
        uart_1_wire.cell_pre_mv = u1w_001v_to_mv(u1w_get_u16_bus(s_rx_buf[1], s_rx_buf[2]));

        pack_mv = (u32)u1w_001v_to_mv(u1w_get_u16_bus(s_rx_buf[3], s_rx_buf[4])) *
                  (u32)uart_1_wire.cell_series;
        if(pack_mv > 0xFFFFUL)
        {
            pack_mv = 0xFFFFUL;
        }
        uart_1_wire.target_voltage_mv = U1W_LIMIT_VOLTAGE((u16)pack_mv);
        uart_1_wire.step = U1W_STEP_A6;
        break;

    case U1W_CMD_A6:
        /* A6 xxxx yyyy CHK：xxxx=单电芯最大充电电流 0.1A，整包需乘并联数。 */
        uart_1_wire.max_charge_current_ma =
            U1W_LIMIT_CURRENT(u1w_cell_01a_to_pack_ma(u1w_get_u16_bus(s_rx_buf[1], s_rx_buf[2])));
        uart_1_wire.step = U1W_STEP_A7;
        break;

    case U1W_CMD_A7:
        /* A7 XX YY 00 ZZ CHK：XX/YY=二级温区，ZZ=单电芯二级电流 0.1A。 */
        uart_1_wire.derate_low_degc = (s8)s_rx_buf[1];
        uart_1_wire.derate_high_degc = (s8)s_rx_buf[2];
        uart_1_wire.derate_current_ma = U1W_LIMIT_CURRENT(u1w_cell_01a_to_pack_ma(s_rx_buf[4]));
        uart_1_wire.link_state = U1W_LINK_ONLINE;
        uart_1_wire.step = U1W_STEP_B1;
        break;

    case U1W_CMD_B1:
        /* B1 xxxx yyyy CHK：xxxx=最低单节，yyyy=最高单节，单位 0.01V。 */
        uart_1_wire.cell_min_mv = u1w_001v_to_mv(u1w_get_u16_bus(s_rx_buf[1], s_rx_buf[2]));
        uart_1_wire.cell_max_mv = u1w_001v_to_mv(u1w_get_u16_bus(s_rx_buf[3], s_rx_buf[4]));
        uart_1_wire.step = U1W_STEP_B3;
        break;

    case U1W_CMD_B3:
        /* B3 XX YY CHK：XX=电池温度，YY=充电 MOS 温度；0xAA 表示无 MOS 温度。 */
        uart_1_wire.batt_temp_degc = (s8)s_rx_buf[1];
        uart_1_wire.mos_temp_degc = (s8)s_rx_buf[2];
        uart_1_wire.step = U1W_STEP_B4;
        break;

    case U1W_CMD_B4:
        /* B4 XX YY CHK：XX=SOC，YY=充电状态位。 */
        uart_1_wire.soc_percent = s_rx_buf[1];
        if(uart_1_wire.soc_percent > 100U)
        {
            uart_1_wire.soc_percent = 100U;
        }
        uart_1_wire.charge_status = s_rx_buf[2];
        uart_1_wire.step = U1W_STEP_B6;
        break;

    case U1W_CMD_B6:
        /* B6 YY XX CHK：从机回显控制类型和数据，当前只需要确认收到即可。 */
        uart_1_wire.step = U1W_STEP_B1;
        break;

    default:
        uart_1_wire.step = U1W_STEP_A0;
        break;
    }

    uart_1_wire.retry_count = 0U;
    uart_1_wire.retry_over = 0U;
    uart_1_wire.offline_count_10ms = 0U;
}

/**
  * @brief  检查总线是否持续低电平超过 30ms。
  * @note   只上报错误并停止当前等待，不自动重连。
  */
static bit u1w_bus_low_check_10ms(void)
{
    if(com_uart_get_dir() != UART_1WIRE_DIR_RX)
    {
        s_bus_low_count_10ms = 0U;
        return 0;
    }

    if(COM_PIN != 0)
    {
        s_bus_low_count_10ms = 0U;
        return 0;
    }

    if(s_bus_low_count_10ms < 0xFFU)
    {
        s_bus_low_count_10ms++;
    }

    if(s_bus_low_count_10ms >= U1W_BUS_LOW_10MS_TH)
    {
        uart_1_wire.last_error = U1W_ERR_BUS_LOW;
        uart_1_wire.retry_over = 1U;
        uart_1_wire.link_state = U1W_LINK_OFFLINE;
        uart_1_wire.charge_status = U1W_B4_FAIL;
        u1w_clear_wait();
        com_fifo_clear();
        return 1;
    }

    return 0;
}

/**
  * @brief  2s 通讯离线计数。
  * @note   只上报离线，不自动回 A0；主流程决定是否 reset_link。
  */
static void u1w_offline_tick_10ms(void)
{
    if(uart_1_wire.offline_count_10ms < (COM_TIMEOUT_MS / TASK_10MS))
    {
        uart_1_wire.offline_count_10ms++;
        return;
    }

    uart_1_wire.last_error = U1W_ERR_COMM_LOST;
    uart_1_wire.retry_over = 1U;
    uart_1_wire.link_state = U1W_LINK_OFFLINE;
    uart_1_wire.charge_status = U1W_B4_FAIL;
    u1w_clear_wait();
}

/*============================== 对外接口 ====================================*/

/**
  * @brief  初始化协议层。
  */
void uart_1_wire_init(void)
{
    memclr(&uart_1_wire, sizeof(uart_1_wire));
    memclr(s_rx_buf, sizeof(s_rx_buf));
    memclr(s_tx_buf, sizeof(s_tx_buf));

    uart_1_wire.link_state = U1W_LINK_OFFLINE;
    uart_1_wire.step = U1W_STEP_A0;
    uart_1_wire.last_error = U1W_ERR_NONE;
    uart_1_wire.retry_count = 0U;
    uart_1_wire.retry_over = 0U;
    uart_1_wire.cell_series = BAT_SERIES;
    uart_1_wire.cell_parallel = 1U;
    uart_1_wire.cell_pre_mv = CELL_PRE_MV;
    uart_1_wire.charge_status = U1W_B4_FAIL;
    uart_1_wire.target_voltage_mv = SET_vMAX;
    uart_1_wire.max_charge_current_ma = iMAX;
    uart_1_wire.derate_current_ma = iMAX;
    uart_1_wire.derate_low_degc = 10;
    uart_1_wire.derate_high_degc = 40;

    s_bus_low_count_10ms = 0U;
    s_start_delay_10ms = U1W_POWER_ON_DELAY_10MS;
    s_full_display_10ms = 0U;
    s_last_mos_data = 0xFFU;

    s_wait_reply = 0;
    s_frame_ready = 0;
    s_charge_enable = 1;
    s_need_mos_update = 1;

    u1w_clear_wait();
    com_fifo_clear();
    com_uart_set_rx_mode();
}

/**
  * @brief  P30 单线通信 10ms 轮询任务。
  * @note   本函数只负责接收、超时、错误计时；不自动发送下一条命令。
  */
void uart_1_wire_poll_10ms(void)
{
    if(s_full_display_10ms != 0U)
    {
        s_full_display_10ms--;
    }

    if(s_start_delay_10ms != 0U)
    {
        s_start_delay_10ms--;
    }

    u1w_rx_poll();

    if(u1w_bus_low_check_10ms() != 0)
    {
        u1w_offline_tick_10ms();
        return;
    }

    if(s_frame_ready != 0)
    {
        if(u1w_check_frame() != 0)
        {
            u1w_parse_frame();
        }
        else
        {
            u1w_fail_once(U1W_ERR_FRAME);
        }

        s_frame_ready = 0;
        s_rx_len = 0U;
        u1w_offline_tick_10ms();
        return;
    }

    if(s_wait_reply != 0)
    {
        if(u1w_ms_elapsed(s_wait_start_ms, U1W_PROTO_TIMEOUT_MS) != 0)
        {
            u1w_clear_wait();
            com_fifo_clear();
            u1w_fail_once(U1W_ERR_TIMEOUT);
        }

        u1w_offline_tick_10ms();
        return;
    }

    u1w_offline_tick_10ms();
}

/**
  * @brief  兼容旧接口。
  * @note   当前版本发送间隔由主流程控制，因此本函数只等同于 uart_1_wire_poll_10ms()。
  */
void uart_1_wire_task_10ms(void)
{
    uart_1_wire_poll_10ms();
}

/**
  * @brief  查询当前是否允许主流程发送下一条协议命令。
  */
bit uart_1_wire_can_send(void)
{
    if(s_wait_reply != 0)
    {
        return 0;
    }

    if(s_frame_ready != 0)
    {
        return 0;
    }

    if(s_start_delay_10ms != 0U)
    {
        return 0;
    }

    if(uart_1_wire.retry_over != 0U)
    {
        return 0;
    }

    return 1;
}

/**
  * @brief  查询协议层是否正忙。
  */
bit uart_1_wire_is_busy(void)
{
    if((s_wait_reply != 0) || (s_frame_ready != 0))
    {
        return 1;
    }

    return 0;
}

/**
  * @brief  查询是否已经连续失败超过上限，需要主流程处理。
  */
bit uart_1_wire_is_retry_over(void)
{
    if(uart_1_wire.retry_over != 0U)
    {
        return 1;
    }

    return 0;
}

/**
  * @brief  获取最后一次协议错误码。
  */
u8 uart_1_wire_get_last_error(void)
{
    return uart_1_wire.last_error;
}

/**
  * @brief  清除普通错误码。
  * @note   不清 retry_over；retry_over 必须由主流程调用 reset_link 后清除。
  */
void uart_1_wire_clear_error(void)
{
    if(uart_1_wire.retry_over == 0U)
    {
        uart_1_wire.last_error = U1W_ERR_NONE;
    }
}

/**
  * @brief  查询单线通信是否在线。
  */
bit uart_1_wire_is_online(void)
{
    if(uart_1_wire.link_state == U1W_LINK_ONLINE)
    {
        return 1;
    }

    return 0;
}

/**
  * @brief  查询 B4 是否存在停充类异常。
  */
bit uart_1_wire_has_fault(void)
{
    if((uart_1_wire.charge_status & U1W_B4_FAULT_MASK) != 0U)
    {
        return 1;
    }

    return 0;
}

/**
  * @brief  查询电池包是否允许进入正常充电。
  */
bit uart_1_wire_is_ready(void)
{
    if(uart_1_wire_is_online() == 0)
    {
        return 0;
    }

    if(uart_1_wire_has_fault() != 0)
    {
        return 0;
    }

    if(s_charge_enable == 0)
    {
        return 0;
    }

    return 1;
}

/**
  * @brief  获取协议层目标充电电压，单位 mV。
  */
u16 uart_1_wire_get_target_voltage_mv(void)
{
    return U1W_LIMIT_VOLTAGE(uart_1_wire.target_voltage_mv);
}

/**
  * @brief  获取协议层目标充电电流，单位 mA。
  */
u16 uart_1_wire_get_target_current_ma(void)
{
    u16 target_ma;

    if((uart_1_wire_is_online() == 0) ||
       (uart_1_wire_has_fault() != 0) ||
       (s_charge_enable == 0))
    {
        return 0U;
    }

    target_ma = U1W_LIMIT_CURRENT(uart_1_wire.max_charge_current_ma);

    if((uart_1_wire.derate_current_ma != 0U) &&
       (uart_1_wire.derate_current_ma < target_ma) &&
       ((uart_1_wire.batt_temp_degc < uart_1_wire.derate_low_degc) ||
        (uart_1_wire.batt_temp_degc > uart_1_wire.derate_high_degc)))
    {
        target_ma = uart_1_wire.derate_current_ma;
    }

    if((target_ma != 0U) && (target_ma < 100U))
    {
        target_ma = 100U;
    }

    return target_ma;
}

/**
  * @brief  设置是否允许 B6 打开电池包充电 MOS。
  * @note   充电主流程进入异常、满电、待机时传 0；允许充电时传 1。
  */
void uart_1_wire_set_charge_enable(bit enable)
{
    if(s_charge_enable != enable)
    {
        s_charge_enable = enable;
        s_need_mos_update = 1;
    }
}

/**
  * @brief  启动满电后 3 分钟电量显示窗口。
  * @note   本函数只打开显示窗口；真正的 B6 03 SOC 会在后续 B6 轮询中周期性重复发送。
  *         是否停充、是否回待机由充电主流程决定。
  */
void uart_1_wire_start_full_display(void)
{
    s_full_display_10ms = (u16)U1W_FULL_DISPLAY_10MS;
}

/**
  * @brief  停止满电电量显示。
  */
void uart_1_wire_stop_full_display(void)
{
    s_full_display_10ms = 0U;
}

/**
  * @brief  外部请求重新握手。
  * @note   本函数不做 BT+ 拔包判断，只提供给主流程在需要时调用。
  */
void uart_1_wire_reset_link(void)
{
    u1w_clear_wait();
    com_fifo_clear();
    com_uart_set_rx_mode();

    uart_1_wire.link_state = U1W_LINK_OFFLINE;
    uart_1_wire.step = U1W_STEP_A0;
    uart_1_wire.last_error = U1W_ERR_NONE;
    uart_1_wire.retry_count = 0U;
    uart_1_wire.retry_over = 0U;
    uart_1_wire.charge_status = U1W_B4_FAIL;
    uart_1_wire.offline_count_10ms = 0U;

    s_bus_low_count_10ms = 0U;
    s_start_delay_10ms = U1W_POWER_ON_DELAY_10MS;
    s_last_mos_data = 0xFFU;
    s_need_mos_update = 1;
}
