/**
  ******************************************************************************
  * @file    uart_1_wire.c
  * @brief   P30 单线 UART 通信协议实现，异步轮询版。
  *
  * 主流程：
  * 1. 接收：FIFO 字节进入缓存后，从头 for 扫描已知 CMD。
  *    CMD 已知、长度够、CHK 正确，才认为是合法帧并更新数据。
  *    校验失败不清空整包，而是从下一字节继续找帧头。
  *
  * 2. 发送：不再等待某一帧回复。
  *    ch.c 设置 STOP / HANDSHAKE / CHARGE 阶段后，本文件每 100ms 发一帧。
  *    HANDSHAKE 阶段轮询 A0/A1/A4/A6/A7/B1/B3/B4。
  *    CHARGE 阶段轮询 B1/B3/B4/B6，B6 固定要求打开充电 MOS。
  *
  * 3. 超时：
  *    2 秒没有任意合法帧，通信超时。
  *    3 秒关键帧没有刷新，也判通信超时。
  ******************************************************************************
  */

#include "uart_1_wire.h"
#include "uart.h"
#include "fifo.h"
#include "gpio.h"
#include "timer.h"
#include "uart_1_wire_debug.h"

#ifndef U1W_TX_PERIOD_10MS
#define U1W_TX_PERIOD_10MS                 (10U)      /* 100ms 发一帧 */
#endif

#ifndef U1W_ANY_RX_TIMEOUT_10MS
#define U1W_ANY_RX_TIMEOUT_10MS            (200U)     /* 2秒没有任意合法帧 */
#endif

#ifndef U1W_KEY_RX_TIMEOUT_10MS
#define U1W_KEY_RX_TIMEOUT_10MS            (300U)     /* 3秒关键帧没有刷新 */
#endif

#ifndef U1W_FULL_DISPLAY_10MS
#define U1W_FULL_DISPLAY_10MS              (18000U)   /* 满电显示3分钟 */
#endif

#define U1W_LIMIT_VOLTAGE(v)               (((v) == 0U) ? SET_vMAX : (((v) > SET_vMAX) ? SET_vMAX : (v)))
#define U1W_LIMIT_CURRENT(i)               (((i) == 0U) ? iMAX     : (((i) > iMAX)     ? iMAX     : (i)))

typedef enum
{
    U1W_KEY_A0 = 0U,
    U1W_KEY_A1,
    U1W_KEY_A4,
    U1W_KEY_A6,
    U1W_KEY_A7,
    U1W_KEY_B1,
    U1W_KEY_B3,
    U1W_KEY_B4,
    U1W_KEY_MAX
} U1W_KEY_Types;

#define U1W_HANDSHAKE_MASK                 (0xFFU)

typedef struct
{
    u8  stage;                              /* UART_1WIRE_STAGE_Types */
    u8  tx_index;                           /* 当前发送表下标 */
    u8  tx_tick_10ms;                       /* 100ms 发送节拍 */
    u8  rx_len;                             /* 接收缓存有效长度 */
    u8  rx_fifo_last_cnt;                   /* 上一次看到的 FIFO 数据量，用于模拟接收空闲 */
    u16 any_rx_age_10ms;                    /* 任意合法帧多久没收到 */
    u16 full_display_10ms;                  /* 满电显示持续时间 */
    u16 key_age_10ms[U1W_KEY_MAX];          /* 关键帧多久没收到 */
} U1W_CTRL_Types;

static U1W_CTRL_Types idata s_u1w;
static u8 xdata s_rx_buf[COM_FRAME_LEN];
static u8 idata s_tx_buf[4];

static u8 code s_handshake_cmd[] =
{
    U1W_CMD_A0,
    U1W_CMD_A1,
    U1W_CMD_A4,
    U1W_CMD_A6,
    U1W_CMD_A7,
    U1W_CMD_B1,
    U1W_CMD_B3,
    U1W_CMD_B4,
};

static u8 code s_charge_cmd[] =
{
    U1W_CMD_B1,
    U1W_CMD_B3,
    U1W_CMD_B4,
    U1W_CMD_B6,
};

static u8 code s_temp_wait_cmd[] =
{
    U1W_CMD_B3,
    U1W_CMD_B4,
};

UART_1WIRE_INFO_Types xdata uart_1_wire;
UART_1WIRE_CHARGE_INFO_Types idata u1w_info;

