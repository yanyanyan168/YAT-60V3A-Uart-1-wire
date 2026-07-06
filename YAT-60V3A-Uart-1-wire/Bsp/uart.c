/**
  ******************************************************************************
  * @file    uart.c
  * @brief   UART 驱动文件，包含两个UART通道：
  *          1. UART0 (ch_uart): 用于调试打印，全双工 (P25-TX, P24-RX)
  *          2. UART1 (com_uart): 用于单线半双工通信，动态切换方向 (P30)
  ******************************************************************************
  */
#include "uart.h"
#include "gpio.h"
#include "fifo.h"
 
/* 静态变量：记录单线半双工通信通道(UART1)当前的通信方向 (TX 或 RX) */
static UART_1WIRE_DIR_Types idata s_com_dir;
 
#if DBG_PRINTF_EN
/* 当启用调试打印宏时，重写putchar函数，将标准输出重定向到UART0 */
char puchar(char c)
{
    ch_uart_send_byte((u8)c);
    return c;
}
char putchar(char c)
{
    return puchar(c);
}
#endif
 
/* 设置UART0波特率寄存器（直接写入计算好的寄存器值） */
void ch_uart_set_baud_reg(u16 baud_reg)
{
    // 写入波特率寄存器高八位
    UART0_BAUD1 = (u8)((baud_reg >> 8) & 0xFFU);
    // 写入波特率寄存器低八位
    UART0_BAUD0 = (u8)(baud_reg & 0xFFU);
}
 
/* 设置UART0波特率（传入实际波特率值，如9600，自动计算寄存器值） */
void ch_uart_set_baud(u32 baud)
{
    ch_uart_set_baud_reg(UART_BAUD_REG_VAL(baud));
}
 
/* 初始化UART0（调试通道） */
void ch_uart_init(u32 baud)
{
    DEBUG_TX_OUTPUT();             // 配置P25为推挽输出模式
    DEBUG_RX_INPUT();              // 配置P24为输入模式
    FOUT_S25 = GPIO_FOUT_UART0_TX; // 将P25输出功能复用选择为 UART0_TX
    FIN_S7 = GPIO_FIN_SEL_P24;     // 将P24输入功能复用选择为 UART0_RX
    
    __DisableIRQ(UART0_IRQn);      // 配置期间关闭UART0中断
    ch_uart_set_baud(baud);    // 设置调试波特率
    
    // 配置UART0控制寄存器:
    // UART_STOP_BIT(0x0) : 0x0对应1bit停止位
    // UART_TX_INV(0x0)   : 0x0对应TX电平不取反
    // UART_RX_IRQ_EN(0x1): 0x1对应使能RX中断
    // UART_EN(0x1)       : 0x1对应使能UART
    UART0_CON0 = UART_STOP_BIT(0x0) |
                 UART_TX_INV(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1);
    __EnableIRQ(UART0_IRQn);       // 配置完成，使能UART0中断
}
 
/* UART0发送单个字节（阻塞式，带超时等待） */
void ch_uart_send_byte(u8 dat)
{
    u32 wait_loop;
    wait_loop = UART_TX_WAIT;
    // 等待上一次发送完成
    while(((UART0_STA & UART_TX_DONE(0x1)) == 0U) && (wait_loop != 0UL))
    {
        wait_loop--;
    }
    
    // 向该寄存器写入数据则触发该数据的发送
    UART0_DATA = dat;
    
    wait_loop = UART_TX_WAIT;
    // 等待当前字节发送完成
    while(((UART0_STA & UART_TX_DONE(0x1)) == 0U) && (wait_loop != 0UL))
    {
        wait_loop--;
    }
}
 
/* UART0发送缓冲区数据 */
void ch_uart_send_buf(u8 *buf, u8 len)
{
    while((buf != 0) && (len != 0U))
    {
        ch_uart_send_byte(*buf);
        buf++;
        len--;
    }
}
 
/* UART0发送字符串 */
void ch_uart_send_str(char *str)
{
    while((str != 0) && (*str != '\0'))
    {
        ch_uart_send_byte((u8)*str);
        str++;
    }
}
 
/* 设置UART1波特率寄存器 */
static void com_uart_set_baud_reg(u16 baud_reg)
{
    // 写入波特率寄存器高八位
    UART1_BAUD1 = (u8)((baud_reg >> 8) & 0xFFU);
    // 写入波特率寄存器低八位
    UART1_BAUD0 = (u8)(baud_reg & 0xFFU);
}
 
