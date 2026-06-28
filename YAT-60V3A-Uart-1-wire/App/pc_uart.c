/**
  ******************************************************************************
  * @file    pc_uart.c
  * @brief   DEBUG 串口交互逻辑。
  *
  * 兼容点：
  * 1. P24/P25 DEBUG 通道的接收数据来自 ch_rx_fifo。
  * 2. 收到 "*RST" 返回 1，主流程切入 usr_cal_func()。
  * 3. 保留 "par" 参数打印、"ENA" 自动上传和 40 字节调试帧格式。
  * 4. 只处理 DEBUG 调试口，不解析 P30 单线通信协议。
  ******************************************************************************
  */
#include "pc_uart.h"
#include "fifo.h"
#include "uart.h"
#include "adc.h"
#include "ch.h"
#include "timer.h"
#include "stm32_crc.h"

#define PC_UART_DATA_LEN                 (36U)
#define PC_UART_FRAME_LEN                (40U)
#define PC_UART_RX_BUF_LEN               (16U)

u8 idata data_len_bk;

static u8 idata s_auto_count;
static bit s_tx_auto;
static u8 xdata s_pc_tx_buf[PC_UART_FRAME_LEN] = { 0 };
static u8 xdata s_pc_rx_buf[PC_UART_RX_BUF_LEN];

/**
  * @brief  打包保护标志，方便 PC 调试工具显示。
  */
static u8 pc_uart_pack_flag(void)
{
    u8 flag;

    flag = 0U;
    if(ch_flag.ch_ntcErr != 0U) { flag |= BIT8(0); }
    if(ch_flag.ch_hotErr != 0U) { flag |= BIT8(1); }
    if(ch_flag.ch_hot != 0U)    { flag |= BIT8(2); }
    if(ch_flag.ch_ovp != 0U)    { flag |= BIT8(3); }
    if(ch_flag.ch_ocp != 0U)    { flag |= BIT8(4); }
    if(ch_flag.ch_vacErr != 0U) { flag |= BIT8(5); }
    if(ch_flag.ch_hard != 0U)   { flag |= BIT8(6); }

    return flag;
}

static void pc_uart_print_current_line(char *name, u16 current_ma)
{
    uart_printf("%s  %umA\n", name, current_ma);
}

static void pc_uart_print_voltage_line(char *name, u16 voltage_mv)
{
    uart_printf("%s  %umV\n", name, voltage_mv);
}

static void pc_uart_print_voltage_range_line(char *name, u16 low_mv, u16 high_mv)
{
    uart_printf("%s  %u-%umV\n", name, low_mv, high_mv);
}

/**
  * @brief  打印整机参数，保留原调试口 "par" 命令入口。
  */
static void pc_uart_print_param(void)
{
    uart_printf("%s %s\n", MODEL_NAME, VERSION);
    uart_printf("\n%s\n", PROJECT_NAME);

    pc_uart_print_current_line("修复电流", iREPAIR);
    pc_uart_print_current_line("预充电流", iPRE);
    pc_uart_print_current_line("恒流电流", iMAX);
    pc_uart_print_current_line("转灯电流", iGED);
    pc_uart_print_current_line("保护电流", iOCP);
    pc_uart_print_current_line("电流恢复", iOCP_OK);

    pc_uart_print_voltage_line("\n识别电压", vSTART);
    pc_uart_print_voltage_line("修复结束", vPRE_30V);
    pc_uart_print_voltage_range_line("预充电压", vPRE_30V, vPRE_37V5);
    pc_uart_print_voltage_line("最高电压", SET_vMAX);
    pc_uart_print_voltage_line("高压保护", vDCOVP);

    uart_printf("\n内部NTC:\n");
    uart_printf("高温关断 ADC>%u\n", T_HOT_ERR);
    uart_printf("高温恢复 ADC<%u\n", T_HOT_ERR_OK);
    uart_printf("高温提示 ADC>%u\n", T_CH_HOT);
    uart_printf("提示恢复 ADC<%u\n", T_CH_HOT_OK);

    uart_printf("\n预充定时 %umin\n", TIM_PRE);
    uart_printf("CC+CV定时 %umin\n", TIM_CCCV);

    uart_printf("上拉电阻: %uk\n", (u16)R1);
    uart_printf("下拉电阻: %uk\n", (u16)R2);
    uart_printf("电流电阻: %umR\n", (u16)Ra);
    uart_printf("GAIN: %u\n", (u16)GAIN);

    uart_printf("\n状态:\n0空载\n1 检测\n2 握手\n3 预充\n4 CCCV\n5 满电\n6 OVP\n7 预超\n8 OTP\n");
    uart_printf("9 OCP\n10 NTC\n11 硬件\n12 欠压\n13 CV超时\n14 BMS温\n15 BMS异常\n16 修复\n17 老化\n");
    uart_printf("当前状态: %u\n", (u16)ch_state);
}

