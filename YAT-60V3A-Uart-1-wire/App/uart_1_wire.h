/**
  ******************************************************************************
  * @file    uart_1_wire.h
  * @brief   P30 单线 UART 通信协议接口，异步轮询版。
  *
  * 设计边界：
  * 1. uart_1_wire.c 只负责协议收发、帧扫描、校验、解析和通信超时。
  * 2. ch.c 只设置通信阶段，并根据握手完成/通信超时控制充电流程。
  * 3. 发送不再“发一帧等一帧回复”，而是按阶段每 100ms 轮询发送。
  * 4. 接收从缓存中扫描已知 CMD，CMD + 长度 + CHK 正确才更新数据。
  ******************************************************************************
  */
#ifndef __UART_1_WIRE_H__
#define __UART_1_WIRE_H__

#include "common.h"

/* 命令字：只保留充电器流程用到的命令。 */
#define U1W_CMD_A0                         (0xA0U)    /* 查询从机配置 */
#define U1W_CMD_A1                         (0xA1U)    /* 查询电芯规格和容量 */
#define U1W_CMD_A4                         (0xA4U)    /* 查询预充/满充电压 */
#define U1W_CMD_A6                         (0xA6U)    /* 查询最大充放电电流 */
#define U1W_CMD_A7                         (0xA7U)    /* 查询二级温区和限流 */
#define U1W_CMD_B1                         (0xB1U)    /* 查询最低/最高单节电压 */
#define U1W_CMD_B3                         (0xB3U)    /* 查询电池温度/MOS温度 */
#define U1W_CMD_B4                         (0xB4U)    /* 查询SOC和充电状态 */
#define U1W_CMD_B6                         (0xB6U)    /* 控制电池包 MOS/LED显示 */

#ifndef U1W_MASTER
#define U1W_MASTER                         (0x10U)    /* A0 主机类型：充电器 */
#endif
#define U1W_CELL_TYPE_21700                 (1U)       /* A1 XX=1表示21700，其他按18650处理 */

#define U1W_B6_TYPE_LED                    (0x00U)    /* B6：LED控制 */
#define U1W_B6_TYPE_MOS                    (0x01U)    /* B6：MOS控制 */
#define U1W_B6_TYPE_SOC                    (0x03U)    /* B6：满电显示SOC */
#define U1W_MOS_CHG_ON                     (0x01U)    /* B6：要求打开充电MOS */
#define U1W_LED_SHOW_CHARGE                (0x02U)    /* B6：显示充电状态5S */

/* B4 状态位。当前版本只记录，不再用这些位决定 B6 开关。 */
#define U1W_B4_OV                          (0x01U)
#define U1W_B4_LOW_TEMP                    (0x02U)
#define U1W_B4_HIGH_TEMP                   (0x04U)
#define U1W_B4_MOS_HOT                     (0x08U)
#define U1W_B4_OCP                         (0x10U)
#define U1W_B4_SHORT                       (0x20U)
#define U1W_B4_TIMEOUT                     (0x40U)
#define U1W_B4_FAIL                        (0x80U)
#define U1W_B4_FAULT_MASK                  (0xFFU)

/* 错误码。 */
#define U1W_ERR_NONE                       (0x00U)
#define U1W_ERR_FRAME                      (0x01U)
#define U1W_ERR_TIMEOUT                    (0x02U)    /* 兼容调试打印 */
#define U1W_ERR_RETRY_OVER                 (0x03U)    /* 兼容调试打印 */
#define U1W_ERR_BUS_LOW                    (0x04U)    /* 兼容调试打印 */
#define U1W_ERR_COMM_LOST                  (0x05U)
#define U1W_ERR_SEND                       (0x06U)
#define U1W_ERR_NOT_READY                  (0x07U)    /* 兼容调试打印 */

/* ch.c 控制通信层处于哪个阶段。 */
typedef enum
{
    U1W_STAGE_STOP = 0U,                    /* 不通信，释放 COM */
    U1W_STAGE_HANDSHAKE,                    /* 轮询 A0/A1/A4/A6/A7/B1/B3/B4 */
    U1W_STAGE_CHARGE,                       /* 轮询 B1/B3/B4/B6，B6要求开MOS */
    U1W_STAGE_FULL_DISPLAY,                 /* 满电后发送B6 03 SOC，3分钟后拉低COM */
    U1W_STAGE_TEMP_WAIT,                    /* 温度异常等待恢复，继续轮询B3/B4 */
    U1W_STAGE_PULL_LOW                      /* 主动拉低COM，告知BMS主机断开 */
} UART_1WIRE_STAGE_Types;

typedef enum
{
    U1W_LINK_OFFLINE = 0U,
    U1W_LINK_HANDSHAKE,
    U1W_LINK_ONLINE
} UART_1WIRE_LINK_Types;

/* 协议解析结果，供调试和少量充电算法读取。 */
typedef struct
{
    u8  link_state;
    u8  last_error;
    u8  comm_timeout;                       /* 1=总通信或关键帧超时 */
    u8  key_timeout_cmd;                    /* 最近超时的关键帧CMD，0表示总超时 */
    u8  stage;                              /* UART_1WIRE_STAGE_Types */
    u8  handshake_ok;                       /* A0/A1/A4/A6/A7/B1/B3/B4 收齐标志 */
    u8  handshake_mask;                     /* A0/A1/A4/A6/A7/B1/B3/B4 收齐原始掩码 */
    u8  retry_over;                         /* 兼容旧调试含义：通信已超时 */
    u8  aging_cmd;                          /* 收到 AGING 老化命令 */

    u8  pack_id;
    u8  cell_type;
    u8  cell_series;
    u8  cell_parallel;
    u8  soc_percent;
    u8  charge_status;

    s8  batt_temp_degc;
    s8  mos_temp_degc;
    s8  derate_low_degc;
    s8  derate_high_degc;

    u16 cell_cap_01ah;
    u16 cell_pre_mv;
    u16 cell_min_mv;
    u16 cell_max_mv;
    u16 target_voltage_mv;
    u16 max_charge_current_ma;
    u16 derate_current_ma;
    u16 target_current_ma;

    u16 offline_count_10ms;                 /* 距离上次合法帧的时间 */
    u16 no_rx_10ms;                         /* 距离上次合法帧的时间，供充电中拔电池判断 */
} UART_1WIRE_INFO_Types;

extern UART_1WIRE_INFO_Types xdata uart_1_wire;

void uart_1_wire_init(void);
void uart_1_wire_reset_link(void);
void uart_1_wire_set_stage(u8 stage);
void uart_1_wire_poll_10ms(void);
void uart_1_wire_task_10ms(void);           /* 兼容旧接口，等同于 poll_10ms */


#endif
