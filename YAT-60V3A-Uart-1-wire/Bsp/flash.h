#ifndef __USER_FLASH_H__
#define __USER_FLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include <stdio.h>

#define FLASH_CAL_ADDR            (0x00)          // 起始地址128byte对齐
     
     
void flash_ferase_sector(u8 addr);
void flash_program(u8 addr, u8 *p_data, u8 len);
void flash_fread(u8 addr, u8 *p_data, u16 len);
     
#ifdef __cplusplus
}
#endif

#endif //__USER_PWM_H__