/* 切换UART1为接收模式（单线半双工） */
void com_uart_set_rx_mode(void)
{
    __DisableIRQ(UART1_IRQn);       // 切换前关闭中断
    
    FOUT_S30 = 0U;                  // 将P30输出功能复用选择为0x0 (P30AF功能输出)，停止TX输出
    COM_UART_INPUT();               // 配置P30为输入模式
    FIN_S8 = GPIO_FIN_SEL_P30;      // 将P30输入功能复用选择为 UART1_RX
    
    // 配置UART1控制寄存器:
    // UART_STOP_BIT(0x0) : 0x0对应1bit停止位
    // UART_TX_INV(0x0)   : 0x0对应TX电平不取反
    // UART_RX_IRQ_EN(0x1): 0x1对应使能RX中断
    // UART_EN(0x1)       : 0x1对应使能UART
    UART1_CON0 = UART_STOP_BIT(0x0) |
                 UART_TX_INV(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1);
                 
    s_com_dir = UART_1WIRE_DIR_RX;  // 更新状态：当前为接收方向
    __EnableIRQ(UART1_IRQn);        // 恢复中断
}
 
/* 切换UART1为发送模式（单线半双工） */
void com_uart_set_tx_mode(void)
{
    __DisableIRQ(UART1_IRQn);       // 切换前关闭中断
    
    FIN_S8 = 0U;                    // 清除输入功能复用选择
    COM_UART_OUTPUT();              // 配置P30为推挽输出模式
    FOUT_S30 = GPIO_FOUT_UART1_TX;  // 将P30输出功能复用选择为 UART1_TX
    
    // 配置UART1控制寄存器:
    // UART_STOP_BIT(0x0) : 0x0对应1bit停止位
    // UART_TX_INV(0x0)   : 0x0对应TX电平不取反
    // UART_EN(0x1)       : 0x1对应使能UART (未配置UART_RX_IRQ_EN，即不使能RX中断)
    UART1_CON0 = UART_STOP_BIT(0x0) |
                 UART_TX_INV(0x0) |
                 UART_EN(0x1);
                 
    s_com_dir = UART_1WIRE_DIR_TX;  // 更新状态：当前为发送方向
}
 
/* 获取UART1当前的通信方向 */
UART_1WIRE_DIR_Types com_uart_get_dir(void)
{
    return s_com_dir;
}
 
/* 初始化UART1（通信通道），默认进入接收模式 */
void com_uart_init(void)
{
    com_uart_set_baud_reg(UART_BAUD_REG_VAL(COM_BAUD)); // 设置通信波特率
    com_uart_set_rx_mode();                             // 默认设为接收模式
}
 
/* UART1发送单个字节（带超时检测和错误返回） */
BSP_RESULT_Types com_uart_send_byte(u8 dat)
{
    u32 wait_loop;
    wait_loop = UART_TX_WAIT;
    // 等待上次发送完成
    while(((UART1_STA & UART_TX_DONE(0x1)) == 0U) && (wait_loop != 0UL))
    {
        wait_loop--;
    }
    if(wait_loop == 0UL)
    {
        return BSP_ERR_TIMEOUT;  // 等待超时返回错误
    }
    
    // 向该寄存器写入数据则触发该数据的发送
    UART1_DATA = dat;
    
    wait_loop = UART_TX_WAIT;
    // 等待当前字节发送完成
    while(((UART1_STA & UART_TX_DONE(0x1)) == 0U) && (wait_loop != 0UL))
    {
        wait_loop--;
    }
    if(wait_loop == 0UL)
    {
        return BSP_ERR_TIMEOUT;  // 等待超时返回错误
    }
    return BSP_OK;               // 发送成功
}
 
/* UART1发送缓冲区数据（自动处理方向切换） */
BSP_RESULT_Types com_uart_send_buf(u8 *buf, u8 len)
{
    BSP_RESULT_Types ret;
    if((buf == 0) || (len == 0U))
    {
        return BSP_ERR_PARAM;  // 参数校验
    }
    
    ret = BSP_OK;
    com_uart_set_tx_mode();    // 切换为发送模式
    
    while(len != 0U)           // 循环发送
    {
        ret = com_uart_send_byte(*buf);
        if(ret != BSP_OK)
        {
            break;             // 发送失败则跳出
        }
        buf++;
        len--;
    }
    
    com_uart_set_rx_mode();    // 发送完毕，自动切回接收模式
    return ret;
}
 
/* 初始化所有UART通道 */
void uart_init(void)
{
    ch_uart_init(DBG_BAUD);   // 初始化UART0 (调试通道)
    com_uart_init();  // 初始化UART1 (通信通道)
}