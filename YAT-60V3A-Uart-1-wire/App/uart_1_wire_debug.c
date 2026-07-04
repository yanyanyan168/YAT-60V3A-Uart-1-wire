/**
  ******************************************************************************
  * @file    uart_1_wire_debug.c
  * @brief   P30 单线 UART 轻量调试日志。
  *
  * 说明：
  * 1. 正常成功帧合并为一行，减少串口打印占用。
  * 2. 原始发送/接收数据统一用连续 16 进制显示。
  * 3. 运行时日志只用 ASCII，避免 C51 中文字符串乱码和代码空间增加。
  ******************************************************************************
  */

#include "uart_1_wire_debug.h"
#include "uart.h"

#if (U1W_DEBUG_EN != 0U)

/* 缓存最近一次发送帧，用于收到回复时合并打印一行日志。 */
static u8 idata s_dbg_tx_cmd;
static u8 idata s_dbg_tx_len;
static u8 idata s_dbg_tx_buf[4];

/* 发送一个调试字符，底层仍走调试 UART。 */
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

/* 打印 1 字节十六进制，不带 0x，例如 A0。 */
static void u1w_dbg_put_hex8(u8 val)
{
    u1w_dbg_put_hex4((u8)(val >> 4));
    u1w_dbg_put_hex4(val);
}

/* 命令过滤：0 表示全部打印，非 0 只打印指定命令。 */
static bit u1w_dbg_cmd_allowed(u8 cmd)
{
#if (U1W_DEBUG_CMD_FILTER != 0U)
    if(cmd != U1W_DEBUG_CMD_FILTER)
    {
        return 0;
    }
#else
    if(cmd == 0xFFU)
    {
        return 1;
    }
#endif
    return 1;
}

/* 命令短名，兼顾可读性和代码空间。 */
static char *u1w_dbg_cmd_name(u8 cmd)
{
    switch(cmd)
    {
    case U1W_CMD_A0: return "ID";
    case U1W_CMD_A1: return "CAP";
    case U1W_CMD_A4: return "VSET";
    case U1W_CMD_A6: return "ISET";
    case U1W_CMD_A7: return "TDER";
    case U1W_CMD_B1: return "CELLV";
    case U1W_CMD_B3: return "TEMP";
    case U1W_CMD_B4: return "SOC";
    case U1W_CMD_B6: return "MOS";
    default:         return "UNK";
    }
}

static void u1w_dbg_put_cmd_tag(u8 cmd)
{
    u1w_dbg_put_hex8(cmd);
    u1w_dbg_put_char('[');
    u1w_dbg_put_str(u1w_dbg_cmd_name(cmd));
    u1w_dbg_put_char(']');
}

/* 连续打印原始帧数据，中间不加空格以减少日志长度。 */
static void u1w_dbg_put_bytes(u8 *buf, u8 len)
{
    u8 i;

    for(i = 0U; i < len; i++)
    {
        u1w_dbg_put_hex8(buf[i]);
    }
}

#if (U1W_DEBUG_PARSE_EN != 0U)
static void u1w_dbg_put_u16(u16 dat)
{
    char buf[5];
    u8 i;
    u8 started;

    buf[0] = (char)('0' + (dat / 10000U));
    dat %= 10000U;
    buf[1] = (char)('0' + (dat / 1000U));
    dat %= 1000U;
    buf[2] = (char)('0' + (dat / 100U));
    dat %= 100U;
    buf[3] = (char)('0' + (dat / 10U));
    buf[4] = (char)('0' + (dat % 10U));

    started = 0U;
    for(i = 0U; i < 5U; i++)
    {
        if((buf[i] != '0') || (started != 0U) || (i == 4U))
        {
            started = 1U;
            u1w_dbg_put_char(buf[i]);
        }
    }
}

static void u1w_dbg_put_s8(s8 dat)
{
    if(dat < 0)
    {
        u1w_dbg_put_char('-');
        dat = (s8)(0 - dat);
    }
    u1w_dbg_put_u16((u16)((u8)dat));
}

static void u1w_dbg_put_01a(u16 raw)
{
    u1w_dbg_put_u16((u16)(raw / 10U));
    u1w_dbg_put_char('.');
    u1w_dbg_put_char((char)('0' + (raw % 10U)));
    u1w_dbg_put_char('A');
}

static u16 u1w_dbg_get_u16(u8 byte0, u8 byte1)
{
    return (u16)((u16)byte0 | ((u16)byte1 << 8));
}

