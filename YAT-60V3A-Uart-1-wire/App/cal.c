/**
  ******************************************************************************
  * @file    cal.c
  * @brief   DEBUG 校准框架。
  *
  * 说明：
  * 1. 保留 54.6V 项目的校准命令字、CRC32 帧格式和 DEBUG FIFO 入口。
  * 2. 当前阶段只建立框架和默认参数读写，不展开完整产线校准业务。
  ******************************************************************************
  */
#include "cal.h"
#include "bsp_init.h"
#include "adc.h"
#include "fifo.h"
#include "flash.h"
#include "gpio.h"
#include "pwm.h"
#include "timer.h"
#include "uart.h"
#include "ch.h"
#include "stm32_crc.h"
#include "common.h"

#define CAL_FLAG_V2                         CAL_FLAG

xdata CAL_VAL_Types cal_val;

typedef struct
{
    u16 voltage_mv;
    u16 current_ma;
} cal_load_t;

static cal_load_t idata s_load_val;
static u8 idata s_cal_v_step;
static u8 idata s_cc_confirm_count;
static u16 idata s_cal_pwm_val;
static u8 xdata s_cal_tx_buf[16];
static u8 xdata s_cal_rx_buf[CAL_FRAME_LEN];

static u16 cal_adc_from_voltage_mv(u16 voltage_mv)
{
    u32 tmp;
    u32 den;

    tmp = (u32)voltage_mv * R2 * ADC_FULL;
    den = (u32)ADC_VREF_MV *
          (R1 + R2);

    if(den == 0UL)
    {
        return 0;
    }

    tmp = (tmp + (den >> 1)) / den;
    if(tmp > ADC_FULL)
    {
        tmp = ADC_FULL;
    }

    return (u16)tmp;
}

static u16 cal_adc_from_current_ma(u16 current_ma)
{
    u32 tmp;
    u32 den;

    tmp = (u32)current_ma * Ra *
          GAIN * ADC_FULL;
    den = (u32)ADC_VREF_MV * 1000UL;

    if(den == 0UL)
    {
        return 0;
    }

    tmp = (tmp + (den >> 1)) / den;
    if(tmp > ADC_FULL)
    {
        tmp = ADC_FULL;
    }

    return (u16)tmp;
}

void calVal_Init(void)
{
    cal_val.flag = CAL_FLAG_V2;
    cal_val.I4_adc = 0;

    cal_val.I1_val = iMAX;
    cal_val.I1_adc = cal_adc_from_current_ma(cal_val.I1_val);
    cal_val.I2_val = (u16)((u32)iMAX * 3UL / 4UL);
    cal_val.I2_adc = cal_adc_from_current_ma(cal_val.I2_val);
    cal_val.I3_val = iPRE;
    cal_val.I3_adc = cal_adc_from_current_ma(cal_val.I3_val);

    cal_val.V1_val = SET_vMAX;
    cal_val.V2_val = (u16)((u32)SET_vMAX * 15UL / 16UL);
    cal_val.V3_val = (u16)((u32)SET_vMAX * 14UL / 16UL);
    cal_val.V4_val = (u16)((u32)SET_vMAX * 13UL / 16UL);

    cal_val.Vo_V1_adc = cal_adc_from_voltage_mv(cal_val.V1_val);
    cal_val.Vdc_V1_adc = cal_val.Vo_V1_adc;
    cal_val.Vo_V2_adc = cal_adc_from_voltage_mv(cal_val.V2_val);
    cal_val.Vdc_V2_adc = cal_val.Vo_V2_adc;
    cal_val.Vo_V3_adc = cal_adc_from_voltage_mv(cal_val.V3_val);
    cal_val.Vdc_V3_adc = cal_val.Vo_V3_adc;
    cal_val.Vo_V4_adc = cal_adc_from_voltage_mv(cal_val.V4_val);
    cal_val.Vdc_V4_adc = cal_val.Vo_V4_adc;

    cal_val.cv_pwm = PWMMAX;
    cal_val.cv_val = SET_vMAX;
    cal_val.cc_pwm = PWMMAX;
    cal_val.cc_val = iMAX;
}

u16 cal_current_to_duty(u16 current_ma)
{
    u32 duty;

    if((cal_val.cc_val == 0U) || (cal_val.cc_pwm == 0U))
    {
        return 0;
    }

    duty = (u32)current_ma * cal_val.cc_pwm / cal_val.cc_val;
    if(duty > PWMMAX)
    {
        duty = PWMMAX;
    }

    return (u16)duty;
}

u16 cal_voltage_to_duty(u16 voltage_mv)
{
    u32 duty;

    if((cal_val.cv_val == 0U) || (cal_val.cv_pwm == 0U))
    {
        return 0;
    }

    duty = (u32)voltage_mv * cal_val.cv_pwm / cal_val.cv_val;
    if(duty > PWMMAX)
    {
        duty = PWMMAX;
    }

    return (u16)duty;
}