static void u1w_release_com(void)
{
    com_uart_set_rx_mode();
}

static void u1w_pull_com_low(void)
{
    /*
     * 主机主动结束通信：
     * 1. 关闭 UART1 输入/输出复用；
     * 2. P30 改为普通推挽输出；
     * 3. 输出低电平，保持到拔电池或重新握手。
     *
     * 说明：协议中的“COM低电平超过30ms”是给BMS判断主机断开的，
     * 主机侧不再把这个低电平当作异常检测。
     */
    __DisableIRQ(UART1_IRQn);
    UART1_CON0 = 0U;
    FIN_S8 = 0U;
    FOUT_S30 = 0U;
    COM_UART_OUTPUT();
    COM_PIN = 0U;
    __EnableIRQ(UART1_IRQn);
}

/*============================== 小工具函数 ==================================*/

static u8 u1w_sum(u8 *buf, u8 len)
{
    u8 i;
    u8 sum;

    sum = 0U;
    for(i = 0U; i < len; i++)
    {
        sum = (u8)(sum + buf[i]);
    }

    return sum;
}

static u16 u1w_get_u16_bus(u8 byte0, u8 byte1)
{
    return (u16)((u16)byte0 | ((u16)byte1 << 8));
}

static u16 u1w_001v_to_mv(u16 raw_001v)
{
    if(raw_001v > 6500U)
    {
        raw_001v = 6500U;
    }

    return (u16)(raw_001v * 10U);
}

static u16 u1w_cell_01a_to_pack_ma(u16 raw_cell_01a)
{
    u8 parallel;
    u32 ma;

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

static void u1w_refresh_target_current(void)
{
    u16 target_ma;

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

    uart_1_wire.target_current_ma = target_ma;
}

/*============================== 接收：扫描合法帧 ==============================*/

static void u1w_parse_frame(u8 *frame)
{
    u8 cmd;
    u8 xy;
    u8 key;
    u32 pack_mv;

    cmd = frame[0];
    uart_1_wire.last_error = U1W_ERR_NONE;
    uart_1_wire.offline_count_10ms = 0U;
    s_u1w.any_rx_age_10ms = 0U;

    key = U1W_KEY_MAX;

    switch(cmd)
    {
    case U1W_CMD_A0:
        key = U1W_KEY_A0;
        /* A0 ZZ XY CHK：ZZ=识别码，X=串数编码，Y=并数编码。 */
        uart_1_wire.pack_id = frame[1];
        xy = frame[2];
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
        break;

    case U1W_CMD_A1:
        key = U1W_KEY_A1;
        /* A1 XX YY CHK：XX=电芯规格，YY=容量 0.1Ah。 */
        uart_1_wire.cell_type = frame[1];
        uart_1_wire.cell_cap_01ah = frame[2];
        break;

    case U1W_CMD_A4:
        key = U1W_KEY_A4;
        /* A4 xxxx yyyy CHK：xxxx=预充截止，yyyy=满充，单位0.01V。 */
        uart_1_wire.cell_pre_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[1], frame[2]));
        pack_mv = (u32)u1w_001v_to_mv(u1w_get_u16_bus(frame[3], frame[4])) *
                  (u32)uart_1_wire.cell_series;
        if(pack_mv > 0xFFFFUL)
        {
            pack_mv = 0xFFFFUL;
        }
        uart_1_wire.target_voltage_mv = U1W_LIMIT_VOLTAGE((u16)pack_mv);
        break;

    case U1W_CMD_A6:
        key = U1W_KEY_A6;
        /* A6 xxxx yyyy CHK：xxxx=单电芯最大充电电流0.1A。 */
        uart_1_wire.max_charge_current_ma =
            U1W_LIMIT_CURRENT(u1w_cell_01a_to_pack_ma(u1w_get_u16_bus(frame[1], frame[2])));
        u1w_refresh_target_current();
        break;

    case U1W_CMD_A7:
        key = U1W_KEY_A7;
        /* A7 XX YY 00 ZZ CHK：XX/YY=二级温区，ZZ=单电芯二级电流0.1A。 */
        uart_1_wire.derate_low_degc = (s8)frame[1];
        uart_1_wire.derate_high_degc = (s8)frame[2];
        uart_1_wire.derate_current_ma = U1W_LIMIT_CURRENT(u1w_cell_01a_to_pack_ma(frame[4]));
        u1w_refresh_target_current();
        break;

    case U1W_CMD_B1:
        key = U1W_KEY_B1;
        /* B1 xxxx yyyy CHK：xxxx=最低单节，yyyy=最高单节，单位0.01V。 */
        uart_1_wire.cell_min_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[1], frame[2]));
        uart_1_wire.cell_max_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[3], frame[4]));
        break;

    case U1W_CMD_B3:
        key = U1W_KEY_B3;
        /* B3 XX YY CHK：XX=电池温度，YY=充电 MOS 温度。 */
        uart_1_wire.batt_temp_degc = (s8)frame[1];
        uart_1_wire.mos_temp_degc = (s8)frame[2];
        u1w_refresh_target_current();
        break;

    case U1W_CMD_B4:
        key = U1W_KEY_B4;
        /* B4 XX YY CHK：XX=SOC，YY=状态位。状态位只记录，不直接控制B6。 */
        uart_1_wire.soc_percent = frame[1];
        if(uart_1_wire.soc_percent > 100U)
        {
            uart_1_wire.soc_percent = 100U;
        }
        uart_1_wire.charge_status = frame[2];
        break;

    case U1W_CMD_B6:
        /* B6 回显，收到合法帧即可。 */
        break;

    default:
        break;
    }

    if(key < U1W_KEY_MAX)
    {
        s_u1w.key_age_10ms[key] = 0U;
        if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
        {
            uart_1_wire.handshake_mask |= (u8)(1U << key);
        }
    }

    if((uart_1_wire.handshake_mask & U1W_HANDSHAKE_MASK) == U1W_HANDSHAKE_MASK)
    {
        uart_1_wire.link_state = U1W_LINK_ONLINE;
    }
    else if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
    {
        uart_1_wire.link_state = U1W_LINK_HANDSHAKE;
    }

}

