/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   GPIO ?????? IO ???
  ******************************************************************************
  */
#include "gpio.h"

//static void gpio_pull_default(void)
//{
//    P0_PU &= ~(GPIO_P00_PULL_UP(1) | GPIO_P01_PULL_UP(1) |
//               GPIO_P02_PULL_UP(1) | GPIO_P03_PULL_UP(1) |
//               GPIO_P04_PULL_UP(1) | GPIO_P05_PULL_UP(1) |
//               GPIO_P07_PULL_UP(1));
//    P0_PD &= ~(GPIO_P00_PULL_PD(1) | GPIO_P01_PULL_PD(1) |
//               GPIO_P02_PULL_PD(1) | GPIO_P03_PULL_PD(1) |
//               GPIO_P04_PULL_PD(1) | GPIO_P05_PULL_PD(1) |
//               GPIO_P07_PULL_PD(1));
//    P1_PU &= ~(GPIO_P10_PULL_UP(1) | GPIO_P16_PULL_UP(1));
//    P1_PD &= ~(GPIO_P10_PULL_PD(1) | GPIO_P16_PULL_PD(1));
//    P2_PU &= ~(GPIO_P22_PULL_UP(1) | GPIO_P23_PULL_UP(1) |
//               GPIO_P24_PULL_UP(1) | GPIO_P25_PULL_UP(1) |
//               GPIO_P26_PULL_UP(1) | GPIO_P27_PULL_UP(1));
//    P2_PD &= ~(GPIO_P22_PULL_PD(1) | GPIO_P23_PULL_PD(1) |
//               GPIO_P24_PULL_PD(1) | GPIO_P25_PULL_PD(1) |
//               GPIO_P26_PULL_PD(1) | GPIO_P27_PULL_PD(1));
//    P3_PU &= ~GPIO_P30_PULL_UP(1);
//    P3_PD &= ~GPIO_P30_PULL_PD(1);
//}



void gpio_init(void)
{
//    gpio_pull_default();
    DUMMY_LOAD = 0;
    DCJK = 0;
    REPAIR_OUTPUT = 0;
    BATT_DIVIDER_EN = 0;
    VADJ = 0;
    CURR_REF_PWM_PIN = 0;
    GLED = 0;
    RLED = 0;
    FAN = 0;
    DUMMY_LOAD_OUTPUT();
    RELAY_OUTPUT();
    REPAIR_OUTPUT_MODE();
    BATT_DIVIDER_OUTPUT();
    VADJ_OUTPUT();
    CURR_REF_PWM_OUTPUT();
    GLED_OUTPUT();
    RLED_OUTPUT();
    FAN_OUTPUT();
    P27_FLOAT_INPUT();
    NTC_ADC_ANALOG();
    BATT_ADC_ANALOG();
    CURR_ADC_ANALOG();
    DEBUG_RX_INPUT();
    DEBUG_TX_OUTPUT();
    COM_UART_INPUT();
}