/**
  * @brief  打印当前采样和状态，便于 DEBUG 人工联调。
  */
static void pc_uart_print_batt(void)
{
    uart_printf("[BATT] vout=%u mV, curr=%u mA, ntc=%d(0.1C)\r\n",
                val.vout,
                val.curr,
                (s16)val.i_ntc);
    uart_printf("[BATT] state=%bu, flag=%bu, tick=%u\r\n",
                (u8)ch_state,
                pc_uart_pack_flag(),
                timer_get_tick_ms());
}

/**
  * @brief  写入 u16/u32 小端数据，保持 54.6V 原调试帧字节序。
  */
static void pc_uart_put_u16(u8 *buf, u8 index, u16 value)
{
    buf[index + 0U] = (u8)value;
    buf[index + 1U] = (u8)(value >> 8);
}

static void pc_uart_put_u32(u8 *buf, u8 index, u32 value)
{
    buf[index + 0U] = (u8)value;
    buf[index + 1U] = (u8)(value >> 8);
    buf[index + 2U] = (u8)(value >> 16);
    buf[index + 3U] = (u8)(value >> 24);
}

/**
  * @brief  发送 54.6V 风格的 40 字节自动调试帧。
  */
static void pc_uart_send_auto_frame(void)
{
    u32 crc32;

    pc_uart_put_u32(s_pc_tx_buf, 0U,  (u32)val.vout);
    pc_uart_put_u32(s_pc_tx_buf, 4U,  (u32)val.vdc);   /* 本项目无独立 VDC，val.vdc 由 ADC 层镜像 val.vout。 */
    pc_uart_put_u32(s_pc_tx_buf, 8U,  (u32)val.curr);
    pc_uart_put_u32(s_pc_tx_buf, 12U, 0UL);
    pc_uart_put_u16(s_pc_tx_buf, 16U, 0U);
    pc_uart_put_u16(s_pc_tx_buf, 18U, (u16)val.i_ntc);

    s_pc_tx_buf[20] = (u8)ch_state;
    s_pc_tx_buf[21] = pc_uart_pack_flag();

    pc_uart_put_u16(s_pc_tx_buf, 22U, 0U);

    pc_uart_put_u32(s_pc_tx_buf, 28U, 0UL);
    pc_uart_put_u16(s_pc_tx_buf, 32U, 0U);
    pc_uart_put_u16(s_pc_tx_buf, 34U, JIZHONG);

    crc32 = CRC32(s_pc_tx_buf, PC_UART_DATA_LEN);
    pc_uart_put_u32(s_pc_tx_buf, 36U, crc32);
    ch_uart_send_buf((char *)s_pc_tx_buf, PC_UART_FRAME_LEN);
}



/**
  * @brief  DEBUG 串口任务。
  *
  * @retval 1: 收到 "*RST"，主流程切入校准；0: 保持当前充电流程。
  */
u8 pc_uart_func(void)
{
    u8 data_len;
    u8 i;
    u8 status;

    status = 0U;

    data_len = s_ch_fifo.cnt;
    if((data_len == data_len_bk)&&(data_len != 0))
    {
        if(data_len > PC_UART_RX_BUF_LEN)
        {
            data_len = PC_UART_RX_BUF_LEN;
        }

        for(i = 0U; i < data_len; i++)
        {
            s_pc_rx_buf[i] = ch_fifo_pop();
        }

        if(data_len >= 4U)
        {
            if((s_pc_rx_buf[0] == '*') && (s_pc_rx_buf[1] == 'R') &&
               (s_pc_rx_buf[2] == 'S') && (s_pc_rx_buf[3] == 'T'))
            {
                status = 1U;
            }
            else if((s_pc_rx_buf[0] == 'b') && (s_pc_rx_buf[1] == 'a') &&
                    (s_pc_rx_buf[2] == 't') && (s_pc_rx_buf[3] == 't'))
            {
                pc_uart_print_batt();
            }
        }

        if(data_len >= 3U)
        {
            if((s_pc_rx_buf[0] == 'p') && (s_pc_rx_buf[1] == 'a') && (s_pc_rx_buf[2] == 'r'))
            {
                pc_uart_print_param();
            }
            else if((s_pc_rx_buf[0] == 'E') && (s_pc_rx_buf[1] == 'N') && (s_pc_rx_buf[2] == 'A'))
            {
                if(++s_auto_count >= 20U)
                {
                    s_tx_auto = 1;
                    s_auto_count = 0U;
                    pc_uart_send_auto_frame();
                }
            }
        }
    }
    
    data_len_bk = data_len;
    
    return status;
}
