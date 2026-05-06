/**
  ******************************************************************************
  * @file    fifo.c
  * @brief   DEBUG 和 COM 接收环形队列。
  ******************************************************************************
  */
#include "fifo.h"

static u8 xdata s_ch_buf[DEBUG_FIFO_SIZE];
static u8 xdata s_com_buf[COM_FIFO_SIZE];

fifo_t idata s_ch_fifo;
fifo_t idata s_com_fifo;


static void fifo_ctrl_reset(fifo_t *fifo)
{
    fifo->rd = 0U;
    fifo->wr = 0U;
    fifo->cnt = 0U;
    fifo->overflow = 0U;
}

void fifo_module_init(void)
{
    fifo_ctrl_reset(&s_ch_fifo);
    fifo_ctrl_reset(&s_com_fifo);
}

void ch_fifo_clear(void)
{
    fifo_ctrl_reset(&s_ch_fifo);
}

void ch_fifo_push(u8 dat)
{
    if(s_ch_fifo.cnt >= DEBUG_FIFO_SIZE)
    {
        s_ch_fifo.overflow = 1U;
        s_ch_fifo.rd++;
        if(s_ch_fifo.rd >= DEBUG_FIFO_SIZE)
        {
            s_ch_fifo.rd = 0U;
        }
        s_ch_fifo.cnt--;
    }

    s_ch_buf[s_ch_fifo.wr] = dat;
    s_ch_fifo.wr++;
    if(s_ch_fifo.wr >= DEBUG_FIFO_SIZE)
    {
        s_ch_fifo.wr = 0U;
    }
    s_ch_fifo.cnt++;
}

u8 ch_fifo_pop(void)
{
    u8 dat;

    dat = s_ch_buf[s_ch_fifo.rd];
    
    s_ch_fifo.rd++;
    if(s_ch_fifo.rd >= DEBUG_FIFO_SIZE)
    {
        s_ch_fifo.rd = 0U;
    }
    s_ch_fifo.cnt--;
    
    return dat;
}


bit ch_fifo_overflow(void)
{
    return (bit)s_ch_fifo.overflow;
}

void com_fifo_clear(void)
{
    fifo_ctrl_reset(&s_com_fifo);
}

void com_fifo_push(u8 dat)
{
    if(s_com_fifo.cnt >= COM_FIFO_SIZE)
    {
        s_com_fifo.overflow = 1U;
        s_com_fifo.rd++;
        if(s_com_fifo.rd >= COM_FIFO_SIZE)
        {
            s_com_fifo.rd = 0U;
        }
        s_com_fifo.cnt--;
    }

    s_com_buf[s_com_fifo.wr] = dat;
    s_com_fifo.wr++;
    if(s_com_fifo.wr >= COM_FIFO_SIZE)
    {
        s_com_fifo.wr = 0U;
    }
    s_com_fifo.cnt++;
}

u8 com_fifo_pop(void)
{
    u8 dat;
    
    dat = s_com_buf[s_com_fifo.rd];
    s_com_fifo.rd++;
    if(s_com_fifo.rd >= COM_FIFO_SIZE)
    {
        s_com_fifo.rd = 0U;
    }
    s_com_fifo.cnt--;
    
    return dat;
}



bit com_fifo_overflow(void)
{
    return (bit)s_com_fifo.overflow;
}