/**
  * @brief  从接收缓存头部删除指定字节数。
  * @note   用于丢弃已解析的完整帧，或跳过帧前面的无效数据。
  */
static void u1w_remove_rx_bytes(u8 count)
{
    u8 i;

    /* 删除数量超过当前缓存长度，直接清空缓存。 */
    if(count >= s_u1w.rx_len)
    {
        s_u1w.rx_len = 0U;
        return;
    }

    /* 将剩余数据前移，保留后面还没处理的数据。 */
    for(i = 0U; i < (u8)(s_u1w.rx_len - count); i++)
    {
        s_rx_buf[i] = s_rx_buf[(u8)(i + count)];
    }

    /* 更新缓存有效长度。 */
    s_u1w.rx_len = (u8)(s_u1w.rx_len - count);
}

/**
  * @brief  一线通信接收任务。
  * @note   先用 FIFO 长度不再变化来模拟接收空闲，再从缓存中扫描合法帧。
  */
static void u1w_rx_task(void)
{
    u8 i;
    u8 len;
    u8 cmd;
    u8 chk;
    u8 fifo_cnt;

    /*
     * 模拟接收空闲判断：
     * FIFO 有数据，但本次长度和上次不同，说明可能还在继续接收，先不解析。
     * 连续两次 FIFO 长度相同，才认为这一批数据已收完。
     */
    fifo_cnt = s_com_fifo.cnt;
    if(fifo_cnt != 0U)
    {
        if(fifo_cnt != s_u1w.rx_fifo_last_cnt)
        {
            s_u1w.rx_fifo_last_cnt = fifo_cnt;
            return;
        }
    }
    s_u1w.rx_fifo_last_cnt = fifo_cnt;

    /* FIFO 长度稳定后，再把数据搬到协议接收缓存。 */
    while(s_com_fifo.cnt != 0U)
    {
        if(s_u1w.rx_len >= COM_FRAME_LEN)
        {
            u1w_remove_rx_bytes(1U);
        }

        s_rx_buf[s_u1w.rx_len] = com_fifo_pop();
        s_u1w.rx_len++;
    }

    /* FIFO 已搬空，清上次 FIFO 长度记录。 */
    s_u1w.rx_fifo_last_cnt = 0U;

    /*
     * 从接收缓存中扫描帧头 CMD：
     * 校验成功就解析；
     * 校验失败就从下一字节继续找。
     */
    i = 0U;
    while(i < s_u1w.rx_len)
    {
        cmd = s_rx_buf[i];
        len = u1w_reply_len(cmd);

        /* 不是已知 CMD，跳过当前字节。 */
        if(len == 0U)
        {
            i++;
            continue;
        }

        /*
         * 找到可能帧头，但剩余长度不够。
         * 保留这个帧头及后续数据，等待下次接收。
         */
        if((u8)(s_u1w.rx_len - i) < len)
        {
            if(i != 0U)
            {
                u1w_remove_rx_bytes(i);
            }
            return;
        }

        /* 长度足够，计算校验。 */
        chk = u1w_sum(&s_rx_buf[i], (u8)(len - 1U));
        if(chk == s_rx_buf[(u8)(i + len - 1U)])
        {
            /* 合法帧：解析后删除本帧及前面的无效数据。 */
            u1w_dbg_rx_ok(&s_rx_buf[i], len);
            u1w_parse_frame(&s_rx_buf[i]);
            u1w_remove_rx_bytes((u8)(i + len));
            i = 0U;
            continue;
        }

        /* 当前 CMD 像帧头但校验不对，从下一字节继续找。 */
        uart_1_wire.last_error = U1W_ERR_FRAME;
        u1w_dbg_rx_bad(U1W_DBG_BAD_CHK, cmd, len, len, &s_rx_buf[i], chk);
        i++;
    }

    /* 扫描结束后，丢弃已经确认无用的数据。 */
    if(i != 0U)
    {
        u1w_remove_rx_bytes(i);
    }
}

