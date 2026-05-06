/**
  ******************************************************************************
  * @file    fifo.h
  * @brief   DEBUG 和 COM 接收队列接口。
  *
  * 说明：
  * 1. 本模块只处理环形队列，不依赖 UART、GPIO、ADC 等其它驱动。
  * 2. 为减少 RAM 占用，队列控制块只保留读写索引、计数和溢出标志。
  ******************************************************************************
  */
#ifndef __BSP_FIFO_H__
#define __BSP_FIFO_H__

#include "typedef.h"
#include "usr_cfg.h"

typedef struct
{
    u8 rd;
    u8 wr;
    u8 cnt;
    u8 overflow;
} fifo_t;

extern fifo_t idata s_ch_fifo;
extern fifo_t idata s_com_fifo;

void fifo_module_init(void);

void ch_fifo_clear(void);
void ch_fifo_push(u8 dat);
u8 ch_fifo_pop(void);

bit ch_fifo_overflow(void);

void com_fifo_clear(void);
void com_fifo_push(u8 dat);
u8 com_fifo_pop(void);

bit com_fifo_overflow(void);


#endif