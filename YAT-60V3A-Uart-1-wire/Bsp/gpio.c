/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   GPIO ?????? IO ???
  ******************************************************************************
  */
#include "gpio.h"
static void gpio_pull_default(void)
{
    P0_PU &= ~(GPIO_P00_PULL_UP(1) | GPIO_P01_PULL_UP(1) |
               GPIO_P02_PULL_UP(1) | GPIO_P03_PULL_UP(1) |
               GPIO_P04_PULL_UP(1) | GPIO_P05_PULL_UP(1) |
               GPIO_P07_PULL_UP(1));
    P0_PD &= ~(GPIO_P00_PULL_PD(1) | GPIO_P01_PULL_PD(1) |
               GPIO_P02_PULL_PD(1) | GPIO_P03_PULL_PD(1) |
               GPIO_P04_PULL_PD(1) | GPIO_P05_PULL_PD(1) |
               GPIO_P07_PULL_PD(1));
    P1_PU &= ~(GPIO_P10_PULL_UP(1) | GPIO_P16_PULL_UP(1));
    P1_PD &= ~(GPIO_P10_PULL_PD(1) | GPIO_P16_PULL_PD(1));
    P2_PU &= ~(GPIO_P22_PULL_UP(1) | GPIO_P23_PULL_UP(1) |
               GPIO_P24_PULL_UP(1) | GPIO_P25_PULL_UP(1) |
               GPIO_P26_PULL_UP(1) | GPIO_P27_PULL_UP(1));
    P2_PD &= ~(GPIO_P22_PULL_PD(1) | GPIO_P23_PULL_PD(1) |
               GPIO_P24_PULL_PD(1) | GPIO_P25_PULL_PD(1) |
               GPIO_P26_PULL_PD(1) | GPIO_P27_PULL_PD(1));
    P3_PU &= ~GPIO_P30_PULL_UP(1);
    P3_PD &= ~GPIO_P30_PULL_PD(1);
}
void gpio_output_safe_off(void)
{
    relay_off();
    repair_output_off();
    dummy_load_off();
    vadj_low();
    fan_off();
}
void gpio_init(void)
{
    gpio_pull_default();
    RELAY = 0;
    BATT_DIVIDER_EN = 1;
    DUMMY_LOAD = 0;
    REPAIR_OUTPUT = 0;
    VADJ = 0;
    RLED = 0;
    GLED = 1;
    FAN = 0;
    RELAY_OUTPUT();
    BATT_DIVIDER_OUTPUT();
    DUMMY_LOAD_OUTPUT();
    REPAIR_OUTPUT_MODE();
    VADJ_OUTPUT();
    RLED_OUTPUT();
    GLED_OUTPUT();
    FAN_OUTPUT();
    NTC_ADC_ANALOG();
    BATT_ADC_ANALOG();
    CURR_ADC_ANALOG();
    P16_FLOAT_INPUT();
    DEBUG_RX_INPUT();
    DEBUG_TX_OUTPUT();
    COM_UART_INPUT();
}
void relay_on(void)
{
    RELAY = 1;
}
void relay_off(void)
{
    RELAY = 0;
}
void fan_on(void)
{
    FAN = 1;
}
void fan_off(void)
{
    FAN = 0;
}
void dummy_load_on(void)
{
    DUMMY_LOAD = 1;
}
void dummy_load_off(void)
{
    DUMMY_LOAD = 0;
}
void vadj_high(void)
{
    VADJ = 1;
}
void vadj_low(void)
{
    VADJ = 0;
}
void batt_divider_on(void)
{
    BATT_DIVIDER_EN = 1;
}
void batt_divider_off(void)
{
    BATT_DIVIDER_EN = 0;
}
void repair_output_on(void)
{
    if(RELAY == 0)
    {
        REPAIR_OUTPUT = 1;
    }
}
void repair_output_off(void)
{
    REPAIR_OUTPUT = 0;
}
void red_led_on(void)
{
    RLED = 1;
}
void red_led_off(void)
{
    RLED = 0;
}
void red_led_toggle(void)
{
    RLED = !RLED;
}
void green_led_on(void)
{
    GLED = 1;
}
void green_led_off(void)
{
    GLED = 0;
}
void green_led_toggle(void)
{
    GLED = !GLED;
}