/*============================== 发送：按阶段轮询 ==============================*/

/**
  * @brief  组装并发送一帧一线通信命令。
  * @param  cmd 要发送的命令码。
  * @retval 1：发送成功；0：发送失败。
  */
static bit u1w_send_frame(u8 cmd)
{
    u8 tx_len;

    /* 所有帧第 1 字节都是命令码。 */
    s_tx_buf[0] = cmd;

    if(cmd == U1W_CMD_A0)
    {
        /* A0 帧格式特殊：A0 00 主机地址 CHK。 */
        s_tx_buf[1] = 0x00U;
        s_tx_buf[2] = U1W_MASTER;
        s_tx_buf[3] = u1w_sum(s_tx_buf, 3U);
        tx_len = 4U;
    }
    else if(cmd == U1W_CMD_B6)
    {
        /*
         * B6 是主机控制/显示命令：
         * 满电显示阶段发送 SOC；
         * 其他充电阶段固定要求打开充电 MOS。
         */
        if(s_u1w.stage == U1W_STAGE_FULL_DISPLAY)
        {
            s_tx_buf[1] = U1W_B6_TYPE_SOC;
            s_tx_buf[2] = uart_1_wire.soc_percent;
        }
        else
        {
            s_tx_buf[1] = U1W_B6_TYPE_MOS;
            s_tx_buf[2] = U1W_MOS_CHG_ON;  
        }

        s_tx_buf[3] = u1w_sum(s_tx_buf, 3U);
        tx_len = 4U;
    }
    else
    {
        /*
         * 其他查询命令为 2 字节：
         * 第 1 字节：命令码；
         * 第 2 字节：校验和。这里 cmd + cmd = 0，所以校验字节等于 cmd。
         */
        s_tx_buf[1] = cmd;
        s_tx_buf[2] = 0U;
        s_tx_buf[3] = 0U;
        tx_len = 2U;
    }

    /* 发送失败时记录错误，交给主流程后续处理。 */
    if(com_uart_send_buf(s_tx_buf, tx_len) != BSP_OK)
    {
        uart_1_wire.last_error = U1W_ERR_SEND;
        return 0;
    }

    /* 调试打印：发送内容和期望回复长度。 */
    u1w_dbg_tx(s_tx_buf, tx_len, u1w_reply_len(cmd));
    return 1;
}

/**
  * @brief  一线通信发送任务。
  * @note   每 100ms 按当前通信阶段发送一帧，不等待回复。
  */
