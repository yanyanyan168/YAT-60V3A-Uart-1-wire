/**
  ******************************************************************************
  * @file    main.c
  * @brief   60V3A 充电器应用入口，保持 54.6V 项目主循环风格。
  ******************************************************************************
  */
#include "bsp_init.h"
#include "ch.h"
#include "cal.h"
#include "uart_1_wire.h"
#include "common.h"
#include "timer.h"
#include "fifo.h"
#include "uart.h"

bit flg_cal_mode = 0;     /* 0: 充电流程  1: DEBUG 校准流程 */
data u16 next_10ms;

/*
 * 一线通信手动单步测试入口。
 * 当前用于硬件通信未调通阶段：
 * 1. 不跑充电流程；
 * 2. 不做通信超时；
 * 3. 不自动切下一条命令；
 * 4. 通过 DEBUG 串口按键手动发送每一条一线命令；
 * 5. COM 收到任何字节都原始打印，方便看硬件波形和回包。
 */
#define MAIN_1WIRE_TEST_EN              (0U)
#define MAIN_1WIRE_TEST_CMD_NUM         (9U)
#define MAIN_1WIRE_INFO_PERIOD_10MS     (100U)

#if (MAIN_1WIRE_TEST_EN != 0U)
static u8 s_1w_test_idx;
static u8 s_flow_run;
static u8 s_flow_charge;
static u8 s_flow_timeout_printed;
static u16 s_flow_info_tick_10ms;

static void test_put_char(u8 dat)
{
    ch_uart_send_byte(dat);
}

static void test_put_str(char *str)
{
    while(*str != 0)
    {
        test_put_char((u8)(*str));
        str++;
    }
}

static void test_put_crlf(void)
{
    test_put_char(0x0DU);
    test_put_char(0x0AU);
}

static void test_put_hex4(u8 dat)
{
    dat &= 0x0FU;
    if(dat < 10U)
    {
        test_put_char((u8)('0' + dat));
    }
    else
    {
        test_put_char((u8)('A' + dat - 10U));
    }
}

static void test_put_hex8(u8 dat)
{
    test_put_hex4((u8)(dat >> 4));
    test_put_hex4(dat);
}

static void test_put_bytes(u8 *buf, u8 len)
{
    u8 i;

    for(i = 0U; i < len; i++)
    {
        test_put_hex8(buf[i]);
        test_put_char(' ');
    }
}

static u8 test_sum(u8 *buf, u8 len)
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

static u8 test_get_cmd_by_idx(u8 idx)
{
    switch(idx)
    {
    case 0U: return U1W_CMD_A0;
    case 1U: return U1W_CMD_A1;
    case 2U: return U1W_CMD_A4;
    case 3U: return U1W_CMD_A6;
    case 4U: return U1W_CMD_A7;
    case 5U: return U1W_CMD_B1;
    case 6U: return U1W_CMD_B3;
    case 7U: return U1W_CMD_B4;
    default: return U1W_CMD_B6;
    }
}

static void test_print_current_cmd(void)
{
    test_put_str("[1W] 当前序号=");
    test_put_char((u8)('0' + s_1w_test_idx));
    test_put_str(" 命令=0x");
    test_put_hex8(test_get_cmd_by_idx(s_1w_test_idx));
    test_put_crlf();
}

static void test_print_help(void)
{
    test_put_str("[1W] 手动单步测试");
    test_put_crlf();
    test_put_str("[1W] 按键：s发送 r重发 n下一条 p上一条 0~8选择 g流程 x停止 ?帮助");
    test_put_crlf();
    test_put_str("[1W] 命令：0=A0 1=A1 2=A4 3=A6 4=A7 5=B1 6=B3 7=B4 8=B6");
    test_put_crlf();
    test_put_str("[1W] 无超时，不自动下一条");
    test_put_crlf();
    test_print_current_cmd();
}

static void test_make_frame(u8 cmd, u8 *buf, u8 *len)
{
    buf[0] = cmd;

    if(cmd == U1W_CMD_A0)
    {
        buf[1] = 0x00U;
        buf[2] = U1W_MASTER;
        buf[3] = test_sum(buf, 3U);
        *len = 4U;
    }
    else if(cmd == U1W_CMD_B6)
    {
        buf[1] = 0x01U;
        buf[2] = 0x01U;
        buf[3] = test_sum(buf, 3U);
        *len = 4U;
    }
    else
    {
        buf[1] = cmd;
        *len = 2U;
    }
}

