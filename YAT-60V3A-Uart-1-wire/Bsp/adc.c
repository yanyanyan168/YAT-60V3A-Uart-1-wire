/**
  ******************************************************************************
  * @file    adc.c
  * @brief   ADC 初始化、滤波采样和校准换算。
  ******************************************************************************
  */
#include "adc.h"
#include "cal.h"
#include "gpio.h"
#include "timer.h"
#include "ntc.h"

xdata ADC_Type adc;
xdata VAL_Type val;

void adc_init(void)
{
    NTC_ADC_ANALOG();
    BATT_ADC_ANALOG();
    CURR_ADC_ANALOG();

    ADC_CFG1 &= ~(0x7U << 3);
    ADC_CFG1 |= ADC_CLK_SET(5);
    ADC_CFG2 = ADC_CHAN0_SMP_TIME(5);

    ADC_ACON1 &= ~(ADC_VREF_SEL(0x7) | ADC_EXREF_SEL(0x1));
    ADC_ACON1 |= ADC_VREF_SEL(0x6) |
                 ADC_TEN_SEL(0x3) |
                 ADC_INREF_SEL(0x0) |
                 ADC_EXREF_SEL(0x0);

    ADC_ACON0 = ADC_CMP_EN(0x1) |
                ADC_BIAS_EN(0x1) |
                ADC_BIAS_SEL(0x1);

    ADC_CFG0 |= ADC_CHAN0_EN(0x1) | ADC_EN(0x1);
    delay_ms(1);
}

u16 adc_read_raw(ADC_Channel channel)
{
    u8 i;
    u16 raw;
    u16 max_raw;
    u16 min_raw;
    u32 sum;

    sum = 0UL;
    max_raw = 0U;
    min_raw = 0x0FFFU;
    ADC_CHS0 = channel;

    for(i = 0U; i < ADC_SAMPLE_CNT; i++)
    {
        ADC_CFG0 |= ADC_CHAN0_TRG(0x1);
        while((ADC_STA & ADC_CHAN0_DONE(0x1)) == 0U)
        {
        }
        ADC_STA = ADC_CHAN0_DONE(0x1);

        raw = ((u16)ADC_DATAH0 << 4) | ((u16)ADC_DATAL0 >> 4);
        sum += raw;

        if(raw > max_raw)
        {
            max_raw = raw;
        }
        if(raw < min_raw)
        {
            min_raw = raw;
        }
    }

    sum -= max_raw;
    sum -= min_raw;
    return (u16)(sum / (ADC_SAMPLE_CNT - ADC_DROP_CNT));
}

u16 Get_ADC_Channel(ADC_Channel channel)
{
    return adc_read_raw(channel);
}

static u16 adc_line_calc(u16 raw, u16 adc_low, u16 adc_high, u16 val_low, u16 val_high)
{
    u32 num;
    u32 den;
    u32 out;

    if(adc_high <= adc_low)
    {
        return val_low;
    }

    den = (u32)adc_high - adc_low;
    num = (u32)(raw - adc_low);

    if(val_high >= val_low)
    {
        out = (u32)(val_high - val_low) * num / den;
        out += val_low;
    }
    else
    {
        out = (u32)(val_low - val_high) * num / den;
        if(out >= val_low)
        {
            out = 0UL;
        }
        else
        {
            out = (u32)val_low - out;
        }
    }

    if(out > 65535UL)
    {
        out = 65535UL;
    }

    return (u16)out;
}

u16 cal_Vout(void)
{
    u16 raw;
    u32 mv;

    raw = (u16)adc.vout;

    if(raw > cal_val.Vo_V2_adc)
    {
        return adc_line_calc(raw, cal_val.Vo_V2_adc, cal_val.Vo_V1_adc, cal_val.V2_val, cal_val.V1_val);
    }
    if(raw > cal_val.Vo_V3_adc)
    {
        return adc_line_calc(raw, cal_val.Vo_V3_adc, cal_val.Vo_V2_adc, cal_val.V3_val, cal_val.V2_val);
    }
    if(raw > cal_val.Vo_V4_adc)
    {
        return adc_line_calc(raw, cal_val.Vo_V4_adc, cal_val.Vo_V3_adc, cal_val.V4_val, cal_val.V3_val);
    }
    if(cal_val.Vo_V4_adc == 0U)
    {
        return 0U;
    }

    mv = (u32)cal_val.V4_val * raw / cal_val.Vo_V4_adc;
    if(mv > 65535UL)
    {
        mv = 65535UL;
    }
    return (u16)mv;
}

u16 cal_Curr(void)
{
    u32 raw_curr;
    u32 ma;

    raw_curr = (u16)adc.curr;
    if(raw_curr < cal_val.I4_adc)
    {
        raw_curr = cal_val.I4_adc;
    }
    raw_curr -= cal_val.I4_adc;

    if(raw_curr > cal_val.I2_adc)
    {
        return adc_line_calc((u16)raw_curr, cal_val.I2_adc, cal_val.I1_adc, cal_val.I2_val, cal_val.I1_val);
    }
    if(raw_curr > cal_val.I3_adc)
    {
        return adc_line_calc((u16)raw_curr, cal_val.I3_adc, cal_val.I2_adc, cal_val.I3_val, cal_val.I2_val);
    }
    if(cal_val.I3_adc == 0U)
    {
        return 0U;
    }

    ma = (u32)cal_val.I3_val * raw_curr / cal_val.I3_adc;
    if(ma > 65535UL)
    {
        ma = 65535UL;
    }
    return (u16)ma;
}

u16 adc_ntc_raw_to_res10(u16 raw)
{
    if(raw >= (ADC_FULL - 1U))
    {
        raw = (u16)(ADC_FULL - 1U);
    }

    return CH_D2R(raw);
}

void adc_sample_all(void)
{
    u16 ntc_res10;

    adc.vout = (s16)Get_ADC_Channel(AN_VOUT);
    val.vout = cal_Vout();

    adc.curr = (s16)Get_ADC_Channel(AN_CURR);
    val.curr = cal_Curr();

    adc.vdc = adc.vout;
    val.vdc = val.vout;

    adc.i_ntc = Get_ADC_Channel(AN_I_NTC);
    ntc_res10 = adc_ntc_raw_to_res10(adc.i_ntc);
    val.i_ntc = CH_R2T(ntc_res10);
}

void get_adc(void)
{
    adc_sample_all();
}