static void u1w_tx_task(void)
{
    u8 cmd;
    u8 list_len;

    /*
     * 停止阶段、主动拉低 COM 阶段、通信已超时：
     * 不再发送协议帧，并把发送节拍保持为“已到时间”。
     */
    if((s_u1w.stage == U1W_STAGE_STOP) ||
       (s_u1w.stage == U1W_STAGE_PULL_LOW) ||
       (uart_1_wire.comm_timeout != 0U))
    {
        s_u1w.tx_tick_10ms = U1W_TX_PERIOD_10MS;
        return;
    }

    /* 100ms 发送节拍未到，继续计时。 */
    if(s_u1w.tx_tick_10ms < U1W_TX_PERIOD_10MS)
    {
        s_u1w.tx_tick_10ms++;
        return;
    }

    /* 节拍到，准备发送本阶段下一帧。 */
    s_u1w.tx_tick_10ms = 0U;

    if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
    {
        /* 握手阶段：循环发送 A0/A1/A4/A6/A7/B1/B3/B4。 */
        list_len = (u8)ARRAY_SIZE(s_handshake_cmd);
        if(s_u1w.tx_index >= list_len)
        {
            s_u1w.tx_index = 0U;
        }

        cmd = s_handshake_cmd[s_u1w.tx_index];
        s_u1w.tx_index++;
    }
    else if(s_u1w.stage == U1W_STAGE_TEMP_WAIT)
    {
        /* 温度异常等待阶段：只轮询温度/状态相关帧。 */
        list_len = (u8)ARRAY_SIZE(s_temp_wait_cmd);
        if(s_u1w.tx_index >= list_len)
        {
            s_u1w.tx_index = 0U;
        }

        cmd = s_temp_wait_cmd[s_u1w.tx_index];
        s_u1w.tx_index++;
    }
    else if(s_u1w.stage == U1W_STAGE_FULL_DISPLAY)
    {
        /*
         * 满电显示阶段：周期发送 B6 显示 SOC。
         * 到达 3 分钟后，主动拉低 COM，通知 BMS 主机结束通信。
         */
        if(s_u1w.full_display_10ms >= U1W_FULL_DISPLAY_10MS)
        {
            u1w_pull_com_low();
            return;
        }

        cmd = U1W_CMD_B6;
    }
    else
    {
        /* 正常充电阶段：循环发送 B1/B3/B4/B6。 */
        list_len = (u8)ARRAY_SIZE(s_charge_cmd);
        if(s_u1w.tx_index >= list_len)
        {
            s_u1w.tx_index = 0U;
        }

        cmd = s_charge_cmd[s_u1w.tx_index];
        s_u1w.tx_index++;
    }

    /* 发送选中的命令帧，发送失败由 u1w_send_frame() 记录错误。 */
    (void)u1w_send_frame(cmd);
}

/*============================== 超时计数 =====================================*/

/**
  * @brief  一线通信计时任务，10ms 调用一次。
  * @note   负责统计总通信超时、关键帧超时，以及满电显示 3 分钟计时。
  */