static void test_send_current_cmd(void)
{
    u8 cmd;
    u8 tx_len;
    u8 tx_buf[4];

    cmd = test_get_cmd_by_idx(s_1w_test_idx);
    test_make_frame(cmd, tx_buf, &tx_len);

    test_put_str("[1W] 发送序号=");
    test_put_char((u8)('0' + s_1w_test_idx));
    test_put_str(" 命令=0x");
    test_put_hex8(cmd);
    test_put_str(" 字节=");
    test_put_bytes(tx_buf, tx_len);
    test_put_crlf();

    if(com_uart_send_buf(tx_buf, tx_len) != BSP_OK)
    {
        test_put_str("[1W] 发送失败");
        test_put_crlf();
    }

    com_uart_set_rx_mode();
}

static void test_next_cmd(void)
{
    s_1w_test_idx++;
    if(s_1w_test_idx >= MAIN_1WIRE_TEST_CMD_NUM)
    {
        s_1w_test_idx = 0U;
    }

    test_send_current_cmd();
}

static void test_prev_cmd(void)
{
    if(s_1w_test_idx == 0U)
    {
        s_1w_test_idx = MAIN_1WIRE_TEST_CMD_NUM - 1U;
    }
    else
    {
        s_1w_test_idx--;
    }

    test_send_current_cmd();
}



static void test_put_u16(u16 dat)
{
    char buf[5];
    u8 i;
    u8 started;

    buf[0] = (char)('0' + (dat / 10000U));
    dat %= 10000U;
    buf[1] = (char)('0' + (dat / 1000U));
    dat %= 1000U;
    buf[2] = (char)('0' + (dat / 100U));
    dat %= 100U;
    buf[3] = (char)('0' + (dat / 10U));
    buf[4] = (char)('0' + (dat % 10U));

    started = 0U;
    for(i = 0U; i < 5U; i++)
    {
        if((buf[i] != '0') || (started != 0U) || (i == 4U))
        {
            started = 1U;
            test_put_char((u8)buf[i]);
        }
    }
}

static void test_put_s8(s8 dat)
{
    if(dat < 0)
    {
        test_put_char('-');
        dat = (s8)(0 - dat);
    }
    test_put_u16((u16)((u8)dat));
}

static void test_put_01_unit(u16 raw)
{
    test_put_u16((u16)(raw / 10U));
    test_put_char('.');
    test_put_char((u8)('0' + (raw % 10U)));
}

static void test_print_flow_info(void)
{
    test_put_str("[流程测试] 包=");
    test_put_u16(uart_1_wire.cell_series);
    test_put_str("串");
    test_put_u16(uart_1_wire.cell_parallel);
    test_put_str("并 容量=");
    test_put_01_unit(uart_1_wire.cell_cap_01ah);
    test_put_str("Ah 目标电压mV=");
    test_put_u16(uart_1_wire.target_voltage_mv);
    test_put_str(" 目标电流mA=");
    test_put_u16(uart_1_wire.target_current_ma);
    test_put_crlf();

    test_put_str("[流程测试] 单高mV=");
    test_put_u16(uart_1_wire.cell_max_mv);
    test_put_str(" 电量=");
    test_put_u16(uart_1_wire.soc_percent);
    test_put_str(" 温度=");
    test_put_s8(uart_1_wire.batt_temp_degc);
    test_put_str(" MOS=");
    test_put_s8(uart_1_wire.mos_temp_degc);
    test_put_str(" 状态=0x");
    test_put_hex8(uart_1_wire.charge_status);
    test_put_crlf();
}

static void test_flow_start(void)
{
    s_flow_run = 1U;
    s_flow_charge = 0U;
    s_flow_timeout_printed = 0U;
    s_flow_info_tick_10ms = 0U;

    com_fifo_clear();
    uart_1_wire_reset_link();
    uart_1_wire_set_stage(U1W_STAGE_HANDSHAKE);

    test_put_str("[测试] 协议启动");
    test_put_crlf();
}

