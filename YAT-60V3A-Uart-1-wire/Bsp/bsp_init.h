/**
  ******************************************************************************
  * @file    bsp_init.h
  * @brief   项目级系统初始化接口。
  *
  * 说明：
  * 1. SDK 原始 system.c 保留在 Libraries/Source，不在 Bsp 中重复建立 system.c。
  * 2. 本文件只声明本项目的时钟、看门狗和全局初始化入口。
  * 3. 底层寄存器宏仍来自 SDK 的 Libraries/Include/system.h 和 include.h。
  ******************************************************************************
  */
#ifndef __BSP_INIT_H__
#define __BSP_INIT_H__

#include "common.h"

void system_clock_init(void);
void wdt_init(void);
void wdt_feed(void);
void Sys_Init(void);

#endif