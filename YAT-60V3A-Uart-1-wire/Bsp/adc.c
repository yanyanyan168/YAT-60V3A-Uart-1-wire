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
    ADC_CFG1 |= ADC_CLK_SET(5);    // 分频比为 n+1，6 分频
    
    ADC_CFG2 = ADC_CHAN0_SMP_TIME(5); // 通道 0 采样时间配置，配置比为 n+1 时钟

    ADC_ACON1 &= ~(ADC_VREF_SEL(0x7) | ADC_EXREF_SEL(0x1));   // 关闭外部参考电压 ADCSELEXREF=0; (默认:ADCSELINREF=1;)
    ADC_ACON1 |= ADC_VREF_SEL(0x6) |                          // 0x0:保留  0x1:2.0V  0x2:2.4V  0x3:3.0V  0x4:3.6V  0x5:4.2V  0x6:VCCA  0x7:保留位
                 ADC_TEN_SEL(0x3)  |                          // ADC测试信号选择  0x0:测试信号  0x1:保留  0x2:保留  0x3:关闭测试信号
                 ADC_INREF_SEL(0)  |                          // ADC中内部参考能使信号  0x0:关闭  0x1:打开
                 ADC_EXREF_SEL(0);                            // ADC外部参考选择信号  0x0:不选择外部参考  0x1:选择EXREF(P07)为参考电压
    
    ADC_ACON0  = ADC_CMP_EN(0x1)  |                           // 打开ADC中的CMP使能信号
                 ADC_BIAS_EN(0x1) |                           // 打开ADC偏置电流能使信号
                 ADC_BIAS_SEL(0x1);
    
    ADC_CFG0  |= ADC_CHAN0_EN(0x1) |                          // 使能通道0转换
                 ADC_EN(0x1);                                 // 使能A/D转换
    delay_ms(1);                                              // 等待ADC模块配置稳定，需要等待20us以上
}



u16 Get_ADC_Channel(ADC_Channel channel)
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



/******************************************************************
函数名：adc_line_calc
功能：  两点线性换算

说明：
        已确认本项目中 ADC 越大，实际值越大，所以这里只处理上升曲线。

        换算公式：
        out = val_low + (val_high - val_low) * (raw - adc_low)
                         / (adc_high - adc_low)

输入：
        raw       当前 ADC 值
        adc_low   低标定点 ADC 值
        adc_high  高标定点 ADC 值
        val_low   低标定点实际值
        val_high  高标定点实际值

返回：
        换算后的实际值，最大限制为 65535

注意：
        1. adc_high 必须大于 adc_low，否则直接返回 val_low
        2. raw 小于等于 adc_low 时，直接返回 val_low
        3. 本函数适合 ADC 越大、实际值越大的场景
******************************************************************/
static u16 adc_line_calc(u16 raw,
                         u16 adc_low,
                         u16 adc_high,
                         u16 val_low,
                         u16 val_high)
{
    u32 out;
    u16 adc_diff;
    u16 raw_diff;
    u16 val_diff;

    /* 标定 ADC 异常，避免除 0 */
    if(adc_high <= adc_low)
    {
        return val_low;
    }

    /* 当前 ADC 低于低标定点，直接返回低点值 */
    if(raw <= adc_low)
    {
        return val_low;
    }

    adc_diff = adc_high - adc_low;
    raw_diff = raw - adc_low;
    val_diff = val_high - val_low;

    /*
     * 只在乘法结果处使用 u32。
     * 对 51 来说，比全程 u32 运算更省一些。
     */
    out = (u32)val_diff * raw_diff;
    out = out / adc_diff;
    out += val_low;

    /* 超过 u16 最大值时限幅 */
    if(out > 65535UL)
    {
        out = 65535UL;
    }

    return (u16)out;
}


/******************************************************************
函数名：cal_Vout
功能：  输出电压 ADC 换算

说明：
        使用两段线性换算：

        低段：
            0 ADC -> 0 mV
            Vo_V2_adc -> V2_val

        高段：
            Vo_V2_adc -> V2_val
            Vo_V1_adc -> V1_val

返回：
        输出电压，单位按原工程定义，一般为 mV
******************************************************************/
u16 cal_Vout(void)
{
    u16 raw;

    raw = (u16)adc.vout;

    /*
     * 高于 V2 标定点，使用 V2 -> V1 两点线性换算。
     */
    if(raw > cal_val.Vo_V2_adc)
    {
        return adc_line_calc(raw,
                             cal_val.Vo_V2_adc,
                             cal_val.Vo_V1_adc,
                             cal_val.V2_val,
                             cal_val.V1_val);
    }

    /*
     * 低于或等于 V2 标定点，使用 0 -> V2 线性换算。
     * 如果 Vo_V2_adc 为 0，adc_line_calc 内部会直接返回 0。
     */
    return adc_line_calc(raw,
                         0U,
                         cal_val.Vo_V2_adc,
                         0U,
                         cal_val.V2_val);
}


/******************************************************************
函数名：cal_Curr
功能：  输出电流 ADC 换算

说明：
        电流 ADC 先扣除零点偏移 I4_adc。

        换算分三段：

        低段：
            0 ADC -> 0 mA
            I3_adc -> I3_val

        中段：
            I3_adc -> I3_val
            I2_adc -> I2_val

        高段：
            I2_adc -> I2_val
            I1_adc -> I1_val

返回：
        输出电流，单位按原工程定义，一般为 mA
******************************************************************/
u16 cal_Curr(void)
{
    u16 raw;

    raw = (u16)adc.curr;

    /*
     * 扣除电流零点偏移。
     * adc.curr 小于等于 I4_adc 时，认为电流为 0。
     */
    if(raw <= cal_val.I4_adc)
    {
        raw = 0U;
    }
    else
    {
        raw -= cal_val.I4_adc;
    }

    /*
     * 高电流段：
     * I2_adc -> I2_val
     * I1_adc -> I1_val
     */
    if(raw > cal_val.I2_adc)
    {
        return adc_line_calc(raw,
                             cal_val.I2_adc,
                             cal_val.I1_adc,
                             cal_val.I2_val,
                             cal_val.I1_val);
    }

    /*
     * 中电流段：
     * I3_adc -> I3_val
     * I2_adc -> I2_val
     */
    if(raw > cal_val.I3_adc)
    {
        return adc_line_calc(raw,
                             cal_val.I3_adc,
                             cal_val.I2_adc,
                             cal_val.I3_val,
                             cal_val.I2_val);
    }

    /*
     * 低电流段：
     * 0 ADC -> 0 mA
     * I3_adc -> I3_val
     *
     * 如果 I3_adc 为 0，adc_line_calc 内部会直接返回 0。
     */
    return adc_line_calc(raw,
                         0U,
                         cal_val.I3_adc,
                         0U,
                         cal_val.I3_val);
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

    adc.i_ntc = Get_ADC_Channel(AN_I_NTC);
    ntc_res10 = adc_ntc_raw_to_res10(adc.i_ntc);
    val.i_ntc = CH_R2T(ntc_res10);
}
