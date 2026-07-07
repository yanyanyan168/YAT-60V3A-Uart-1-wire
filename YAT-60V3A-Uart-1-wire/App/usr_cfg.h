/**
  ******************************************************************************
  * @file    usr_cfg.h
  * @brief   60V3A 充电器项目用户配置文件
  *
  * 说明：
  * 1. 本文件只放项目级参数、阈值、通信速率、Flash 校准区偏移等配置。
  * 2. 业务逻辑、状态机流程、协议处理不要写在本文件内。
  * 3. 本工程使用 Keil C51，文件编码建议保持 GBK/ANSI，避免中文注释乱码。
  * 4. 中文只用于注释，不参与串口打印，不涉及 Keil C51 0xFD 字符串补丁问题。
  ******************************************************************************
  */
#ifndef __USR_CFG_H__
#define __USR_CFG_H__

/* 项目信息 ------------------------------------------------------------*/
#define PROJECT_NAME                "60V3A_CHARGER_1WIRE"  /* 工程名称，用于调试/识别 */
#define MODEL_NAME                  "60V3A"                 /* 机种名称 */
#define VERSION                     "V0.01"                 /* 软件版本号 */
#define JIZHONG                     (6030U)                  /* 机种编号/项目识别码，按上位机约定保留 */

/* 系统时基 ------------------------------------------------------------*/
#define SYS_CLK_HZ                  (24000000UL)             /* 系统主频，单位：Hz */
#define TASK_10MS                   (10U)                    /* 主循环基础任务周期，单位：ms */
#define COM_TIMEOUT_MS              (2000U)                  /* 一线通信整体超时时间，单位：ms */
#define UART_TX_WAIT                (60000UL)                /* 串口发送等待保护计数，防止死等 */

/* ADC 换算参数 --------------------------------------------------------*/
#define ADC_VREF_MV                 (5000UL)                 /* ADC 参考电压，单位：mV */
#define ADC_FULL                    (4095UL)                 /* 12 位 ADC 满量程 */
#define ADC_SAMPLE_CNT              (10U)                    /* ADC 采样次数 */
#define ADC_DROP_CNT                (2U)                     /* 去极值数量/滤波丢弃数量 */
#define R1                          (300UL)                  /* 输出电压采样上拉电阻，单位：Kohm */
#define R2                          (20UL)                   /* 输出电压采样下拉电阻，单位：Kohm */
#define Ra                          (50UL)                   /* 电流采样电阻，单位：mohm */
#define GAIN                        (27UL)                   /* 电流采样放大倍数 */

/* 充电电压/电流阈值 ---------------------------------------------------*/
#define BAT_SERIES                  (15U)                    /* 电池串数，60V 锂电按 15 串处理 */
#define CELL_LOW_MV                 (1000U)                  /* 单节极低电压阈值，单位：mV */
#define CELL_REPAIR_MV              (2000U)                  /* 单节修复/预充进入参考阈值，单位：mV */
#define CELL_PRE_MV                 (2500U)                  /* 单节预充切换阈值，单位：mV */
#define CELL_FULL_MV                (4200U)                  /* 单节满电目标电压，单位：mV */
#define CELL_RECHG_MV               (4000U)                  /* 单节回充参考电压，单位：mV */
#define vRESET                      (1000U)                  /* 输出复位/掉电判断参考，单位：mV */
#define vSTART                      (15000)                  /* 起充最低包电压，单位：mV */
#define vPRE_30V                    (30000)                  /* 低压修复/预充参考包电压，单位：mV */
#define vPRE_38V                    (38000)                  /* 预充转正常充电参考包电压，单位：mV */
#define vCH60                       (60000)                  /* 回充参考包电压，单位：mV */
#define SET_vMAX                    (63000U)                 /* 默认恒压设定，单位：mV */
#define vDCOVP                      (64500U)                 /* 输出过压保护阈值，单位：mV */
#define iREPAIR                     (100U)                   /* 低压修复电流，单位：mA */
#define iPRE                        (500U)                   /* 预充电流，单位：mA */
#define iMAX                        (3000U)                  /* 最大充电电流，单位：mA */
#define iGED                        (500U)                   /* 满电/转灯电流阈值，单位：mA */
#define iOCP                        (3500U)                  /* 过流保护阈值，单位：mA */
#define iOCP_OK                     (3200U)                  /* 过流恢复阈值，单位：mA */
#define TIM_PRE                     (30U)                    /* 预充最长时间，单位：min */
#define TIM_CCCV                    (480U)                   /* 恒流恒压最长时间，单位：min */


/* NTC 温度阈值 --------------------------------------------------------*/
#define T_HOT_ERR                   (1050U)                  /* NTC 高温保护阈值，单位：0.1℃，1050=105.0℃ */
#define T_HOT_ERR_OK                (800U)                   /* NTC 高温保护恢复阈值，单位：0.1℃，800=80.0℃ */
#define T_CH_HOT                    (950U)                   /* 内部高温提示阈值，不参与降额，单位：0.1℃ */
#define T_CH_HOT_OK                 (700U)                   /* 内部高温提示恢复阈值，不参与降额，单位：0.1℃ */
#define BATT_18650_CHG_TEMP_LOW_C   (-2)                     /* 18650电池充电低温保护阈值，单位：℃ */
#define BATT_18650_CHG_TEMP_HIGH_C  (50)                     /* 18650电池充电高温保护阈值，单位：℃ */
#define BATT_18650_REC_TEMP_LOW_C   (0)                      /* 18650电池温度恢复低阈值，单位：℃ */
#define BATT_18650_REC_TEMP_HIGH_C  (45)                     /* 18650电池温度恢复高阈值，单位：℃ */
#define BATT_21700_CHG_TEMP_LOW_C   (-2)                     /* 21700电池充电低温保护阈值，单位：℃ */
#define BATT_21700_CHG_TEMP_HIGH_C  (57)                     /* 21700电池充电高温保护阈值，单位：℃ */
#define BATT_21700_REC_TEMP_LOW_C   (0)                      /* 21700电池温度恢复低阈值，单位：℃ */
#define BATT_21700_REC_TEMP_HIGH_C  (50)                     /* 21700电池温度恢复高阈值，单位：℃ */

/* 通信参数 ------------------------------------------------------------*/
#define DBG_BAUD                    (115200UL)               /* 调试串口波特率 */
#define COM_BAUD                    (4800UL)                 /* 一线通信串口波特率 */
#define DEBUG_FIFO_SIZE             (32U)                    /* 调试串口接收 FIFO 大小 */
#define COM_FIFO_SIZE               (64U)                    /* 一线通信接收 FIFO 大小 */
#define CAL_FRAME_LEN               (16U)                    /* 校准帧最大长度 */
#define COM_FRAME_LEN               (32U)                    /* 一线通信帧最大长度 */
#define U1W_MASTER                  (0x10U)                  /* 一线通信主机地址/设备标识 */


/* PWM DAC 参数 --------------------------------------------------------*/
#define PWM_FREQ_HZ                 (10000UL)                /* PWM DAC 频率，单位：Hz */

/* 电流 PWM 预置：mA * mohm * 放大倍数 / 5V。 */
#define CC(x_ma)                    \
    (u16)(((((((u32)(x_ma) * Ra * GAIN) + 500UL) / 1000UL) * (PWMMAX + 1UL)) + (ADC_VREF_MV / 2UL)) / ADC_VREF_MV)

/* Flash 校准区配置 ------------------------------------------------------*/
#define CAL_FLAG                    (0x5AA5U)                /* 校准数据有效标志 */

#endif