void save_cal_data(void)
{
    cal_val.flag = CAL_FLAG_V2;
    flash_ferase_sector(FLASH_CAL_ADDR);
    flash_program(FLASH_CAL_ADDR, (u8 *)&cal_val, (u8)sizeof(cal_val));
}

void read_cal_data(void)
{
    flash_fread(FLASH_CAL_ADDR, (u8 *)&cal_val, sizeof(cal_val));
    if(cal_val.flag != CAL_FLAG_V2)
    {
        calVal_Init();
    }
}

static void cal_tx_str_crc(char *str)
{
    u8 len;
    u32 crc;

    len = 0;
    while((str[len] != '\0') && (len < 12U))
    {
        s_cal_tx_buf[len] = (u8)str[len];
        len++;
    }

    crc = CRC32(s_cal_tx_buf, len);
    s_cal_tx_buf[len + 0U] = (u8)(crc);
    s_cal_tx_buf[len + 1U] = (u8)(crc >> 8);
    s_cal_tx_buf[len + 2U] = (u8)(crc >> 16);
    s_cal_tx_buf[len + 3U] = (u8)(crc >> 24);
    ch_uart_send_buf((char *)s_cal_tx_buf, (u8)(len + 4U));
}

static bit cal_frame_crc_ok(u8 *rx_buf, u8 len)
{
    union
    {
        u32 d32;
        u8 dat[4];
    } rx_crc;
    u32 calc_crc;

    if(len < 5U)
    {
        return 0;
    }

    calc_crc = CRC32(rx_buf, (u16)(len - 4U));
    rx_crc.dat[3] = rx_buf[len - 4U];
    rx_crc.dat[2] = rx_buf[len - 3U];
    rx_crc.dat[1] = rx_buf[len - 2U];
    rx_crc.dat[0] = rx_buf[len - 1U];

    return (rx_crc.d32 == calc_crc) ? 1 : 0;
}

static void cal_parse_load_value(u8 *rx_buf, u8 len)
{
    if(len >= 13U)
    {
        s_load_val.voltage_mv  = (u16)rx_buf[1];
        s_load_val.voltage_mv |= (u16)rx_buf[2] << 8;
        s_load_val.current_ma  = (u16)rx_buf[5];
        s_load_val.current_ma |= (u16)rx_buf[6] << 8;
    }
}

static u16 cal_abs_diff_u16(u16 a, u16 b)
{
    return (a >= b) ? (u16)(a - b) : (u16)(b - a);
}

static u16 cal_pwm_limit(u32 duty)
{
    if(duty > PWMMAX)
    {
        duty = PWMMAX;
    }

    return (u16)duty;
}

static u16 cal_scale_pwm(u16 base, u16 num)
{
    return cal_pwm_limit((u32)base * num / 16U);
}

static u16 cal_voltage_pwm_step(u16 diff_mv)
{
    u16 step;

    step = (u16)(diff_mv / 100U);
    if(step == 0U)
    {
        step = 1U;
    }

    return step;
}

static u16 cal_curr_adc_offset(void)
{
    if(adc.curr >= cal_val.I4_adc)
    {
        return (u16)(adc.curr - cal_val.I4_adc);
    }

    return 0U;
}

