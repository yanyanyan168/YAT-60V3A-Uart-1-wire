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
 * P30     : COM 单线 UART，必须使用硬件 UART1 复用。
 * P27     : 悬空，保持输入，不主动配置为输出。
 * P25/P24 : DEBUG UART TX/RX，兼容 54.6V DEBUG/校准通道。
 * P26     : FAN 风扇开关，高电平开启，充电时开启。
 * P23     : 红充电指示灯，高电平点亮。
 * P22     : 绿充电指示灯，高电平点亮。
 * P16     : 电流环基准 PWM 模拟 DAC，使用 STMR0 PWM。
 * P10     : AIN8 电流采样，50mR 电阻 27 倍放大。
 * P07     : VADJ，高电平拉高输出电压，上电/待机/异常必须为低。
 * P03     : AIN3 电池电压采样，300k/20k 分压。
 * P05     : AIN1 NTC 采样。
 * P04     : 电池电压采样分压通断控制。
 * P02     : 超低压修复输出开关，高电平打开，继电器断开时才允许使用。
 * P01     : 充电输出继电器控制，高电平闭合。
 * P00     : 假负载开关，高电平打开，负载卸载时短时开启。
 */
 
/* 引脚别名定义，方便代码阅读与维护 */
#define DUMMY_LOAD                          P00
#define DCJK                                P01  // 充电输出控制
#define REPAIR_OUTPUT                       P02
#define BATT_ADC_PIN                        P03
#define BATT_DIVIDER_EN                     P04
#define NTC_ADC_PIN                         P05
#define VADJ                                P07
#define CURR_ADC_PIN                        P10
#define CURR_REF_PWM_PIN                    P16
#define GLED                                P22
#define DEBUG_RX_PIN                        P24
#define DEBUG_TX_PIN                        P25
#define RLED                                P23
#define FAN                                 P26
#define COM_PIN                             P30
 
 
/* GPIO 模式宏：严格使用 SDK 的 GPIO_Pxx_MODE_SEL 宏配置。
 * 通过修改 Px_MDx 寄存器来设置引脚的工作模式。
 */
#define DUMMY_LOAD_OUTPUT()                 do{P0_MD0=(P0_MD0&(~GPIO_P00_MODE_SEL(0x3)))|GPIO_P00_MODE_SEL(0x1);}while(0) // P00推挽输出
#define RELAY_OUTPUT()                      do{P0_MD0=(P0_MD0&(~GPIO_P01_MODE_SEL(0x3)))|GPIO_P01_MODE_SEL(0x1);}while(0) // P01推挽输出
#define REPAIR_OUTPUT_MODE()                do{P0_MD0=(P0_MD0&(~GPIO_P02_MODE_SEL(0x3)))|GPIO_P02_MODE_SEL(0x1);}while(0) // P02推挽输出
#define BATT_ADC_ANALOG()                   do{P0_MD0=(P0_MD0&(~GPIO_P03_MODE_SEL(0x3)))|GPIO_P03_MODE_SEL(0x3);}while(0) // P03模拟输入
#define BATT_DIVIDER_OUTPUT()               do{P0_MD1=(P0_MD1&(~GPIO_P04_MODE_SEL(0x3)))|GPIO_P04_MODE_SEL(0x1);}while(0) // P04推挽输出
#define NTC_ADC_ANALOG()                    do{P0_MD1=(P0_MD1&(~GPIO_P05_MODE_SEL(0x3)))|GPIO_P05_MODE_SEL(0x3);}while(0) // P05模拟输入
#define VADJ_OUTPUT()                       do{P0_MD1=(P0_MD1&(~GPIO_P07_MODE_SEL(0x3)))|GPIO_P07_MODE_SEL(0x1);}while(0) // P07推挽输出
#define CURR_ADC_ANALOG()                   do{P1_MD0=(P1_MD0&(~GPIO_P10_MODE_SEL(0x3)))|GPIO_P10_MODE_SEL(0x3);}while(0) // P10模拟输入
#define CURR_REF_PWM_OUTPUT()               do{P1_MD1=(P1_MD1&(~GPIO_P16_MODE_SEL(0x3)))|GPIO_P16_MODE_SEL(0x1);}while(0) // P16推挽输出
#define GLED_OUTPUT()                       do{P2_MD0=(P2_MD0&(~GPIO_P22_MODE_SEL(0x3)))|GPIO_P22_MODE_SEL(0x1);}while(0) // P22推挽输出
#define RLED_OUTPUT()                       do{P2_MD0=(P2_MD0&(~GPIO_P23_MODE_SEL(0x3)))|GPIO_P23_MODE_SEL(0x1);}while(0) // P23推挽输出
#define DEBUG_RX_INPUT()                    do{P2_MD1=(P2_MD1&(~GPIO_P24_MODE_SEL(0x3)));}while(0)                      // P24输入(高阻)
#define DEBUG_TX_OUTPUT()                   do{P2_MD1=(P2_MD1&(~GPIO_P25_MODE_SEL(0x3)))|GPIO_P25_MODE_SEL(0x1);}while(0) // P25推挽输出
#define FAN_OUTPUT()                        do{P2_MD1=(P2_MD1&(~GPIO_P26_MODE_SEL(0x3)))|GPIO_P26_MODE_SEL(0x1);}while(0) // P26推挽输出
#define P27_FLOAT_INPUT()                   do{P2_MD1=(P2_MD1&(~GPIO_P27_MODE_SEL(0x3)));}while(0)                      // P27输入(高阻)
#define COM_UART_INPUT()                    do{P3_MD0=(P3_MD0&(~GPIO_P30_MODE_SEL(0x3)));}while(0)                      // P30输入(高阻) - 用于UART1 RX
#define COM_UART_OUTPUT()                   do{P3_MD0=(P3_MD0&(~GPIO_P30_MODE_SEL(0x3)))|GPIO_P30_MODE_SEL(0x1);}while(0) // P30推挽输出 - 用于UART1 TX
 
void gpio_init(void);
 
 
#endif