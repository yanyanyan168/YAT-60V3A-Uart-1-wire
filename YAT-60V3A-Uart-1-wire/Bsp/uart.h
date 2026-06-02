/**
  ******************************************************************************
  * @file    uart.h
  * @brief  
  ******************************************************************************
  */
#ifndef __BSP_UART_H__
#define __BSP_UART_H__
#include "common.h"
#define UART_BAUD_REG_VAL(baud)             ((u16)((SYS_CLK_HZ - (baud)) / (baud)))
#define BAUD_4800                           UART_BAUD_REG_VAL(4800UL)
#define BAUD_9600                           UART_BAUD_REG_VAL(9600UL)
#define BAUD_19200                          UART_BAUD_REG_VAL(19200UL)
#define BAUD_38400                          UART_BAUD_REG_VAL(38400UL)
#define BAUD_115200                         UART_BAUD_REG_VAL(115200UL)
#define SET_BAUD(x)                         ch_uart_set_baud_reg((u16)(x))

void uart_init(void);
void ch_uart_init(void);
void ch_uart_set_baud(u32 baud);
void ch_uart_set_baud_reg(u16 baud_reg);
void ch_uart_send_byte(u8 dat);
void ch_uart_send_buf(u8 *buf, u8 len);
void ch_uart_send_str(char *str);
void com_uart_init(void);
void com_uart_set_rx_mode(void);
void com_uart_set_tx_mode(void);
UART_1WIRE_DIR_Types com_uart_get_dir(void);
BSP_RESULT_Types com_uart_send_byte(u8 dat);
BSP_RESULT_Types com_uart_send_buf(u8 *buf, u8 len);

#endif