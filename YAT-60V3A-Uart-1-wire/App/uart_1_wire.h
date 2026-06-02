/**
  ******************************************************************************
  * @file    uart_1_wire.h
  * @brief   P30 单线 UART 通信协议接口，充电器专用版。
  *
  * 模块边界：
  * 1. 只实现 PDF 6.1“充电器和电池包通讯示例（充电）”相关命令：
  *    A0、A1、A4、A6、A7、B1、B3、B4、B6。
  * 2. 不实现工具模式、振荡模式、PDU 命令、产线配置、固件信息、记录查询等功能。
  * 3. 不处理“BT+ 端口判断电池包拔除”，该逻辑属于充电主流程/硬件检测。
  * 4. 发送间隔由充电主流程控制；本模块只负责发送当前阶段命令、等待回复、校验、解析和错误上报。
  * 5. 连续超时/帧错误超过 U1W_RETRY_MAX 后，本模块只置位 retry_over，不自动回 A0，由主流程决定是否复位重连。
  ******************************************************************************
  */
#ifndef __UART_1_WIRE_H__
#define __UART_1_WIRE_H__

#include "common.h"

/*============================== 命令字 ======================================*/

#define U1W_CMD_A0                         (0xA0U)    /* 查询从机配置 */
#define U1W_CMD_A1                         (0xA1U)    /* 查询电芯规格和容量 */
#define U1W_CMD_A4                         (0xA4U)    /* 查询单节预充和满充电压 */
#define U1W_CMD_A6                         (0xA6U)    /* 查询最大充放电电流 */
#define U1W_CMD_A7                         (0xA7U)    /* 查询二级充电温度和电流 */
#define U1W_CMD_B1                         (0xB1U)    /* 查询单节最低最高电压 */
#define U1W_CMD_B3                         (0xB3U)    /* 查询电池包和充电 MOS 温度 */
#define U1W_CMD_B4                         (0xB4U)    /* 查询电池包充电状态 */
#define U1W_CMD_B6                         (0xB6U)    /* 控制电池包显示或 MOS */

/* A0 主机类型：1X 表示充电器。 */
#ifndef U1W_MASTER
#define U1W_MASTER                         (0x10U)
#endif

/* B6 控制类型：充电器当前只用 MOS 控制和满电显示电量。 */
#define U1W_B6_TYPE_MOS                    (0x01U)    /* MOSFET 控制 */
#define U1W_B6_TYPE_SOC                    (0x03U)    /* 显示指定电量 */

/* B6 MOS 控制数据位。 */
#define U1W_MOS_CHG_ON                     (0x01U)    /* bit0：充电 MOS ON */

/* B4 充电状态位。当前按任意异常位停充，若厂家后续区分提示位，可再调整掩码。 */
#define U1W_B4_OV                          (0x01U)    /* 单节过充 */
#define U1W_B4_LOW_TEMP                    (0x02U)    /* 充电低温 */
#define U1W_B4_HIGH_TEMP                   (0x04U)    /* 充电高温 */
#define U1W_B4_MOS_HOT                     (0x08U)    /* 充电 MOS 过温 */
#define U1W_B4_OCP                         (0x10U)    /* 过流 */
#define U1W_B4_SHORT                       (0x20U)    /* 短路 */
#define U1W_B4_TIMEOUT                     (0x40U)    /* 充电超时 */
#define U1W_B4_FAIL                        (0x80U)    /* 电池包失效 */
#define U1W_B4_FAULT_MASK                  (0xFFU)

/* 协议层错误码。 */
#define U1W_ERR_NONE                       (0x00U)
#define U1W_ERR_FRAME                      (0x01U)    /* 帧长/命令/CHK 错误 */
#define U1W_ERR_TIMEOUT                    (0x02U)    /* 等待回复超时 */
#define U1W_ERR_RETRY_OVER                 (0x03U)    /* 当前命令连续失败超过上限 */
#define U1W_ERR_BUS_LOW                    (0x04U)    /* 总线持续低电平异常 */
#define U1W_ERR_COMM_LOST                  (0x05U)    /* 通讯超过 2s 中断 */
#define U1W_ERR_SEND                       (0x06U)    /* 发送接口返回失败 */
#define U1W_ERR_NOT_READY                  (0x07U)    /* 发送条件不满足 */