static void test_flow_stop(void)
{
    s_flow_run = 0U;
    s_flow_charge = 0U;
    s_flow_timeout_printed = 0U;
    s_flow_info_tick_10ms = 0U;

    uart_1_wire_set_stage(U1W_STAGE_STOP);
    com_fifo_clear();
    com_uart_set_rx_mode();

    test_put_str("[测试] 协议停止，回到手动模式");
    test_put_crlf();
}

static void test_flow_task(void)
{
    if(s_flow_run == 0U)
    {
        return;
    }

    uart_1_wire_poll_10ms();

    if((u1w_info.comm_timeout != 0U) && (s_flow_timeout_printed == 0U))
    {
        s_flow_timeout_printed = 1U;
        test_put_str("[测试] 通信超时 命令=0x");
        test_put_hex8(u1w_info.key_timeout_cmd);
        test_put_crlf();
    }

    if((s_flow_charge == 0U) && (u1w_info.handshake_ok != 0U))
    {
        s_flow_charge = 1U;
        test_put_str("[测试] 握手完成");
        test_put_crlf();
        test_print_flow_info();
        uart_1_wire_set_stage(U1W_STAGE_CHARGE);
        test_put_str("[测试] 切到充电轮询");
        test_put_crlf();
    }

    s_flow_info_tick_10ms++;
    if(s_flow_info_tick_10ms >= MAIN_1WIRE_INFO_PERIOD_10MS)
    {
        s_flow_info_tick_10ms = 0U;
        test_print_flow_info();
    }
}


static void test_debug_key_task(void)
{
    u8 dat;

    while(s_ch_fifo.cnt != 0U)
    {
        dat = ch_fifo_pop();

        if((dat == 'g') || (dat == 'G'))
        {
            test_flow_start();
        }
        else if((dat == 'x') || (dat == 'X'))
        {
            test_flow_stop();
        }
        else if(dat == '?')
        {
            test_print_help();
        }
        else if(s_flow_run != 0U)
        {
            /* 流程测试运行中，只响应 g/x/?，其他按键忽略。 */
        }
        else if((dat >= '0') && (dat <= '8'))
        {
            s_1w_test_idx = (u8)(dat - '0');
            test_send_current_cmd();
        }
        else if((dat == 's') || (dat == 'S') || (dat == 'r') || (dat == 'R'))
        {
            test_send_current_cmd();
        }
        else if((dat == 'n') || (dat == 'N'))
        {
            test_next_cmd();
        }
        else if((dat == 'p') || (dat == 'P'))
        {
            test_prev_cmd();
        }
        else
        {
            /* 忽略回车、换行和其他无效按键。 */
        }
    }
}

static void test_rx_task(void)
{
    u8 dat;

    if(s_com_fifo.cnt == 0U)
    {
        return;
    }

    test_put_str("[1W] 接收=");
    while(s_com_fifo.cnt != 0U)
    {
        dat = com_fifo_pop();
        test_put_hex8(dat);
        test_put_char(' ');
    }
    test_put_crlf();
}

static void usr_1wire_test_func(void)
{
    s_1w_test_idx = 0U;

    ch_fifo_clear();
    com_fifo_clear();
    com_uart_set_rx_mode();
    next_10ms = timer_deadline_ms(TASK_10MS);

    test_print_help();

    for(;;)
    {
        if(timer_period_elapsed(&next_10ms, TASK_10MS) != 0)
        {
            wdt_feed();
            test_debug_key_task();
            if(s_flow_run != 0U)
            {
                test_flow_task();
            }
            else
            {
                test_rx_task();
            }
        }
    }
}
#endif

void main(void)
{
    Sys_Init();
    read_cal_data();
    uart_1_wire_init();

    usr_printf("%s %s\r\n", PROJECT_NAME, VERSION);

    for(;;)
    {
#if (MAIN_1WIRE_TEST_EN != 0U)
        usr_1wire_test_func();
#else
        if(flg_cal_mode == 0)
        {
            usr_ch_func();
        }
        else
        {
            usr_cal_func();
        }
#endif
    }
}
