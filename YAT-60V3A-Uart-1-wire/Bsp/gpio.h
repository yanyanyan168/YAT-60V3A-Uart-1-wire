/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   GPIO 命名、方向配置和板级 IO 控制接口。
  ******************************************************************************
  */
#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "common.h"

/* 引脚功能表 ---------------------------------------------------------------
 * P31/P17 : 程序下载口，本项目不配置。
 * P30     : FAN 风扇开关，高电平开启，充电时开启。
 * P27/P26 : 绿/红充电指示灯，高电平点亮。
 * P25/P24 : DEBUG UART TX/RX，兼容 54.6V DEBUG/校准通道。
 * P23     : VADJ，高电平拉高输出电压，上电/待机/异常必须为低。
 * P22     : 电流环基准 PWM 模拟 DAC，使用 STMR0 PWM。
 * P16     : 悬空，保持输入。
 * P10     : AIN8 电流采样，50mR 电阻 27 倍放大。
 * P07     : 超低压修复输出开关，高电平打开，继电器断开时才允许使用。
 * P05     : 假负载开关，高电平打开，负载卸载时短时开启。
 * P04     : COM 单线 UART，必须使用硬件 UART1 复用。
 * P03     : AIN3 电池电压采样，300k/20k 分压。
 * P02     : 电池电压采样分压通断控制。
 * P01     : AIN1 NTC 采样。
 * P00     : 充电输出继电器控制，高电平闭合。
 */

#define RELAY                               P00
#define DCJK                                P00
#define NTC_ADC_PIN                         P01
#define BATT_DIVIDER_EN                     P02
#define BATT_ADC_PIN                        P03
#define COM_PIN                             P04
#define DUMMY_LOAD                          P05
#define REPAIR_OUTPUT                       P07
#define CURR_ADC_PIN                        P10
#define CURR_REF_PWM_PIN                    P22
#define VADJ                                P23
#define DEBUG_RX_PIN                        P24
#define DEBUG_TX_PIN                        P25
#define RLED                                P26
#define GLED                                P27
#define FAN                                 P30

#define PSON_ON                             do{}while(0)
#define PSON_OFF                            do{}while(0)

/* GPIO 模式宏：严格使用 SDK 的 GPIO_Pxx_MODE_SEL 宏配置。 */
#define RELAY_OUTPUT()                      do{P0_MD0=(P0_MD0&(~GPIO_P00_MODE_SEL(0x3)))|GPIO_P00_MODE_SEL(0x1);}while(0)
#define BATT_DIVIDER_OUTPUT()               do{P0_MD0=(P0_MD0&(~GPIO_P02_MODE_SEL(0x3)))|GPIO_P02_MODE_SEL(0x1);}while(0)
#define BATT_ADC_ANALOG()                   do{P0_MD0=(P0_MD0&(~GPIO_P03_MODE_SEL(0x3)))|GPIO_P03_MODE_SEL(0x3);}while(0)
#define COM_UART_INPUT()                    do{P0_MD1=(P0_MD1&(~GPIO_P04_MODE_SEL(0x3)));}while(0)
#define COM_UART_OUTPUT()                   do{P0_MD1=(P0_MD1&(~GPIO_P04_MODE_SEL(0x3)))|GPIO_P04_MODE_SEL(0x1);}while(0)
#define DUMMY_LOAD_OUTPUT()                 do{P0_MD1=(P0_MD1&(~GPIO_P05_MODE_SEL(0x3)))|GPIO_P05_MODE_SEL(0x1);}while(0)
#define REPAIR_OUTPUT_MODE()                do{P0_MD1=(P0_MD1&(~GPIO_P07_MODE_SEL(0x3)))|GPIO_P07_MODE_SEL(0x1);}while(0)
#define NTC_ADC_ANALOG()                    do{P0_MD0=(P0_MD0&(~GPIO_P01_MODE_SEL(0x3)))|GPIO_P01_MODE_SEL(0x3);}while(0)
#define CURR_ADC_ANALOG()                   do{P1_MD0=(P1_MD0&(~GPIO_P10_MODE_SEL(0x3)))|GPIO_P10_MODE_SEL(0x3);}while(0)
#define P16_FLOAT_INPUT()                   do{P1_MD1=(P1_MD1&(~GPIO_P16_MODE_SEL(0x3)));}while(0)
#define CURR_REF_PWM_OUTPUT()               do{P2_MD0=(P2_MD0&(~GPIO_P22_MODE_SEL(0x3)))|GPIO_P22_MODE_SEL(0x1);}while(0)
#define VADJ_OUTPUT()                       do{P2_MD0=(P2_MD0&(~GPIO_P23_MODE_SEL(0x3)))|GPIO_P23_MODE_SEL(0x1);}while(0)
#define DEBUG_RX_INPUT()                    do{P2_MD1=(P2_MD1&(~GPIO_P24_MODE_SEL(0x3)));}while(0)
#define DEBUG_TX_OUTPUT()                   do{P2_MD1=(P2_MD1&(~GPIO_P25_MODE_SEL(0x3)))|GPIO_P25_MODE_SEL(0x1);}while(0)
#define RLED_OUTPUT()                       do{P2_MD1=(P2_MD1&(~GPIO_P26_MODE_SEL(0x3)))|GPIO_P26_MODE_SEL(0x1);}while(0)
#define GLED_OUTPUT()                       do{P2_MD1=(P2_MD1&(~GPIO_P27_MODE_SEL(0x3)))|GPIO_P27_MODE_SEL(0x1);}while(0)
#define FAN_OUTPUT()                        do{P3_MD0=(P3_MD0&(~GPIO_P30_MODE_SEL(0x3)))|GPIO_P30_MODE_SEL(0x1);}while(0)

void gpio_init(void);
void gpio_output_safe_off(void);

void relay_on(void);
void relay_off(void);
void fan_on(void);
void fan_off(void);
void dummy_load_on(void);
void dummy_load_off(void);
void vadj_high(void);
void vadj_low(void);
void batt_divider_on(void);
void batt_divider_off(void);
void repair_output_on(void);
void repair_output_off(void);

void red_led_on(void);
void red_led_off(void);
void red_led_toggle(void);
void green_led_on(void);
void green_led_off(void);
void green_led_toggle(void);

#endif