typedef enum
{
    U1W_STEP_A0 = 0U,                       /* 查询配置 */
    U1W_STEP_A1,                            /* 查询电芯规格和容量 */
    U1W_STEP_A4,                            /* 查询电压参数 */
    U1W_STEP_A6,                            /* 查询电流参数 */
    U1W_STEP_A7,                            /* 查询二级温区限流 */
    U1W_STEP_B1,                            /* 工作通讯：最低最高单节电压 */
    U1W_STEP_B3,                            /* 工作通讯：温度 */
    U1W_STEP_B4,                            /* 工作通讯：充电状态 */
    U1W_STEP_B6                             /* 工作通讯：MOS 控制/满电显示 */
} UART_1WIRE_STEP_Types;

typedef enum
{
    U1W_LINK_OFFLINE = 0U,                  /* 未上线或已离线 */
    U1W_LINK_HANDSHAKE,                     /* 已开始握手，配置未完整 */
    U1W_LINK_ONLINE                         /* 已完成 A0/A1/A4/A6/A7，进入 B1/B3/B4/B6 循环 */
} UART_1WIRE_LINK_Types;

/* 协议解析结果，供充电主流程读取。 */
typedef struct
{
    u8  link_state;                         /* UART_1WIRE_LINK_Types */
    u8  step;                               /* UART_1WIRE_STEP_Types */
    u8  last_error;                         /* U1W_ERR_xxx */
    u8  retry_count;                        /* 当前命令连续失败次数，便于主流程判断和调试 */
    u8  retry_over;                         /* 连续失败超过上限后置 1；主流程处理后调用 reset_link 清除 */

    u8  pack_id;                            /* A0：电池包识别码 */
    u8  cell_series;                        /* A0：串数，5~20 串 */
    u8  cell_parallel;                      /* A0：并数，1~16 并 */
    u8  soc_percent;                        /* B4：电量百分比 */
    u8  charge_status;                      /* B4：充电状态位 */

    s8  batt_temp_degc;                     /* B3：电池包温度，degC */
    s8  mos_temp_degc;                      /* B3：充电 MOS 温度，0xAA 表示无 MOS 温度 */
    s8  derate_low_degc;                    /* A7：二级低温点，degC */
    s8  derate_high_degc;                   /* A7：二级高温点，degC */

    u16 cell_cap_01ah;                      /* A1：容量，0.1Ah，用于最长充电保护时间 */
    u16 cell_pre_mv;                        /* A4：单节预充截止电压，mV */
    u16 cell_min_mv;                        /* B1：单节最低电压，mV */
    u16 cell_max_mv;                        /* B1：单节最高电压，mV */
    u16 target_voltage_mv;                  /* A4：整包目标电压，mV */
    u16 max_charge_current_ma;              /* A6：整包最大充电电流，mA */
    u16 derate_current_ma;                  /* A7：整包二级限流，mA */

    u16 offline_count_10ms;                 /* 通讯离线计数，10ms 为单位 */
} UART_1WIRE_INFO_Types;

extern UART_1WIRE_INFO_Types xdata uart_1_wire;

void uart_1_wire_init(void);
void uart_1_wire_poll_10ms(void);
void uart_1_wire_task_10ms(void);           /* 兼容旧接口：只等同于 poll_10ms，不再自动发送 */

bit uart_1_wire_can_send(void);
bit uart_1_wire_send_next(void);
bit uart_1_wire_is_busy(void);
bit uart_1_wire_is_retry_over(void);
u8  uart_1_wire_get_last_error(void);
void uart_1_wire_clear_error(void);

bit uart_1_wire_is_online(void);
bit uart_1_wire_is_ready(void);
bit uart_1_wire_has_fault(void);

u16 uart_1_wire_get_target_voltage_mv(void);
u16 uart_1_wire_get_target_current_ma(void);

void uart_1_wire_set_charge_enable(bit enable);
void uart_1_wire_start_full_display(void);
void uart_1_wire_stop_full_display(void);
void uart_1_wire_reset_link(void);

#endif
