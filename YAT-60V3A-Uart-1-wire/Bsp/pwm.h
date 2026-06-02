/**
  ******************************************************************************
  * @file    pwm.h
  * @brief   P16 STMR0 PWM DAC interface.
  ******************************************************************************
  */
#ifndef __BSP_PWM_H__
#define __BSP_PWM_H__
#include "common.h"
#define STMR0_PERIOD_VAL                    (SYS_CLK_HZ / PWM_FREQ_HZ - 1UL)
#define PWMMAX                              ((u16)STMR0_PERIOD_VAL)
#define SET_PERIOD(ch, value)               do{STMR##ch##_PRH=STMR_PRD_VAL_H(((value)>>8)&0xFF);STMR##ch##_PRL=STMR_PRD_VAL_L((value)&0xFF);}while(0)
#define SET_DUTY(ch, value)                 do{STMR##ch##_CMPAH=STMR_CMPA_VAL_H(((value)>>8)&0xFF);STMR##ch##_CMPAL=STMR_CMPA_VAL_L((value)&0xFF);STMR_LOADEN|=STMR_0_LOAD_EN(0x1);}while(0)
void pwm_init(void);
#define PWM_Init()                        pwm_init()
void set_Curr_Duty(u16 duty);
void set_Vol_Duty(u16 duty);
u16 get_Curr_Duty(void);
u16 get_Vol_Duty(void);
void pwm_set_current_ref_ma(u16 current_ma);
#endif