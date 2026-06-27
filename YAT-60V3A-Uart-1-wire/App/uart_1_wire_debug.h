/**
  ******************************************************************************
  * @file    uart_1_wire_debug.h
  * @brief   P30 单线 UART 协议轻量调试日志接口。
  *
  * 设计目标：
  * 1. 保留通信调试能力。
  * 2. 尽量少占 C51 代码空间。
  * 3. 命令字和数据全部用 16 进制，方便对照协议和抓包。
  ******************************************************************************
  */
#ifndef __UART_1_WIRE_DEBUG_H__
#define __UART_1_WIRE_DEBUG_H__

#include "uart_1_wire.h"

#ifndef U1W_DEBUG_EN
#define U1W_DEBUG_EN                        (0U)
#endif

#ifndef U1W_DEBUG_PARSE_EN
#define U1W_DEBUG_PARSE_EN                  (0U)
#endif

#define U1W_DBG_BAD_NONE                    (0U)
#define U1W_DBG_BAD_LEN                     (1U)
#define U1W_DBG_BAD_CMD                     (2U)
#define U1W_DBG_BAD_CHK                     (3U)

#if (U1W_DEBUG_EN != 0U)
void u1w_dbg_tx(u8 *buf, u8 tx_len, u8 expect_len);
void u1w_dbg_rx_ok(u8 *buf, u8 rx_len);
void u1w_dbg_rx_bad(u8 reason, u8 wait_cmd, u8 wait_len, u8 rx_len, u8 *buf, u8 chk_calc);
void u1w_dbg_fail(u8 err, u8 wait_cmd, u8 rx_len, u8 wait_len, u8 retry_count);
void u1w_dbg_stop(u8 err);
void u1w_dbg_parse_ok(u8 cmd);
#else
#define u1w_dbg_tx(buf, tx_len, expect_len)                                      do{}while(0)
#define u1w_dbg_rx_ok(buf, rx_len)                                                do{}while(0)
#define u1w_dbg_rx_bad(reason, wait_cmd, wait_len, rx_len, buf, chk_calc)          do{}while(0)
#define u1w_dbg_fail(err, wait_cmd, rx_len, wait_len, retry_count)                 do{}while(0)
#define u1w_dbg_stop(err)                                                         do{}while(0)
#define u1w_dbg_parse_ok(cmd)                                                     do{}while(0)
#endif

#endif
