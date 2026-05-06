/**
  ******************************************************************************
  * @file    timer.c
  * @brief   TIMER4 ?? 1ms tick????? time_after ? 10ms ?????
  ******************************************************************************
  */
#include "timer.h"
data bit flg_10ms;
volatile u16 idata g_sys_tick_ms;
static u8 idata s_tick_10ms_count;
void timer_init(void)
{
#define TIMER4_1MS_PERIOD_VAL          (SYS_CLK_HZ / 32UL / 1000UL - 1UL)
    __EnableIRQ(TMR4_IRQn);
    TMR_ALLCON = TMR4_CNT_CLR(0x1);
    TMR4_CAP10 = TMR4_PRD_VAL_L((u8)((TIMER4_1MS_PERIOD_VAL >> 0) & 0xFFU));
    TMR4_CAP11 = TMR4_PRD_VAL_H((u8)((TIMER4_1MS_PERIOD_VAL >> 8) & 0xFFU));
    TMR4_CON0 = TMR4_PRESCALE_SEL(0x5) |
                TMR4_SOURCE_SEL(0x7) |
                TMR4_MODE_SEL(0x0);
    TMR4_IE0 = TMR4_PRD_IRQ_EN(0x1);
    TMR4_EN = TMR4_EN(0x1);
}
void timer_isr_tick_1ms(void)
{
    g_sys_tick_ms++;
    s_tick_10ms_count++;
    if(s_tick_10ms_count >= TASK_10MS)
    {
        s_tick_10ms_count = 0U;
        flg_10ms = 1;
    }
}
u16 timer_get_tick_ms(void)
{
    bit ea_bak;
    u16 tick_ms;
    ea_bak = IE_EA;
    IE_EA = 0;
    tick_ms = g_sys_tick_ms;
    IE_EA = ea_bak;
    return tick_ms;
}
u16 timer_deadline_ms(u16 delay_ms)
{
    return (u16)(timer_get_tick_ms() + delay_ms);
}
bit timer_period_elapsed(u16 *next_ms, u16 period_ms)
{
    u16 now_ms;
    if((next_ms == 0) || (period_ms == 0U))
    {
        return 0;
    }
    now_ms = timer_get_tick_ms();
    if((now_ms == *next_ms) || time_after(now_ms, *next_ms))
    {
        do
        {
            *next_ms = (u16)(*next_ms + period_ms);
        } while((now_ms == *next_ms) || time_after(now_ms, *next_ms));
        return 1;
    }
    return 0;
}
void delay_1us(u16 us)
{
    u8 i;
    while(us != 0U)
    {
        for(i = 0U; i < 4U; i++)
        {
            _nop_();
        }
        us--;
    }
}
void TIME2_Init(void)
{
    timer_init();
}