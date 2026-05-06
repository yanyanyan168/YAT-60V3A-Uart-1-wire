/**
  ******************************************************************************
  * @file    common.h
  * @brief   ????????????????
  ******************************************************************************
  */
#ifndef __BSP_COMMON_H__
#define __BSP_COMMON_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "typedef.h"
#include "include.h"
#include "usr_cfg.h"
#include <stdio.h>
typedef enum
{
    BSP_OK = 0,
    BSP_ERR = 1,
    BSP_ERR_TIMEOUT = 2,
    BSP_ERR_PARAM = 3,
    BSP_ERR_OVERFLOW = 4,
} BSP_RESULT_Types;
typedef enum
{
    UART_1WIRE_DIR_RX = 0,
    UART_1WIRE_DIR_TX = 1,
} UART_1WIRE_DIR_Types;
typedef struct
{
    u8 ch_ntcErr : 1;
    u8 ch_hotErr : 1;
    u8 ch_hot    : 1;
    u8 ch_ovp    : 1;
    u8 ch_ocp    : 1;
    u8 ch_vacErr : 1;
    u8 ch_hard   : 1;
} FLAG_Types;
typedef struct
{
    u8 ms;
    u8 s;
    u16 min;
} TIMER_Types;

typedef enum
{
    CH_IDLE = 0U,
    CH_Check,
    BMS_HANDSHAKE,   /* 等待BMS握手成功：等待A0/A1/A4/A6/A7完成，输出关闭 */
    CH_Pre1,
    CH_CCCV,
    CH_FULL,
    CH_OVP,
    CH_TimOut,
    CH_OTP,
    CH_OCP,
    NTC_ERR,
    HW_ERR,
    CH_UVP,
    CCCV_TimOut,
    BMS_TEMP_ERR,   /* BMS充电过/欠温：输出关闭，温度恢复后继续充电 */
    BMS_ERR,        /* BMS其它异常/通信连续失败：输出关闭，只等拔电池恢复 */
    CH_AGING,
} CH_STATUS_Types;
#define usr_printf                         printf
#define uart_printf                        printf
#define ARRAY_SIZE(a)                      (sizeof(a) / sizeof((a)[0]))
#define BSP_MIN(a, b)                      (((a) < (b)) ? (a) : (b))
#define BSP_MAX(a, b)                      (((a) > (b)) ? (a) : (b))
void memclr(void *addr, u16 len);
u16 common_clamp_u16(u16 val, u16 min_val, u16 max_val);
u8 common_sum_u8(u8 *buf, u8 len);

extern FLAG_Types idata ch_flag;
extern TIMER_Types idata Tim;
extern u8 idata data_len_bk;

void ch_err_ck(void);
void TimCut(void);
void Red_Flash(u8 period_n10ms);
void Ged_Flash(u8 period_n10ms);
void RGed_Flash(u8 period_n10ms);
#ifdef __cplusplus
}
#endif
#endif