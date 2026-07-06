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
#define CAL_ADC_VOL(x_mv)                   \
    (u16)((((u32)(x_mv) * ADC_FULL) + ((ADC_VREF_MV * ((R1 + R2) / R2)) / 2UL)) / (ADC_VREF_MV * ((R1 + R2) / R2)))
#define CAL_ADC_CURR(x_ma)                  \
    (u16)(((((((u32)(x_ma) * Ra * GAIN) + 500UL) / 1000UL) * ADC_FULL) + (ADC_VREF_MV / 2UL)) / ADC_VREF_MV)

xdata CAL_VAL_Types cal_val;

extern u16 idata s_dummy_load_10ms;

typedef struct
{
    u16 voltage_mv;
    u16 current_ma;
} cal_load_t;

static cal_load_t idata s_load_val;
static u8 idata s_cal_v_step;
static u8 idata s_cc_confirm_count;
static u8 xdata s_cal_tx_buf[16];
static u8 xdata s_cal_rx_buf[CAL_FRAME_LEN];

void calVal_Init(void)
{
    cal_val.flag = CAL_FLAG_V2;
    cal_val.I4_adc = 0;

    cal_val.I1_val = iMAX;
    cal_val.I1_adc = CAL_ADC_CURR(iMAX);
    cal_val.I2_val = (u16)((u32)iMAX * 3UL / 4UL);
    cal_val.I2_adc = CAL_ADC_CURR((u16)((u32)iMAX * 3UL / 4UL));
    cal_val.I3_val = iPRE;
    cal_val.I3_adc = CAL_ADC_CURR(iPRE);

    cal_val.V1_val = SET_vMAX;
    cal_val.V2_val = (u16)((u32)SET_vMAX * 13UL / 16UL);

    cal_val.Vo_V1_adc = CAL_ADC_VOL(SET_vMAX);

    cal_val.Vo_V2_adc = CAL_ADC_VOL((u16)((u32)SET_vMAX * 13UL / 16UL));

    /* 无校准时按硬件链路预置：I * 采样电阻 * 放大倍数 / 5V。 */
    cal_val.cc_pwm = CC(iMAX);
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


static u16 cal_curr_adc_offset(void)
{
    if(adc.curr >= cal_val.I4_adc)
    {
        return (u16)(adc.curr - cal_val.I4_adc);
    }

    return 0U;
}


void usr_cal_func(void)
{
    u8 len;
    u8 i;
    u8 task;
//    char echo_ok[4];
    uint8_t split[8];
    union
    {
        u32 d32;
        u8 dat[4];
    } rx_crc;

    uint32_t crc32;

    DCJK = 1;
    REPAIR_OUTPUT = 0;
    VADJ = 1;
    FAN = 1;
    s_cal_v_step = 0U;
    s_cc_confirm_count = 0U;
    
    calVal_Init();
    
    set_Curr_Duty(PWMMAX);
    next_10ms = timer_deadline_ms(TASK_10MS);
    //SET_BAUD(BAUD_9600);
    ch_uart_init(9600);
    while(flg_cal_mode != 0)
    {
        if(timer_period_elapsed(&next_10ms, TASK_10MS) != 0)
        {
            wdt_feed();
            adc_sample_all();
            Ged_Flash(50);
            
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
                
                crc32 = CRC32((uint8_t*)s_cal_rx_buf, len - 4);
                
                rx_crc.dat[3] = s_cal_rx_buf[len - 4 + 0];
                rx_crc.dat[2] = s_cal_rx_buf[len - 4 + 1];
                rx_crc.dat[1] = s_cal_rx_buf[len - 4 + 2];
                rx_crc.dat[0] = s_cal_rx_buf[len - 4 + 3];

                if(rx_crc.d32 == crc32)
                {

                    task = s_cal_rx_buf[0];
                    for(i = 0; i < 8; i++)
                    {
                        split[i] = s_cal_rx_buf[i + 1];
                    }
                    /* Calibration payload also uses 10mV / 10mA values. */
                    s_load_val.voltage_mv = (u16)split[0];
                    s_load_val.voltage_mv |= (u16)split[1] << 8;

                    s_load_val.current_ma = (u16)split[4];
                    s_load_val.current_ma |= (u16)split[5] << 8;
                    
                    
                    switch(task)
                    {
                    case '*':
                        cal_tx_str_crc("*OK");
                        s_cal_v_step = 0U;
                        s_cc_confirm_count = 0U;
                        cal_val.I4_adc = adc.curr;
                        VADJ = 1;
                        task = 1;
                        break;

                    case '#':
                        if(s_cal_v_step == 0)
                        {
                            cal_val.Vo_V1_adc  = adc.vout;
                            cal_val.V1_val     = s_load_val.voltage_mv;
                            s_cal_v_step = 1U;
                            cal_tx_str_crc("#ADJ");
                            VADJ = 0;
                        }
                        else if(s_cal_v_step == 1) 
                        {
                            s_cal_v_step = 2U;
                            cal_tx_str_crc("#ADJ");
                            VADJ = 0;
                        }
                        else
                        {
                            cal_val.I3_adc = cal_curr_adc_offset();
                            cal_val.I3_val = s_load_val.current_ma;
                            
                            cal_val.Vo_V2_adc = adc.vout;
                            cal_val.V2_val    = s_load_val.voltage_mv;
                            
                            s_cal_v_step = 0U;
                            VADJ = 1;
                            cal_tx_str_crc("#OK");
                        }
                        task = 1;
                        break;

                    case '$':
                        cal_val.I1_adc = cal_curr_adc_offset();
                        cal_val.I1_val = s_load_val.current_ma;

                        cal_tx_str_crc("$OK");
                        task = 1;
                        break;

                    case '!':
                        cal_val.I2_adc = cal_curr_adc_offset();
                        cal_val.I2_val = s_load_val.current_ma;
                        set_Curr_Duty(cal_val.cc_pwm);
                        s_cc_confirm_count = 0U;
                        cal_tx_str_crc("!OK");
                        task = 1;
                        break;

                    case '?':
                        if(s_cc_confirm_count < 2U)
                        {
                            s_cc_confirm_count++;
                        }
                        if(s_cc_confirm_count >= 2U)
                        {
                            cal_val.cc_pwm = get_Curr_Duty();
                            cal_val.cc_val = s_load_val.current_ma;
                            cal_tx_str_crc("?OK");
                        }
                        task = 1;
                        break;

                    case '@':
                        cal_tx_str_crc("@OK");
                        task = 1;
                        break;
                    case '%':
                        cal_tx_str_crc("%OK");
                        task = 1;
                        break;

                    case 'S':
                        save_cal_data();
                        cal_tx_str_crc("SAVE");
                        s_dummy_load_10ms = 500U;
                        DUMMY_LOAD = 1;
                        task = 1;
                        break;

                    case 'e':
                        cal_tx_str_crc("end");
                        delay_ms(50);
                        ch_uart_init(DBG_BAUD);
                        flg_cal_mode = 0;
                        task = 1;
                        break;

//                    case '%':
//                    case 'A':
//                    case 'B':
//                    case 'C':
//                    case 'D':
//                    case 'E':
//                    case 'F':
//                        echo_ok[0] = (char)task;
//                        echo_ok[1] = 'O';
//                        echo_ok[2] = 'K';
//                        echo_ok[3] = '\0';
//                        cal_tx_str_crc(echo_ok);
//                        task = 1;
//                        break;

                    default:
                        cal_tx_str_crc("ERROR");
                        break;
                    }
                }
                else
                {
                    cal_tx_str_crc("ERROR");
                }
            }
            
            data_len_bk = len;
        }
    }
}
