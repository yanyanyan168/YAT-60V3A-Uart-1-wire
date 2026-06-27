/**
  ******************************************************************************
  * @file    bsp_init.c
  * @brief   ??????????????????
  ******************************************************************************
  */
#include "bsp_init.h"
#include "gpio.h"
#include "fifo.h"
#include "uart.h"
#include "pwm.h"
#include "adc.h"
#include "timer.h"

void wdt_init(void)
{
    WDT_KEY = WDT_KEY_VAL(0x55);
    WDT_CON = WDT_PRESCALE_SEL(0x7);
    WDT_KEY = WDT_KEY_VAL(0xCC);
}

void wdt_feed(void)
{
    WDT_KEY = WDT_KEY_VAL(0xAA);
}

void Sys_Init(void)
{
    IE_EA = 0;
    
    system_init();
    gpio_init();
    fifo_module_init();
    pwm_init();
    adc_init();
    uart_init();
    timer_init();
    wdt_init();
    IE_EA = 1;
}