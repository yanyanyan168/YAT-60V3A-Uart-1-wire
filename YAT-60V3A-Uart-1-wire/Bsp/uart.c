/**
  ******************************************************************************
  * @file    uart.c
  * @brief   UART ??????RX ??? FIFO???????
  ******************************************************************************
  */
#include "uart.h"
#include "gpio.h"
#include "fifo.h"

static UART_1WIRE_DIR_Types idata s_com_dir;

#if DBG_PRINTF_EN
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
void ch_uart_set_baud_reg(u16 baud_reg)
{
    UART0_BAUD1 = (u8)((baud_reg >> 8) & 0xFFU);
    UART0_BAUD0 = (u8)(baud_reg & 0xFFU);
}
void ch_uart_set_baud(u32 baud)
{
    if(baud == 0UL)
    {
        baud = DBG_BAUD;
    }
    ch_uart_set_baud_reg(UART_BAUD_REG_VAL(baud));
}

void ch_uart_init(void)
{
    DEBUG_TX_OUTPUT();
    DEBUG_RX_INPUT();
    FOUT_S25 = GPIO_FOUT_UART0_TX;
    FIN_S7 = GPIO_FIN_SEL_P24;
    __DisableIRQ(UART0_IRQn);
    ch_uart_set_baud(DBG_BAUD);
    UART0_CON0 = UART_STOP_BIT(0x0) |
                 UART_TX_INV(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1);
    __EnableIRQ(UART0_IRQn);
}

void ch_uart_send_byte(u8 dat)
{
    u32 wait_loop;
    wait_loop = UART_TX_WAIT;
    while(((UART0_STA & UART_TX_DONE(0x1)) == 0U) && (wait_loop != 0UL))
    {
        wait_loop--;
    }
    UART0_DATA = dat;
    wait_loop = UART_TX_WAIT;
    while(((UART0_STA & UART_TX_DONE(0x1)) == 0U) && (wait_loop != 0UL))
    {
        wait_loop--;
    }
}

void ch_uart_send_buf(u8 *buf, u8 len)
{
    while((buf != 0) && (len != 0U))
    {
        ch_uart_send_byte(*buf);
        buf++;
        len--;
    }
}

void ch_uart_send_str(char *str)
{
    while((str != 0) && (*str != '\0'))
    {
        ch_uart_send_byte((u8)*str);
        str++;
    }
}

static void com_uart_set_baud_reg(u16 baud_reg)
{
    UART1_BAUD1 = (u8)((baud_reg >> 8) & 0xFFU);
    UART1_BAUD0 = (u8)(baud_reg & 0xFFU);
}

void com_uart_set_rx_mode(void)
{
    __DisableIRQ(UART1_IRQn);
    FOUT_S04 = 0U;
    COM_UART_INPUT();
    FIN_S8 = GPIO_FIN_SEL_P04;
    UART1_CON0 = UART_STOP_BIT(0x0) |
                 UART_TX_INV(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1);
    s_com_dir = UART_1WIRE_DIR_RX;
    __EnableIRQ(UART1_IRQn);
}

void com_uart_set_tx_mode(void)
{
    __DisableIRQ(UART1_IRQn);
    FIN_S8 = 0U;
    COM_UART_OUTPUT();
    FOUT_S04 = GPIO_FOUT_UART1_TX;
    UART1_CON0 = UART_STOP_BIT(0x0) |
                 UART_TX_INV(0x0) |
                 UART_EN(0x1);
    s_com_dir = UART_1WIRE_DIR_TX;
}

UART_1WIRE_DIR_Types com_uart_get_dir(void)
{
    return s_com_dir;
}

void com_uart_init(void)
{
    com_uart_set_baud_reg(UART_BAUD_REG_VAL(COM_BAUD));
    com_uart_set_rx_mode();
}

BSP_RESULT_Types com_uart_send_byte(u8 dat)
{
    u32 wait_loop;
    wait_loop = UART_TX_WAIT;
    while(((UART1_STA & UART_TX_DONE(0x1)) == 0U) && (wait_loop != 0UL))
    {
        wait_loop--;
    }
    if(wait_loop == 0UL)
    {
        return BSP_ERR_TIMEOUT;
    }
    UART1_DATA = dat;
    wait_loop = UART_TX_WAIT;
    while(((UART1_STA & UART_TX_DONE(0x1)) == 0U) && (wait_loop != 0UL))
    {
        wait_loop--;
    }
    if(wait_loop == 0UL)
    {
        return BSP_ERR_TIMEOUT;
    }
    return BSP_OK;
}


BSP_RESULT_Types com_uart_send_buf(u8 *buf, u8 len)
{
    BSP_RESULT_Types ret;
    if((buf == 0) || (len == 0U))
    {
        return BSP_ERR_PARAM;
    }
    ret = BSP_OK;
    com_uart_set_tx_mode();
    while(len != 0U)
    {
        ret = com_uart_send_byte(*buf);
        if(ret != BSP_OK)
        {
            break;
        }
        buf++;
        len--;
    }
    com_uart_set_rx_mode();
    return ret;
}

void uart_init(void)
{
    ch_uart_init();
    com_uart_init();
}



void UART1_Init(void)
{
    ch_uart_init();
}

void uart1_tx(char *str, u8 len)
{
    ch_uart_send_buf((u8 *)str, len);
}