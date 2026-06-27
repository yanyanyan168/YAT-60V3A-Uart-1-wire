/**
  ******************************************************************************
  * @file    uart.h
  * @brief   UART 驱动头文件，包含波特率计算宏及函数声明
  ******************************************************************************
  */
#ifndef __BSP_UART_H__
#define __BSP_UART_H__
#include "common.h"

/* 波特率寄存器值计算宏: (系统时钟 - 波特率) / 波特率 */
#define UART_BAUD_REG_VAL(baud)             ((u16)((SYS_CLK_HZ - (baud)) / (baud)))

/* 常用波特率寄存器值快捷定义 */
#define BAUD_4800                           UART_BAUD_REG_VAL(4800UL)
#define BAUD_9600                           UART_BAUD_REG_VAL(9600UL)
#define BAUD_19200                          UART_BAUD_REG_VAL(19200UL)
#define BAUD_38400                          UART_BAUD_REG_VAL(38400UL)
#define BAUD_115200                         UART_BAUD_REG_VAL(115200UL)

/* 波特率设置快捷宏 */
#define SET_BAUD(x)                         ch_uart_set_baud_reg((u16)(x))
 
/* UART初始化及通道0(调试)相关函数声明 */
void uart_init(void);
void ch_uart_init(void);
void ch_uart_set_baud(u32 baud);
void ch_uart_set_baud_reg(u16 baud_reg);
void ch_uart_send_byte(u8 dat);
void ch_uart_send_buf(u8 *buf, u8 len);
void ch_uart_send_str(char *str);

/* 通道1(单线通信)相关函数声明 */
void com_uart_init(void);
void com_uart_set_rx_mode(void);
void com_uart_set_tx_mode(void);
UART_1WIRE_DIR_Types com_uart_get_dir(void);
BSP_RESULT_Types com_uart_send_byte(u8 dat);
BSP_RESULT_Types com_uart_send_buf(u8 *buf, u8 len);
 
#endif