static void cal_exec_task(u8 task)
{
    char echo_ok[4];
    u16 diff_mv;
    u16 duty_step;

    switch(task)
    {
    case '*':
        cal_tx_str_crc("*OK");
        delay_ms(10);
        SET_BAUD(BAUD_9600);
        s_cal_v_step = 0U;
        s_cc_confirm_count = 0U;
        s_cal_pwm_val = cal_val.cv_pwm;
        cal_val.I4_adc = adc.curr;
        set_Curr_Duty(PWMMAX);
        set_Vol_Duty(s_cal_pwm_val);
        break;

    case '#':
        switch(s_cal_v_step)
        {
        default:
        case 0:
            diff_mv = cal_abs_diff_u16(s_load_val.voltage_mv, SET_vMAX);
            if((s_load_val.voltage_mv != 0U) && (diff_mv > 300U))
            {
                duty_step = cal_voltage_pwm_step(diff_mv);
                if(s_load_val.voltage_mv < SET_vMAX)
                {
                    s_cal_pwm_val = cal_pwm_limit((u32)s_cal_pwm_val + duty_step);
                }
                else
                {
                    s_cal_pwm_val = (s_cal_pwm_val > duty_step) ?
                                    (u16)(s_cal_pwm_val - duty_step) : 0U;
                }
                set_Vol_Duty(s_cal_pwm_val);
            }
            else
            {
                cal_val.Vo_V1_adc = adc.vout;
                cal_val.Vdc_V1_adc = cal_val.Vo_V1_adc;   /* 本项目无独立 VDC，保留字段镜像 Vo。 */
                if(s_load_val.voltage_mv != 0U)
                {
                    cal_val.V1_val = s_load_val.voltage_mv;
                    cal_val.cv_val = s_load_val.voltage_mv;
                }
                cal_val.cv_pwm = s_cal_pwm_val;
                set_Vol_Duty(cal_scale_pwm(s_cal_pwm_val, 15U));
                set_Curr_Duty(PWMMAX);
                s_cal_v_step = 1U;
            }
            cal_tx_str_crc("#ADJ");
            break;

        case 1:
            cal_val.Vo_V2_adc = adc.vout;
            cal_val.Vdc_V2_adc = cal_val.Vo_V2_adc;
            if(s_load_val.voltage_mv != 0U)
            {
                cal_val.V2_val = s_load_val.voltage_mv;
            }
            set_Vol_Duty(cal_scale_pwm(s_cal_pwm_val, 13U));
            s_cal_v_step = 2U;
            cal_tx_str_crc("#ADJ");
            break;

        case 2:
            cal_val.Vo_V3_adc = adc.vout;
            cal_val.Vdc_V3_adc = cal_val.Vo_V3_adc;
            if(s_load_val.voltage_mv != 0U)
            {
                cal_val.V3_val = s_load_val.voltage_mv;
            }
            set_Vol_Duty(cal_scale_pwm(s_cal_pwm_val, 11U));
            s_cal_v_step = 3U;
            cal_tx_str_crc("#ADJ");
            break;

        case 3:
            cal_val.I3_adc = cal_curr_adc_offset();
            if(s_load_val.current_ma != 0U)
            {
                cal_val.I3_val = s_load_val.current_ma;
            }
            cal_val.Vo_V4_adc = adc.vout;
            cal_val.Vdc_V4_adc = cal_val.Vo_V4_adc;
            if(s_load_val.voltage_mv != 0U)
            {
                cal_val.V4_val = s_load_val.voltage_mv;
            }
            set_Vol_Duty(cal_val.cv_pwm);
            s_cal_v_step = 0U;
            cal_tx_str_crc("#OK");
            break;
        }
        break;

    case '$':
        cal_val.I1_adc = cal_curr_adc_offset();
        if(s_load_val.current_ma != 0U)
        {
            cal_val.I1_val = s_load_val.current_ma;
            cal_val.cc_val = s_load_val.current_ma;
        }
        cal_tx_str_crc("$OK");
        break;

    case '!':
        cal_val.I2_adc = cal_curr_adc_offset();
        if(s_load_val.current_ma != 0U)
        {
            cal_val.I2_val = s_load_val.current_ma;
        }
        set_Curr_Duty(cal_val.cc_pwm);
        s_cc_confirm_count = 0U;
        cal_tx_str_crc("!OK");
        break;

    case '?':
        if(s_cc_confirm_count < 2U)
        {
            s_cc_confirm_count++;
        }
        if(s_cc_confirm_count >= 2U)
        {
            cal_val.cc_pwm = get_Curr_Duty();
            if(s_load_val.current_ma != 0U)
            {
                cal_val.cc_val = s_load_val.current_ma;
            }
            cal_tx_str_crc("?OK");
        }
        break;

    case '@':
        cal_tx_str_crc("@OK");
        break;

    case 'S':
        save_cal_data();
        set_Curr_Duty(0);
        set_Vol_Duty(0);
        cal_tx_str_crc("SAVE");
        break;

    case 'e':
        cal_tx_str_crc("end");
        delay_ms(50);
        SET_BAUD(BAUD_115200);
        flg_cal_mode = 0;
        break;

    case '%':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        echo_ok[0] = (char)task;
        echo_ok[1] = 'O';
        echo_ok[2] = 'K';
        echo_ok[3] = '\0';
        cal_tx_str_crc(echo_ok);
        break;

    default:
        cal_tx_str_crc("ERROR");
        break;
    }
}

void usr_cal_func(void)
{
    u8 len;
    u8 i;
    u8 task;
    u16 next_10ms;

    relay_on();
    repair_output_off();
    vadj_high();
    fan_on();
    s_cal_v_step = 0U;
    s_cc_confirm_count = 0U;
    s_cal_pwm_val = cal_val.cv_pwm;
    set_Curr_Duty(PWMMAX);
    set_Vol_Duty(s_cal_pwm_val);
    next_10ms = timer_deadline_ms(TASK_10MS);

    while(flg_cal_mode != 0)
    {
        len = s_ch_fifo.cnt;
        if((len == data_len_bk)&&(len != 0))
        {
            if(len > CAL_FRAME_LEN)
            {
                len = CAL_FRAME_LEN;
            }

            for(i = 0; i < len; i++)
            {
                s_cal_rx_buf[i] = ch_fifo_pop();
            }

            if(cal_frame_crc_ok(s_cal_rx_buf, len) != 0)
            {
                task = s_cal_rx_buf[0];
                cal_parse_load_value(s_cal_rx_buf, len);
                cal_exec_task(task);
            }
            else
            {
                cal_tx_str_crc("ERROR");
            }
        }
        
        data_len_bk = len;

        if(timer_period_elapsed(&next_10ms, TASK_10MS) != 0)
        {
            flg_10ms = 0;
            wdt_feed();
            adc_sample_all();
            Ged_Flash(50);
        }
    }

    gpio_output_safe_off();
}