static void u1w_age_task_10ms(void)
{
    u8 i;
    u8 start_key;
    u8 end_key;

    /* 停止通信或主动拉低 COM 时，不再统计通信超时。 */
    if((s_u1w.stage == U1W_STAGE_STOP) || (s_u1w.stage == U1W_STAGE_PULL_LOW))
    {
        return;
    }

    /*
     * 满电显示阶段只统计显示时间。
     * 到 3 分钟后的拉低 COM 动作，由发送任务处理。
     */
    if(s_u1w.stage == U1W_STAGE_FULL_DISPLAY)
    {
        if(s_u1w.full_display_10ms < U1W_FULL_DISPLAY_10MS)
        {
            s_u1w.full_display_10ms++;
        }
        return;
    }

    /* 任意合法帧超时计数：收到任意合法帧会在接收解析处清零。 */
    if(s_u1w.any_rx_age_10ms < U1W_ANY_RX_TIMEOUT_10MS)
    {
        s_u1w.any_rx_age_10ms++;
    }
    uart_1_wire.offline_count_10ms = s_u1w.any_rx_age_10ms;

    /*
     * 根据当前阶段，选择需要监控的关键帧范围：
     * 握手阶段：A0/A1/A4/A6/A7/B1/B3/B4 都要刷新；
     * 温度等待：只看 B3/B4；
     * 充电阶段：只看 B1/B3/B4。
     */
    if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
    {
        start_key = U1W_KEY_A0;
        end_key = U1W_KEY_MAX;
    }
    else if(s_u1w.stage == U1W_STAGE_TEMP_WAIT)
    {
        start_key = U1W_KEY_B3;
        end_key = U1W_KEY_B4 + 1U;
    }
    else
    {
        start_key = U1W_KEY_B1;
        end_key = U1W_KEY_B4 + 1U;
    }

    /* 当前阶段需要的关键帧计时。 */
    for(i = start_key; i < end_key; i++)
    {
        if(s_u1w.key_age_10ms[i] < U1W_KEY_RX_TIMEOUT_10MS)
        {
            s_u1w.key_age_10ms[i]++;
        }
    }

    /* 2 秒内没有收到任何合法帧，判定通信丢失。 */
    if(s_u1w.any_rx_age_10ms >= U1W_ANY_RX_TIMEOUT_10MS)
    {
        uart_1_wire.comm_timeout = 1U;
        uart_1_wire.retry_over = 1U;
        uart_1_wire.last_error = U1W_ERR_COMM_LOST;
        uart_1_wire.key_timeout_cmd = 0U;
        uart_1_wire.link_state = U1W_LINK_OFFLINE;
        return;
    }

    /* 关键帧超过 3 秒未刷新，也判定通信异常，并记录是哪一帧超时。 */
    for(i = start_key; i < end_key; i++)
    {
        if(s_u1w.key_age_10ms[i] >= U1W_KEY_RX_TIMEOUT_10MS)
        {
            uart_1_wire.comm_timeout = 1U;
            uart_1_wire.retry_over = 1U;
            uart_1_wire.last_error = U1W_ERR_COMM_LOST;
            uart_1_wire.link_state = U1W_LINK_OFFLINE;

            switch(i)
            {
            case U1W_KEY_A0: uart_1_wire.key_timeout_cmd = U1W_CMD_A0; break;
            case U1W_KEY_A1: uart_1_wire.key_timeout_cmd = U1W_CMD_A1; break;
            case U1W_KEY_A4: uart_1_wire.key_timeout_cmd = U1W_CMD_A4; break;
            case U1W_KEY_A6: uart_1_wire.key_timeout_cmd = U1W_CMD_A6; break;
            case U1W_KEY_A7: uart_1_wire.key_timeout_cmd = U1W_CMD_A7; break;
            case U1W_KEY_B1: uart_1_wire.key_timeout_cmd = U1W_CMD_B1; break;
            case U1W_KEY_B3: uart_1_wire.key_timeout_cmd = U1W_CMD_B3; break;
            case U1W_KEY_B4: uart_1_wire.key_timeout_cmd = U1W_CMD_B4; break;
            default:         uart_1_wire.key_timeout_cmd = 0U; break;
            }
            return;
        }
    }
}

/*============================== 对外接口 =====================================*/

void uart_1_wire_init(void)
{
    memclr(&uart_1_wire, sizeof(uart_1_wire));
    memclr(&s_u1w, sizeof(s_u1w));
    memclr(s_rx_buf, sizeof(s_rx_buf));
    memclr(s_tx_buf, sizeof(s_tx_buf));

    uart_1_wire.link_state = U1W_LINK_OFFLINE;
    uart_1_wire.last_error = U1W_ERR_NONE;
    uart_1_wire.cell_type = 0U;
    uart_1_wire.cell_series = BAT_SERIES;
    uart_1_wire.cell_parallel = 1U;
    uart_1_wire.cell_pre_mv = CELL_PRE_MV;
    uart_1_wire.target_voltage_mv = SET_vMAX;
    uart_1_wire.max_charge_current_ma = iMAX;
    uart_1_wire.derate_current_ma = iMAX;
    uart_1_wire.target_current_ma = iMAX;
    uart_1_wire.derate_low_degc = 10;
    uart_1_wire.derate_high_degc = 40;

    s_u1w.stage = U1W_STAGE_STOP;
    s_u1w.tx_tick_10ms = U1W_TX_PERIOD_10MS;

    com_fifo_clear();
    u1w_release_com();
}

void uart_1_wire_reset_link(void)
{
    uart_1_wire_init();
}

/**
  * @brief  设置一线通信阶段。
  * @note   切换阶段时，会重置发送节拍、接收缓存、超时计数和关键帧计时。
  */
