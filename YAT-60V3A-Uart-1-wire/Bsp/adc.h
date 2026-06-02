/**
  ******************************************************************************
  * @file    adc.h
  * @brief   ADC 初始化、滤波采样和标定换算接口。
  ******************************************************************************
  */
#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "common.h"

/* ADC 外部通道枚举，参考 YAT 项目 adc.h，与 SDK ADC_CHS0 选择值保持一致。 */
typedef enum
{
    ADC_Channel_P00,
    ADC_Channel_P01,
    ADC_Channel_P02,
    ADC_Channel_P03,
    ADC_Channel_P04,
    ADC_Channel_P05,
    ADC_Channel_P06,
    ADC_Channel_P07,
    ADC_Channel_P10,
    ADC_Channel_P11,
    ADC_Channel_P12,
    ADC_Channel_P13,
    ADC_Channel_P14,
    ADC_Channel_P15,
    ADC_Channel_P16,
    ADC_Channel_P17,
    ADC_Channel_P20,
    ADC_Channel_P21,
    ADC_Channel_P22,
    ADC_Channel_P23,
    ADC_Channel_P24,
    ADC_Channel_P25,
    ADC_Channel_P26,
    ADC_Channel_P27,
    ADC_Channel_P30,
    ADC_Channel_P31,
    ADC_Channel_R0       = 0 | ADC_EXT_SEL(0x1),
    ADC_Channel_VREF_0P6 = 1 | ADC_EXT_SEL(0x1),
    ADC_Channel_R1       = 2 | ADC_EXT_SEL(0x1),
    ADC_Channel_VCCA_D5  = 3 | ADC_EXT_SEL(0x1),
    ADC_Channel_AMP0     = 4 | ADC_EXT_SEL(0x1),
    ADC_Channel_AMP1     = 5 | ADC_EXT_SEL(0x1),
    ADC_Channel_AMP2     = 6 | ADC_EXT_SEL(0x1),
    ADC_Channel_XX       = 7 | ADC_EXT_SEL(0x1),
} ADC_Channel;

/* 原理图对应关系：P10/AIN8 电流，P03/AIN3 电池电压，P05/AIN1 NTC。 */
#define AN_CURR                            ADC_Channel_P10
#define AN_VOUT                            ADC_Channel_P03
#define AN_I_NTC                           ADC_Channel_P05

typedef struct
{
    s16 vout;              /* 电池电压采样原始 ADC，AIN3/P03 */
    s16 vdc;               /* 兼容 54.6V 字段，本项目镜像 vout */
    s16 curr;              /* 电流采样原始 ADC，AIN8/P10 */
    u16 i_ntc;             /* NTC 原始 ADC，AIN1/P05 */
} ADC_Type;

typedef struct
{
    u16 vout;              /* 电池电压，单位 mV */
    u16 vdc;               /* 兼容 54.6V 字段，本项目镜像 vout，单位 mV */
    u16 curr;              /* 充电电流，单位 mA */
    s32 i_ntc;             /* NTC 温度，沿用 54.6V NTC 表返回单位 */
} VAL_Type;

extern xdata ADC_Type adc;
extern xdata VAL_Type val;

void adc_init(void);
#define ADC_Init()                        adc_init()
u16 Get_ADC_Channel(ADC_Channel channel);
u16 adc_read_raw(ADC_Channel channel);
void adc_sample_all(void);
void get_adc(void);

u16 cal_Vout(void);
u16 cal_Curr(void);
u16 adc_ntc_raw_to_res10(u16 raw);

#endif
