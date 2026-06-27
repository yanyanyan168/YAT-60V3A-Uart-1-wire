/**
  ******************************************************************************
  * @file    uart_1_wire_debug.c
  * @brief   P30 单线 UART 轻量中文调试日志。
  *
  * 说明：
  * 1. 只打印必要的收、发、坏帧、失败、停止信息。
  * 2. 命令字和数据字节统一用 16 进制显示。
  * 3. 字符串尽量短，避免 C51 代码空间占用过大。
  ******************************************************************************
  */

#include "uart_1_wire_debug.h"
#include "uart.h"

#if (U1W_DEBUG_EN != 0U)

static void u1w_dbg_put_char(char ch)
{
    ch_uart_send_byte((u8)ch);
}

static void u1w_dbg_put_str(char *str)
{
    ch_uart_send_str(str);
}

static void u1w_dbg_put_hex4(u8 val)
{
    val &= 0x0FU;
    if(val < 10U)
    {
        u1w_dbg_put_char((char)('0' + val));
    }
    else
    {
        u1w_dbg_put_char((char)('A' + val - 10U));
    }
}

static void u1w_dbg_put_hex8(u8 val)
{
    u1w_dbg_put_hex4((u8)(val >> 4));
    u1w_dbg_put_hex4(val);
}

static void u1w_dbg_put_cmd(u8 cmd)
{
    u1w_dbg_put_str("0x");
    u1w_dbg_put_hex8(cmd);
}

static void u1w_dbg_put_bytes(u8 *buf, u8 len)
{
    u8 i;

    for(i = 0U; i < len; i++)
    {
        if(i != 0U)
        {
            u1w_dbg_put_char(' ');
        }
        u1w_dbg_put_hex8(buf[i]);
    }
}

static void u1w_dbg_put_err(u8 err)
{
    switch(err)
    {
    case U1W_ERR_FRAME:
        u1w_dbg_put_str("帧错");
        break;

    case U1W_ERR_TIMEOUT:
        u1w_dbg_put_str("超时");
        break;

    case U1W_ERR_RETRY_OVER:
        u1w_dbg_put_str("重试超限");
        break;

    case U1W_ERR_BUS_LOW:
        u1w_dbg_put_str("总线低");
        break;

    case U1W_ERR_COMM_LOST:
        u1w_dbg_put_str("通讯丢失");
        break;

    case U1W_ERR_SEND:
        u1w_dbg_put_str("发送失败");
        break;

    case U1W_ERR_NOT_READY:
        u1w_dbg_put_str("未就绪");
        break;

    default:
        u1w_dbg_put_str("错误=0x");
        u1w_dbg_put_hex8(err);
        break;
    }
}

void u1w_dbg_tx(u8 *buf, u8 tx_len, u8 expect_len)
{
    u1w_dbg_put_str("[单线]发 命令=");
    u1w_dbg_put_cmd(buf[0]);
    u1w_dbg_put_str(" 长=");
    u1w_dbg_put_hex8(tx_len);
    u1w_dbg_put_str(" 等=");
    u1w_dbg_put_hex8(expect_len);
    u1w_dbg_put_str(" 数据=");
    u1w_dbg_put_bytes(buf, tx_len);
    u1w_dbg_put_str("\r\n");
}

void u1w_dbg_rx_ok(u8 *buf, u8 rx_len)
{
    u1w_dbg_put_str("[单线]收 命令=");
    u1w_dbg_put_cmd(buf[0]);
    u1w_dbg_put_str(" 长=");
    u1w_dbg_put_hex8(rx_len);
    u1w_dbg_put_str(" 数据=");
    u1w_dbg_put_bytes(buf, rx_len);
    u1w_dbg_put_str("\r\n");
}

void u1w_dbg_rx_bad(u8 reason, u8 wait_cmd, u8 wait_len, u8 rx_len, u8 *buf, u8 chk_calc)
{
    u1w_dbg_put_str("[单线]坏帧 原因=");

    if(reason == U1W_DBG_BAD_LEN)
    {
        u1w_dbg_put_str("长度");
    }
    else if(reason == U1W_DBG_BAD_CMD)
    {
        u1w_dbg_put_str("命令");
    }
    else if(reason == U1W_DBG_BAD_CHK)
    {
        u1w_dbg_put_str("校验 算=");
        u1w_dbg_put_hex8(chk_calc);
    }
    else
    {
        u1w_dbg_put_str("未知");
    }

    u1w_dbg_put_str(" 等=");
    u1w_dbg_put_cmd(wait_cmd);
    u1w_dbg_put_str(" 收=");
    u1w_dbg_put_hex8(rx_len);
    u1w_dbg_put_char('/');
    u1w_dbg_put_hex8(wait_len);

    if(rx_len != 0U)
    {
        u1w_dbg_put_str(" 数据=");
        u1w_dbg_put_bytes(buf, rx_len);
    }

    u1w_dbg_put_str("\r\n");
}

void u1w_dbg_fail(u8 err, u8 wait_cmd, u8 rx_len, u8 wait_len, u8 retry_count)
{
    u1w_dbg_put_str("[单线]失败 原因=");
    u1w_dbg_put_err(err);
    u1w_dbg_put_str(" 命令=");
    u1w_dbg_put_cmd(wait_cmd);
    u1w_dbg_put_str(" 收=");
    u1w_dbg_put_hex8(rx_len);
    u1w_dbg_put_char('/');
    u1w_dbg_put_hex8(wait_len);
    u1w_dbg_put_str(" 重试=");
    u1w_dbg_put_hex8(retry_count);
    u1w_dbg_put_str("\r\n");
}

void u1w_dbg_stop(u8 err)
{
    u1w_dbg_put_str("[单线]停止 原因=");
    u1w_dbg_put_err(err);
    u1w_dbg_put_str("\r\n");
}

void u1w_dbg_parse_ok(u8 cmd)
{
#if (U1W_DEBUG_PARSE_EN != 0U)
    u1w_dbg_put_str("[单线]解析 命令=");
    u1w_dbg_put_cmd(cmd);
    u1w_dbg_put_str(" OK");
    u1w_dbg_put_str("\r\n");
#else
    (void)cmd;
#endif
}

#endif