static void u1w_dbg_put_parse(u8 *buf, u8 len)
{
    u8 cmd;
    u8 xy;
    u16 raw1;
    u16 raw2;

    if(len < 4U)
    {
        return;
    }

    cmd = buf[0];
    switch(cmd)
    {
    case U1W_CMD_A0:
        xy = buf[2];
        u1w_dbg_put_str(" ID=0x");
        u1w_dbg_put_hex8(buf[1]);
        u1w_dbg_put_char(' ');
        u1w_dbg_put_u16((u16)(((xy >> 4) & 0x0FU) + 5U));
        u1w_dbg_put_char('S');
        u1w_dbg_put_u16((u16)((xy & 0x0FU) + 1U));
        u1w_dbg_put_char('P');
        break;

    case U1W_CMD_A1:
        u1w_dbg_put_str(" CAP=");
        u1w_dbg_put_u16((u16)(buf[2] / 10U));
        u1w_dbg_put_char('.');
        u1w_dbg_put_char((char)('0' + (buf[2] % 10U)));
        u1w_dbg_put_str("Ah");
        break;

    case U1W_CMD_A4:
        if(len >= 6U)
        {
            raw1 = u1w_dbg_get_u16(buf[1], buf[2]);
            raw2 = u1w_dbg_get_u16(buf[3], buf[4]);
            u1w_dbg_put_str(" PRE=");
            u1w_dbg_put_u16((u16)(raw1 * 10U));
            u1w_dbg_put_str("mV FULL=");
            u1w_dbg_put_u16((u16)(raw2 * 10U));
            u1w_dbg_put_str("mV");
        }
        break;

    case U1W_CMD_A6:
        if(len >= 6U)
        {
            raw1 = u1w_dbg_get_u16(buf[1], buf[2]);
            raw2 = u1w_dbg_get_u16(buf[3], buf[4]);
            u1w_dbg_put_str(" I_CELL=");
            u1w_dbg_put_01a(raw1);
            u1w_dbg_put_str(" RAW2=");
            u1w_dbg_put_u16(raw2);
        }
        break;

    case U1W_CMD_A7:
        if(len >= 6U)
        {
            u1w_dbg_put_str(" T=");
            u1w_dbg_put_s8((s8)buf[1]);
            u1w_dbg_put_str("..");
            u1w_dbg_put_s8((s8)buf[2]);
            u1w_dbg_put_str("C DER=");
            u1w_dbg_put_01a(buf[4]);
        }
        break;

    case U1W_CMD_B1:
        if(len >= 6U)
        {
            raw1 = u1w_dbg_get_u16(buf[1], buf[2]);
            raw2 = u1w_dbg_get_u16(buf[3], buf[4]);
            u1w_dbg_put_str(" MIN=");
            u1w_dbg_put_u16((u16)(raw1 * 10U));
            u1w_dbg_put_str("mV MAX=");
            u1w_dbg_put_u16((u16)(raw2 * 10U));
            u1w_dbg_put_str("mV");
        }
        break;

    case U1W_CMD_B3:
        u1w_dbg_put_str(" BAT=");
        u1w_dbg_put_s8((s8)buf[1]);
        u1w_dbg_put_str("C MOS_T=");
        u1w_dbg_put_s8((s8)buf[2]);
        u1w_dbg_put_char('C');
        break;

    case U1W_CMD_B4:
        u1w_dbg_put_str(" SOC=");
        u1w_dbg_put_u16(buf[1]);
        u1w_dbg_put_str("% ST=0x");
        u1w_dbg_put_hex8(buf[2]);
        break;

    case U1W_CMD_B6:
        u1w_dbg_put_str(" TYPE=0x");
        u1w_dbg_put_hex8(buf[1]);
        u1w_dbg_put_str(" VAL=0x");
        u1w_dbg_put_hex8(buf[2]);
        break;

    default:
        break;
    }
}
#endif

/* 只缓存主机发送帧，当前发送帧最长按 4 字节处理。 */
static void u1w_dbg_copy_tx(u8 *src, u8 len)
{
    u8 i;

    for(i = 0U; i < len; i++)
    {
        s_dbg_tx_buf[i] = src[i];
    }
}

/* 发送时只缓存，不立即打印，避免一帧拆成多行。 */
void u1w_dbg_tx(u8 *buf, u8 tx_len, u8 expect_len)
{
    u8 cmd;

    cmd = buf[0];
    if(u1w_dbg_cmd_allowed(cmd) == 0)
    {
        return;
    }

    if(tx_len > 4U)
    {
        tx_len = 4U;
    }

    s_dbg_tx_cmd = cmd;
    s_dbg_tx_len = tx_len;
    u1w_dbg_copy_tx(buf, tx_len);

    if(expect_len > COM_FRAME_LEN)
    {
        s_dbg_tx_len = 0U;
    }
}

/* 收到合法回复后，把最近 TX 和当前 RX 合并成一行打印。 */
void u1w_dbg_rx_ok(u8 *buf, u8 rx_len)
{
    u8 cmd;

    cmd = buf[0];
    if(u1w_dbg_cmd_allowed(cmd) == 0)
    {
        return;
    }

    u1w_dbg_put_str("1W ");
    u1w_dbg_put_cmd_tag(cmd);
    u1w_dbg_put_str(" TX=");
    if((s_dbg_tx_len != 0U) && (s_dbg_tx_cmd == cmd))
    {
        u1w_dbg_put_bytes(s_dbg_tx_buf, s_dbg_tx_len);
    }
    else
    {
        u1w_dbg_put_char('-');
    }

    u1w_dbg_put_str(" RX=");
    u1w_dbg_put_bytes(buf, rx_len);
#if (U1W_DEBUG_PARSE_EN != 0U)
    u1w_dbg_put_parse(buf, rx_len);
#endif
    u1w_dbg_put_str(" OK\r\n");
}

/* 坏帧日志：保留原始 RX 和必要错误原因。 */
void u1w_dbg_rx_bad(u8 reason, u8 wait_cmd, u8 wait_len, u8 rx_len, u8 *buf, u8 chk_calc)
{
    if(u1w_dbg_cmd_allowed(wait_cmd) == 0)
    {
        return;
    }

    u1w_dbg_put_str("BAD ");
    u1w_dbg_put_cmd_tag(wait_cmd);
    u1w_dbg_put_str(" RX=");
    u1w_dbg_put_bytes(buf, rx_len);
    if(reason == U1W_DBG_BAD_CHK)
    {
        u1w_dbg_put_str(" CALC=");
        u1w_dbg_put_hex8(chk_calc);
    }
    else if(reason == U1W_DBG_BAD_LEN)
    {
        u1w_dbg_put_str(" LEN=");
        u1w_dbg_put_hex8(rx_len);
        u1w_dbg_put_char('/');
        u1w_dbg_put_hex8(wait_len);
    }
    else if(reason == U1W_DBG_BAD_CMD)
    {
        u1w_dbg_put_str(" CMD");
    }
    u1w_dbg_put_str("\r\n");
}


#endif
