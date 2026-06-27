/**
  ******************************************************************************
  * @file    common.c
  * @brief   ?????????
  ******************************************************************************
  */
#include "common.h"
#include "adc.h"
#include "gpio.h"
FLAG_Types idata ch_flag;
TIMER_Types idata Tim;
void memclr(void *addr, u16 len)
{
    u8 *p;
    if(addr == 0)
    {
        return;
    }
    p = (u8 *)addr;
    while(len != 0U)
    {
        *p = 0U;
        p++;
        len--;
    }
}

u16 common_clamp_u16(u16 val, u16 min_val, u16 max_val)
{
    if(val < min_val)
    {
        return min_val;
    }
    if(val > max_val)
    {
        return max_val;
    }
    return val;
}

u8 common_sum_u8(u8 *buf, u8 len)
{
    u8 sum;
    sum = 0U;
    while((buf != 0) && (len != 0U))
    {
        sum = (u8)(sum + *buf);
        buf++;
        len--;
    }
    return sum;
}

static void get_ch_ntcErr(void)
{
    static u8 idata cut_on;
    static u8 idata cut_off;
    if(ch_flag.ch_ntcErr == 0U)
    {
        if((adc.i_ntc > 4050U) || (adc.i_ntc < 50U))
        {
            if(++cut_on >= 50U)
            {
                cut_on = 0U;
                ch_flag.ch_ntcErr = 1U;
            }
        }
        else
        {
            cut_on = 0U;
        }
    }
    else
    {
        if((adc.i_ntc < 3850U) && (adc.i_ntc > 150U))
        {
            if(++cut_off >= 50U)
            {
                cut_off = 0U;
                ch_flag.ch_ntcErr = 0U;
            }
        }
        else
        {
            cut_off = 0U;
        }
    }
}
static void get_ch_hotErr(void)
{
    static u8 idata cut_on;
    static u8 idata cut_off;
    if(ch_flag.ch_hotErr != 0U)
    {
        if(val.i_ntc < T_HOT_ERR_OK)
        {
            if(++cut_off >= 50U)
            {
                cut_off = 0U;
                ch_flag.ch_hotErr = 0U;
            }
        }
        else
        {
            cut_off = 0U;
        }
    }
    else
    {
        if(val.i_ntc > T_HOT_ERR)
        {
            if(++cut_on >= 50U)
            {
                cut_on = 0U;
                ch_flag.ch_hotErr = 1U;
            }
        }
        else
        {
            cut_on = 0U;
        }
    }
}
static void get_ch_hot(void)
{
    static u8 idata cut_on;
    static u8 idata cut_off;
    if(ch_flag.ch_hot != 0U)
    {
        if(val.i_ntc < T_CH_HOT_OK)
        {
            if(++cut_off >= 50U)
            {
                cut_off = 0U;
                ch_flag.ch_hot = 0U;
            }
        }
        else
        {
            cut_off = 0U;
        }
    }
    else
    {
        if(val.i_ntc > T_CH_HOT)
        {
            if(++cut_on >= 50U)
            {
                cut_on = 0U;
                ch_flag.ch_hot = 1U;
            }
        }
        else
        {
            cut_on = 0U;
        }
    }
}
static void get_chovp(void)
{
    static u8 idata cut_on;
    static u8 idata cut_off;
    if(ch_flag.ch_ovp == 0U)
    {
        if(val.vout >= vDCOVP)
        {
            if(++cut_on >= 50U)
            {
                cut_on = 0U;
                ch_flag.ch_ovp = 1U;
            }
        }
        else
        {
            cut_on = 0U;
        }
    }
    else
    {
        if(val.vout <= SET_vMAX)
        {
            if(++cut_off >= 200U)
            {
                cut_off = 0U;
                ch_flag.ch_ovp = 0U;
            }
        }
        else
        {
            cut_off = 0U;
        }
    }
}
static void get_chocp(void)
{
    static u8 idata cut_on;
    static u8 idata cut_off;
    if(ch_flag.ch_ocp == 0U)
    {
        if(val.curr >= iOCP)
        {
            if(++cut_on >= 50U)
            {
                cut_on = 0U;
                ch_flag.ch_ocp = 1U;
            }
        }
        else
        {
            cut_on = 0U;
        }
    }
    else
    {
        if(val.curr <= iOCP_OK)
        {
            if(++cut_off >= 50U)
            {
                cut_off = 0U;
                ch_flag.ch_ocp = 0U;
            }
        }
        else
        {
            cut_off = 0U;
        }
    }
}
void ch_err_ck(void)
{
    get_ch_ntcErr();
    get_ch_hotErr();
    get_ch_hot();
    get_chovp();
    get_chocp();
}
void TimCut(void)
{
    if(++Tim.ms >= 100U)
    {
        Tim.ms = 0U;
        if(++Tim.s >= 60U)
        {
            Tim.s = 0U;
            if(Tim.min < UINT16_MAX)
            {
                Tim.min++;
            }
        }
    }
}
void Red_Flash(u8 period_n10ms)
{
    static u8 idata cut_10ms;
    if(period_n10ms < 2U)
    {
        period_n10ms = 2U;
    }
    GLED = 0;
    if(++cut_10ms > (period_n10ms >> 1))
    {
        cut_10ms = 0U;
        RLED = !RLED;
    }
}
void Ged_Flash(u8 period_n10ms)
{
    static u8 idata cut_10ms;
    if(period_n10ms < 2U)
    {
        period_n10ms = 2U;
    }
    RLED = 0;
    if(++cut_10ms > (period_n10ms >> 1))
    {
        cut_10ms = 0U;
        GLED = !GLED;
    }
}
void RGed_Flash(u8 period_n10ms)
{
    static u8 idata cut_10ms;
    if(period_n10ms < 2U)
    {
        period_n10ms = 2U;
    }
    if(++cut_10ms > (period_n10ms >> 1))
    {
        cut_10ms = 0U;
        if(GLED != 0)
        {
            GLED = 0;
            RLED = 1;
        }
        else
        {
            GLED = 1;
            RLED = 0;
        }
    }
}