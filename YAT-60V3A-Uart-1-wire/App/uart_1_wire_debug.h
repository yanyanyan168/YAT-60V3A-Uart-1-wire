/**
  ******************************************************************************
  * @file    uart_1_wire_debug.h
  * @brief   P30 单线 UART 协议轻量调试日志接口。
  *
  * 设计目标：
  * 1. 保留协议调试能力。
  * 2. 默认关闭，不占正式版代码空间。
  * 3. 调试日志使用 ASCII + 16 进制，方便对照协议和抓包。
  ******************************************************************************
  */
#ifndef __UART_1_WIRE_DEBUG_H__
#define __UART_1_WIRE_DEBUG_H__

#include "uart_1_wire.h"

/* 总开关：0=关闭全部一线调试日志，1=打开。 */
#ifndef U1W_DEBUG_EN
#define U1W_DEBUG_EN                        (0U)
#endif

/* 解析日志开关：当前成功帧已合并打印，本宏保留给调试配置使用。 */
#ifndef U1W_DEBUG_PARSE_EN
#define U1W_DEBUG_PARSE_EN                  (1U)
#endif

/* 命令过滤：0=全部打印，其他值=只打印指定命令。 */
#ifndef U1W_DEBUG_CMD_FILTER
#define U1W_DEBUG_CMD_FILTER                (0U)
#endif

/* 坏帧原因：用于 u1w_dbg_rx_bad() 判断打印哪类错误。 */
#define U1W_DBG_BAD_NONE                    (0U)       /* 未指定错误原因。 */
#define U1W_DBG_BAD_LEN                     (1U)       /* 长度不足或长度不匹配。 */
#define U1W_DBG_BAD_CMD                     (2U)       /* 命令字不符合当前等待帧。 */
#define U1W_DBG_BAD_CHK                     (3U)       /* 校验和错误。 */

#if (U1W_DEBUG_EN != 0U)
/* 记录本次发送帧；只缓存，不立即打印，等待收到回复后合并输出。 */
void u1w_dbg_tx(u8 *buf, u8 tx_len, u8 expect_len);

/* 收到合法回复帧；输出一行 TX/RX/OK 调试日志。 */
void u1w_dbg_rx_ok(u8 *buf, u8 rx_len);

/* 收到异常回复帧；输出 BAD 日志和必要的错误细节。 */
void u1w_dbg_rx_bad(u8 reason, u8 wait_cmd, u8 wait_len, u8 rx_len, u8 *buf, u8 chk_calc);

#else
/* 正式版关闭日志时，所有调试接口编译为空操作，不占运行时间。 */
#define u1w_dbg_tx(buf, tx_len, expect_len)                                      do{}while(0)
#define u1w_dbg_rx_ok(buf, rx_len)                                                do{}while(0)
#define u1w_dbg_rx_bad(reason, wait_cmd, wait_len, rx_len, buf, chk_calc)          do{}while(0)

#endif

#endif
