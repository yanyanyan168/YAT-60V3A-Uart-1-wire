/**
  ******************************************************************************
  * @file    isr.c
  * @brief   UART RX 与 TIMER tick 中断入口。
  *
  * 约束：中断中只读取硬件标志、入 FIFO、置 tick 标志，不解析协议。
  ******************************************************************************
  */
#include "isr.h"
#include "uart.h"
#include "timer.h"
#include "fifo.h"

void TMR4_IRQHandler(void) interrupt TMR4_IRQn
{
    __IRQnIPnPush(TMR4_IRQn);

    if(TMR4_FLAG0 & TMR4_PRD_FLAG(0x1))
    {
        TMR4_CLR0 |= TMR4_PRD_FLG_CLR(0x1);
        timer_isr_tick_1ms();
    }

    __IRQnIPnPop(TMR4_IRQn);
}

void UART0_IRQHandler(void) interrupt UART0_IRQn
{
    u8 dat;

    __IRQnIPnPush(UART0_IRQn);

    if(UART0_STA & UART_RX_DONE(0x1))
    {
        dat = UART0_DATA;
        ch_fifo_push(dat);
    }

    __IRQnIPnPop(UART0_IRQn);
}

void UART1_IRQHandler(void) interrupt UART1_IRQn
{
    u8 dat;

    __IRQnIPnPush(UART1_IRQn);

    if(UART1_STA & UART_RX_DONE(0x1))
    {
        dat = UART1_DATA;
        com_fifo_push(dat);
    }

    __IRQnIPnPop(UART1_IRQn);
}
