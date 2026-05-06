/**
  ******************************************************************************
  * @file    ch.h
  * @brief   充电流程框架接口，状态命名保持 54.6V 项目风格。
  ******************************************************************************
  */
#ifndef __CH_H__
#define __CH_H__

#include "common.h"

extern bit flg_cal_mode;

extern CH_STATUS_Types idata ch_state;
extern CH_STATUS_Types idata last_state;
extern FLAG_Types idata ch_flag;
extern TIMER_Types idata Tim;

void usr_ch_func(void);
void ch_err_ck(void);
void TimCut(void);
void Red_Flash(u8 period_n10ms);
void Ged_Flash(u8 period_n10ms);
void RGed_Flash(u8 period_n10ms);

#endif
