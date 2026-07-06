/**
  ******************************************************************************
  * @file    cal.h
  * @brief   DEBUG 校准框架接口，命令字保持 54.6V 项目兼容。
  ******************************************************************************
  */
#ifndef __CAL_H__
#define __CAL_H__

#include "common.h"

typedef struct
{
    u16 flag;

    u16 I4_adc;

    u16 Vo_V1_adc;
    u16 V1_val;

    u16 I1_adc;
    u16 I1_val;

    u16 I2_adc;
    u16 I2_val;

    u16 I3_adc;
    u16 I3_val;

    u16 Vo_V2_adc;
    u16 V2_val;

    u16 cc_pwm;
    u16 cc_val;
} CAL_VAL_Types;

extern xdata CAL_VAL_Types cal_val;

void calVal_Init(void);
void read_cal_data(void);
void save_cal_data(void);
void usr_cal_func(void);

u16 cal_current_to_duty(u16 current_ma);
u16 cal_Vout(void);
u16 cal_Curr(void);

#define SET_CURR(x_ma)                     cal_current_to_duty((u16)(x_ma))

#endif
