/**
  ******************************************************************************
  * @file    main.c
  * @brief   60V3A 充电器应用入口，保持 54.6V 项目主循环风格。
  ******************************************************************************
  */
#include "bsp_init.h"
#include "ch.h"
#include "cal.h"
#include "uart_1_wire.h"
#include "common.h"

bit flg_cal_mode = 0;     /* 0: 充电流程  1: DEBUG 校准流程 */

void main(void)
{
    Sys_Init();
    calVal_Init();
    read_cal_data();
    uart_1_wire_init();

    usr_printf("%s %s\r\n", PROJECT_NAME, VERSION);

    for(;;)
    {
        if(flg_cal_mode == 0)
        {
            usr_ch_func();
        }
        else
        {
            usr_cal_func();
        }
    }
}