void uart_1_wire_set_stage(u8 stage)
{
    u8 i;

    /* 非法阶段统一转为停止通信，避免状态越界。 */
    if(stage > U1W_STAGE_PULL_LOW)
    {
        stage = U1W_STAGE_STOP;
    }

    /* 阶段未变化，不重复清计数，避免影响当前通信。 */
    if(s_u1w.stage == stage)
    {
        return;
    }

    /* 切换通信阶段后，重新从本阶段第一帧开始发送。 */
    s_u1w.stage = stage;
    s_u1w.tx_index = 0U;
    s_u1w.tx_tick_10ms = U1W_TX_PERIOD_10MS;
    s_u1w.rx_len = 0U;

    /* 清通信超时相关计数和标志。 */
    s_u1w.any_rx_age_10ms = 0U;
    s_u1w.full_display_10ms = 0U;
    uart_1_wire.offline_count_10ms = 0U;
    uart_1_wire.comm_timeout = 0U;
    uart_1_wire.retry_over = 0U;
    uart_1_wire.key_timeout_cmd = 0U;

    /* 清各关键帧刷新计时。 */
    for(i = 0U; i < U1W_KEY_MAX; i++)
    {
        s_u1w.key_age_10ms[i] = 0U;
    }

    if(stage == U1W_STAGE_HANDSHAKE)
    {
        /* 重新握手时，必须重新收齐 A0/A1/A4/A6/A7/B1/B3/B4。 */
        uart_1_wire.handshake_mask = 0U;
        uart_1_wire.link_state = U1W_LINK_HANDSHAKE;
        u1w_release_com();
    }
    else if(stage == U1W_STAGE_STOP)
    {
        /* 停止通信：释放 COM，不主动拉低。 */
        uart_1_wire.link_state = U1W_LINK_OFFLINE;
        u1w_release_com();
    }
    else if(stage == U1W_STAGE_PULL_LOW)
    {
        /* 主动拉低 COM，通知 BMS 主机结束通信。 */
        uart_1_wire.link_state = U1W_LINK_OFFLINE;
        u1w_pull_com_low();
    }
    else
    {
        /* 其他通信阶段需要释放 COM，让总线正常收发。 */
        u1w_release_com();
    }

    /* 阶段切换后，丢弃旧缓存，避免上一阶段残留数据影响新阶段。 */
    com_fifo_clear();
}

static void u1w_refresh_info(void)
{
    u1w_info.stage = s_u1w.stage;
    u1w_info.handshake_ok = ((uart_1_wire.handshake_mask & U1W_HANDSHAKE_MASK) == U1W_HANDSHAKE_MASK) ? 1U : 0U;
    u1w_info.comm_timeout = uart_1_wire.comm_timeout;
    u1w_info.key_timeout_cmd = uart_1_wire.key_timeout_cmd;
    u1w_info.soc_percent = uart_1_wire.soc_percent;
    u1w_info.charge_status = uart_1_wire.charge_status;
    u1w_info.cell_type = uart_1_wire.cell_type;
    u1w_info.batt_temp_degc = uart_1_wire.batt_temp_degc;
    u1w_info.mos_temp_degc = uart_1_wire.mos_temp_degc;
    u1w_info.target_voltage_mv = U1W_LIMIT_VOLTAGE(uart_1_wire.target_voltage_mv);
    u1w_info.target_current_ma = U1W_LIMIT_CURRENT(uart_1_wire.target_current_ma);
    u1w_info.cell_max_mv = uart_1_wire.cell_max_mv;
    u1w_info.no_rx_10ms = s_u1w.any_rx_age_10ms;
}

/**
  * @brief  一线通信 10ms 周期任务。
  * @note   主流程每 10ms 调用一次，内部完成接收、超时计时和周期发送。
  */
void uart_1_wire_poll_10ms(void)
{
    /*
     * 主动拉低 COM 阶段：
     * 持续保持 COM 低电平，不再收发协议帧。
     */
    if(s_u1w.stage == U1W_STAGE_PULL_LOW)
    {
        u1w_pull_com_low();
        u1w_refresh_info();
        return;
    }

    /* 先处理接收，合法帧会清通信计时并更新协议数据。 */
    u1w_rx_task();

    /* 再处理超时计时，判断总通信或关键帧是否超时。 */
    u1w_age_task_10ms();

    /* 最后按当前阶段和 100ms 节拍发送下一帧。 */
    u1w_tx_task();

    /* 更新给 ch.c 直接读取的通信快照。 */
    u1w_refresh_info();
}

void uart_1_wire_task_10ms(void)
{
    uart_1_wire_poll_10ms();
}

