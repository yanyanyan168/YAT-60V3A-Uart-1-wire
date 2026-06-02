/**
  ******************************************************************************
  * @file    pwm.c
  * @brief   STMR0 PWM ??????????
  ******************************************************************************
  */
#include "pwm.h"
#include "gpio.h"
static u16 idata s_curr_duty;
static u16 idata s_vol_duty_shadow;
void pwm_init(void)
{
    CURR_REF_PWM_OUTPUT();
    FOUT_S22 = 0U;
    FOUT_S16 = GPIO_FOUT_STMR0_PWMOUT;
    STMR0_PSC = STMR_PRESCALE_VAL(0x00);
    SET_PERIOD(0, PWMMAX);
    SET_DUTY(0, PWM_DUTY_DEF);
    STMR_PWMVALA &= ~STMR_0_PWMVALA(0x1);
    STMR_PWMEN |= STMR_0_PWM_EN(0x1);
    STMR_CNTMD |= STMR_0_CNT_MODE(0x1);
    STMR_LOADEN |= STMR_0_LOAD_EN(0x1);
    STMR_CNTCLR |= STMR_0_CNT_CLR(0x1);
    STMR_CNTEN |= STMR_0_CNT_EN(0x1);
    s_curr_duty = 0U;
    s_vol_duty_shadow = 0U;
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
void set_Vol_Duty(u16 duty)
{
    if(duty > PWMMAX)
    {
        duty = PWMMAX;
    }
    s_vol_duty_shadow = duty;
}
u16 get_Curr_Duty(void)
{
    return s_curr_duty;
}
u16 get_Vol_Duty(void)
{
    return s_vol_duty_shadow;
}
void pwm_set_current_ref_ma(u16 current_ma)
{
    u32 duty;
    if(current_ma > iMAX)
    {
        current_ma = iMAX;
    }
    duty = (u32)current_ma * PWMMAX / iMAX;
    set_Curr_Duty((u16)duty);
}