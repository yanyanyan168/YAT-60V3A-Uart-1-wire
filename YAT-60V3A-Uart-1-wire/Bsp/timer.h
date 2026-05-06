/**
  ******************************************************************************
  * @file    timer.h
  * @brief   1ms 系统 tick、10ms 软件周期和阻塞延时接口。
  ******************************************************************************
  */
#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "common.h"

extern bit flg_10ms;
extern volatile u16 idata g_sys_tick_ms;

void timer_init(void);
void timer_isr_tick_1ms(void);
u16 timer_get_tick_ms(void);
u16 timer_deadline_ms(u16 delay_ms);
bit timer_period_elapsed(u16 *next_ms, u16 period_ms);
void delay_1us(u16 us);

/* 54.6V 命名兼容。 */
void TIME2_Init(void);

#endif
