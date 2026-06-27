/**
  ******************************************************************************
  * @file    pwm.c
  * @brief   STMR0 PWM ??????????
  ******************************************************************************
  */
#include "pwm.h"
#include "gpio.h"
static u16 idata s_curr_duty;


void pwm_init(void)
{
    /* 配置电流基准 PWM 输出引脚 */
    CURR_REF_PWM_OUTPUT();

    /* 关闭 S22 复用功能，避免与 PWM 输出冲突 */
    FOUT_S22 = 0U;

    /* 将 S16 复用为 STMR0 PWM 输出 */
    FOUT_S16 = GPIO_FOUT_STMR0_PWMOUT;

    /* STMR0 不分频，使用原始定时器时钟 */
    STMR0_PSC = STMR_PRESCALE_VAL(0x00);

    /* 设置 PWM 周期 */
    SET_PERIOD(0, PWMMAX);

    /* 设置 PWM 初始占空比 */
    SET_DUTY(0, PWMMAX/2);

    /* 清除 STMR0 PWMVALA 相关配置位 */
    STMR_PWMVALA &= ~STMR_0_PWMVALA(0x1);

    /* 使能 STMR0 PWM 输出 */
    STMR_PWMEN |= STMR_0_PWM_EN(0x1);

    /* 设置 STMR0 为计数模式 */
    STMR_CNTMD |= STMR_0_CNT_MODE(0x1);

    /* 使能 STMR0 装载更新 */
    STMR_LOADEN |= STMR_0_LOAD_EN(0x1);

    /* 清零 STMR0 计数器 */
    STMR_CNTCLR |= STMR_0_CNT_CLR(0x1);

    /* 启动 STMR0 计数 */
    STMR_CNTEN |= STMR_0_CNT_EN(0x1);

    /* 软件记录当前电流环 PWM 占空比 */
    s_curr_duty = 0U;

}

void set_Curr_Duty(u16 duty)
{
    if(duty > PWMMAX)
    {
        duty = PWMMAX;
    }
    s_curr_duty = duty;
    SET_DUTY(0, duty);
}


u16 get_Curr_Duty(void)
{
    return s_curr_duty;
}
