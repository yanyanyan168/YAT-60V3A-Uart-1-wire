/**
  ******************************************************************************
  * @file    timer.c
  * @brief   TIMER4 ?? 1ms tick????? time_after ? 10ms ?????
  ******************************************************************************
  */
#include "timer.h"

volatile u16 idata g_sys_tick_ms;
static u8 idata s_tick_10ms_count;

// 1ms 周期的中断
void timer_init(void)
{
    /* 
     * 定时器周期值计算：系统时钟 / 预分频(32) / 目标频率(1000Hz) - 1
     * 计算结果：24000000 / 32 / 1000 - 1 = 749
     * 配置的是 1ms 周期的中断
     */
#define TIMER4_1MS_PERIOD_VAL          (SYS_CLK_HZ / 32UL / 1000UL - 1UL)
    
    __EnableIRQ(TMR4_IRQn);                             // 使能TIMER4模块中断
    
    TMR_ALLCON = TMR4_CNT_CLR(0x1);                    // 清除Timer4计数值
    
    // 写入周期值低8位
    TMR4_CAP10 = TMR4_PRD_VAL_L((u8)((TIMER4_1MS_PERIOD_VAL >> 0) & 0xFFU));
    // 写入周期值高8位
    TMR4_CAP11 = TMR4_PRD_VAL_H((u8)((TIMER4_1MS_PERIOD_VAL >> 8) & 0xFFU));
    
    // 配置Timer4控制寄存器0
    // TMR4_PRESCALE_SEL(0x5) : 预分频选择为32分频
    // TMR4_SOURCE_SEL(0x7)   : 计数源选择为系统时钟
    // TMR4_MODE_SEL(0x0)     : 选择计数模式
    TMR4_CON0 = TMR4_PRESCALE_SEL(0x5) |
                TMR4_SOURCE_SEL(0x7)   |
                TMR4_MODE_SEL(0x0);
                
    TMR4_IE0 = TMR4_PRD_IRQ_EN(0x1);                   // 使能计数值等于周期值中断
    
    TMR4_EN = TMR4_EN(0x1);                            // 使能定时器4
}


void timer_isr_tick_1ms(void)
{
    g_sys_tick_ms++;
    s_tick_10ms_count++;
    if(s_tick_10ms_count >= TASK_10MS)
    {
        s_tick_10ms_count = 0U;
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


/**
  * @brief  判断指定周期是否已经到期。
  *
  * 到了周期点就返回 1；
  * 如果中间卡顿错过多个周期，只补一次，并把下一次时间推到未来。
  *
  * @param  next_ms
  *         下一次触发时间点的指针，单位 ms。
  *         调用前应先用 timer_deadline_ms(period_ms) 初始化。
  *
  * @param  period_ms
  *         周期时间，单位 ms。
  *
  * @retval 1  周期已到，本次需要执行任务。
  * @retval 0  周期未到，或者参数无效。
  *
  * @note   本函数适合在主循环中反复调用。
  *         当当前 tick 到达或超过 *next_ms 时，返回 1，
  *         并自动把 *next_ms 推进到下一次触发时间。
  *
  * @note   do-while 的作用：
  *         如果主循环被阻塞，导致错过了多个周期，
  *         本函数不会连续返回多次补执行旧任务，
  *         而是把 next_ms 一直推进到 now_ms 之后，
  *         保证后续周期重新对齐。
  *
  * @note   使用 time_after() 比直接比较大小更安全，
  *         可以处理 16bit 毫秒计数溢出回绕的情况。
  */
bit timer_period_elapsed(u16 *next_ms, u16 period_ms)
{
    u16 now_ms;

    /*
     * 参数保护：
     * 1. next_ms 为空，无法更新下一次触发时间。
     * 2. period_ms 为 0，周期无意义。
     */
    if((next_ms == 0) || (period_ms == 0U))
    {
        return 0;
    }

    /* 读取当前系统毫秒 tick。 */
    now_ms = timer_get_tick_ms();

    /*
     * 当前时间已经到达或超过计划触发时间。
     * now_ms == *next_ms：刚好到点。
     * time_after(now_ms, *next_ms)：当前时间已经超过触发点。
     */
    if((now_ms == *next_ms) || time_after(now_ms, *next_ms))
    {
        /*
         * 将下一次触发时间向后推进。
         *
         * 如果主循环延迟较大，可能已经错过多个周期。
         * 这里连续推进，直到 *next_ms 落在 now_ms 之后。
         *
         * 这样本函数本次只返回 1 次，不会为了补偿旧周期而连续触发多次。
         */
        do
        {
            *next_ms = (u16)(*next_ms + period_ms);
        } while((now_ms == *next_ms) || time_after(now_ms, *next_ms));

        return 1;
    }

    /* 还没到下一次触发时间。 */
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