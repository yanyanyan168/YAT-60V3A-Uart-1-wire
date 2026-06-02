/**
  ******************************************************************************
  * @file    usr_cfg.h
  * @brief   60V3A ??????
  *
  * ???
  * 1. ??????????????????????????
  * 2. ?????? 54.6V2.5A???????????????????
  * 3. ???????????????????
  ******************************************************************************
  */
#ifndef __USR_CFG_H__
#define __USR_CFG_H__
/* ?????? ------------------------------------------------------------*/
#define PROJECT_NAME                "60V3A_CHARGER_1WIRE"
#define MODEL_NAME                  "60V3A"
#define VERSION                     "V0.01"
#define JIZHONG                     (6030U)
#define V_RATED_MV                  (63000UL)
#define V_MAX_MV                    (64300UL)
#define I_RATED_MA                  (3000U)
/* ???? ----------------------------------------------------------------*/
#define SYS_CLK_HZ                  (24000000UL)
#define TASK_10MS                   (10U)
#define ADC_PERIOD_MS               (10U)
#define COM_TIMEOUT_MS              (2000U)
#define PROTO_TIMEOUT_MS            (30U)
#define UART_TX_WAIT                (60000UL)
/* ADC ??????? ------------------------------------------------------*/
#define ADC_VREF_MV                 (5000UL)
#define ADC_FULL                    (4095UL)
#define ADC_SAMPLE_CNT              (10U)
#define ADC_DROP_CNT                (2U)
#define R1                          (300UL)
#define R2                          (20UL)
#define Ra                          (50UL)
#define GAIN                        (27UL)
/* ??????? ----------------------------------------------------------*/
#define BAT_SERIES                  (15U)
#define CELL_LOW_MV                 (1000U)
#define CELL_REPAIR_MV              (2000U)
#define CELL_PRE_MV                 (2500U)
#define CELL_FULL_MV                (4200U)
#define CELL_RECHG_MV               (4000U)
#define vRESET                      (1000U)
#define vSTART                      (CELL_LOW_MV * BAT_SERIES)
#define vPRE                        (CELL_REPAIR_MV * BAT_SERIES)
#define vPRE1                       (CELL_PRE_MV * BAT_SERIES)
#define vCH20                       (CELL_RECHG_MV * BAT_SERIES)
#define SET_vMAX                    (63000U)
#define vDCOVP                      (64500U)
#define iREPAIR                     (100U)
#define iPRE                        (500U)
#define iMAX                        (3000U)
#define iGED                        (500U)
#define iOCP                        (3500U)
#define iOCP_OK                     (3200U)
#define TIM_PRE                     (30U)
#define TIM_CCCV                    (480U)
#define RELAY_OFF_MS                (1000U)
#define DUMMY_LOAD_MS               (3000U)
/* ???? ----------------------------------------------------------------*/
#define T_HOT_ERR                   (1050U)
#define T_HOT_ERR_OK                (800U)
#define T_CH_HOT                    (950U)
#define T_CH_HOT_OK                 (700U)
/* ???? ----------------------------------------------------------------*/
#define DBG_BAUD                    (115200UL)
#define COM_BAUD                    (4800UL)
#define DEBUG_FIFO_SIZE             (32U)
#define COM_FIFO_SIZE               (64U)
#define CAL_FRAME_LEN               (16U)
#define COM_FRAME_LEN               (32U)
#define U1W_MASTER                  (0x10U)
#define U1W_B6_TYPE_DEF             (0x01U)
#define U1W_B6_DATA_DEF             (0x01U)
/* PWM DAC -----------------------------------------------------------------*/
#define PWM_FREQ_HZ                 (10000UL)
#define PWM_DUTY_DEF                (0U)
/* Flash -------------------------------------------------------------------*/
#define FLASH_INFO_SEC_H            (0x3FU)
#define FLASH_CAL_OFF               (0x00U)  /* 校准数据偏移：实际地址 0x3F00 */
#define FLASH_CAL_SIZE              (0x80U)  /* 当前只使用 128 Bytes 校准区 */
#define CAL_FLAG                    (0x5AA5U)
#endif