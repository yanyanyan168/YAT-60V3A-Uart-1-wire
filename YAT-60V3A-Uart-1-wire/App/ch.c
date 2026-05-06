/**
  ******************************************************************************
  * @file    ch.c
  * @brief   主充电流程。
  *
  * 说明：
  * 1. 本文件按 54.6V2.5A 的状态机风格搭建，负责充电状态流转和输出控制。
  * 2. P04 一线通信协议在 uart_1_wire.c/.h 中实现，本文件只读取目标电压、电流、BMS状态。
  * 3. 每次状态变化必须打印中文日志，便于 DEBUG 串口现场判断流程走向。
  * 4. 电压、电流和计时阈值统一来自 usr_cfg.h，不在流程中散落硬编码。
  *
  * BMS异常处理边界：
  * 1. BMS_HANDSHAKE 专门等待A0/A1/A4/A6/A7握手成功，握手完成后才进入CH_Check。
 * 2. BMS 充电低温/高温进入 BMS_TEMP_ERR，输出关闭，温度恢复后回 CH_Check 继续充电。
  * 2. BMS 其它异常或通信连续失败进入 BMS_ERR，输出关闭，只等拔电池恢复 CH_IDLE。
  * 3. BT+ 拔包判断仍由主充电流程通过 val.vout < vRESET 完成，不放协议层处理。
  ******************************************************************************
  */
#include "ch.h"
#include "adc.h"
#include "gpio.h"
#include "pwm.h"
#include "timer.h"
#include "bsp_init.h"
#include "cal.h"
#include "usr_cfg.h"
#include "pc_uart.h"
#include "uart_1_wire.h"

CH_STATUS_Types idata ch_state;
CH_STATUS_Types idata last_state;

static u8 idata s_cut[4];          /* 状态内确认计数，防止临界点抖动。 */
static u8 idata s_tx_auto_tim;      /* DEBUG 自动上传计时，沿用 54.6V pc_uart_func(auto_tim)。 */

/* 一线通信主机发送间隔。协议要求从机回复后空闲再发下一条，当前由主流程控制。 */
#ifndef U1W_CH_SEND_INTERVAL_MS
#define U1W_CH_SEND_INTERVAL_MS      (30U)
#endif

/* BMS温度类异常允许恢复后继续充电，其它BMS异常只等拔电池。 */
#define BMS_TEMP_FAULT_MASK          (U1W_B4_LOW_TEMP | U1W_B4_HIGH_TEMP)
#define BMS_OTHER_FAULT_MASK         ((u8)(U1W_B4_FAULT_MASK & (u8)(~BMS_TEMP_FAULT_MASK)))

typedef struct
{
    CH_STATUS_Types state;          /* 状态枚举。 */
    char *name;                     /* DEBUG 打印用中文状态名。 */
} CH_STATE_ATTR_T;

/*
 * 状态属性表：
 * 1. BMS_HANDSHAKE 是等待通信配置阶段，输出关闭，握手成功后才进入CH_Check。
 * 2. BMS_TEMP_ERR 温度恢复后可继续充电，不清 Tim，便于保留累计充电时间。
 * 3. BMS_ERR 属于锁定类异常，只等拔电池恢复，不自动继续充电。
 * 4. 保护状态保留 Tim，方便 DEBUG 帧看到进入保护前累计充电时间。
 */
static CH_STATE_ATTR_T code s_ch_state_attr[] =
{
    { CH_IDLE,        "空载"       },
    { CH_Check,       "检测"       },
    { BMS_HANDSHAKE,  "等待BMS握手"},
    { CH_Pre1,        "修复/预充"  },
    { CH_CCCV,        "恒流恒压"   },
    { CH_FULL,        "满电"       },
    { CH_OVP,         "过压保护"   },
    { CH_TimOut,      "预充超时"   },
    { CH_OTP,         "过温保护"   },
    { CH_OCP,         "过流保护"   },
    { NTC_ERR,        "NTC异常"    },
    { HW_ERR,         "硬件异常"   },
    { CH_UVP,         "欠压保护"   },
    { CCCV_TimOut,    "CCCV超时"   },
    { BMS_TEMP_ERR,   "BMS温度异常"},
    { BMS_ERR,        "BMS异常"    },
    { CH_AGING,       "老化"       },
};


/**
  * @brief  按状态属性表获取中文状态名。
  */
static char *ch_state_name(CH_STATUS_Types state)
{
    u8 i;

    for(i = 0U; i < ARRAY_SIZE(s_ch_state_attr); i++)
    {
        if(s_ch_state_attr[i].state == state)
        {
            return s_ch_state_attr[i].name;
        }
    }

    return "未知";
}

/**
  * @brief  统一状态切换入口。
  * @param  next_state: 目标状态。
  * @param  reason    : 中文切换原因，可传空指针。
  */
static void ch_set_state(CH_STATUS_Types next_state, char *reason)
{
    if(ch_state == next_state)
    {
        return;
    }

    uart_printf("状态:%s 到 %s",
                ch_state_name(ch_state),
                ch_state_name(next_state));
    if(reason != 0)
    {
        uart_printf("，%s", reason);
    }
    uart_printf("，V=%umV I=%umA NTC=%ld\n",
                val.vout,
                val.curr,
                val.i_ntc);

    ch_state = next_state;
}

/**
  * @brief  判断是否仍有充电器硬件保护标志未恢复。
  */
static bit ch_fault_flag_active(void)
{
    if((ch_flag.ch_ntcErr != 0) ||
       (ch_flag.ch_hotErr != 0) ||
       (ch_flag.ch_ovp != 0) ||
       (ch_flag.ch_ocp != 0) ||
       (ch_flag.ch_vacErr != 0) ||
       (ch_flag.ch_hard != 0))
    {
        return 1;
    }

    return 0;
}

/**
  * @brief  保护抢占检查。
  * @return 1 表示本周期处于充电器硬件保护态。
  */
static bit ch_check_protect_state(void)
{
    if(ch_flag.ch_ntcErr != 0)
    {
        ch_set_state(NTC_ERR, "内部NTC开路或短路");
        return 1;
    }
    else if(ch_flag.ch_hotErr != 0)
    {
        ch_set_state(CH_OTP, "充电器高温");
        return 1;
    }
    else if(ch_flag.ch_ovp != 0)
    {
        ch_set_state(CH_OVP, "充电器高压");
        return 1;
    }
    else if(ch_flag.ch_ocp != 0)
    {
        ch_set_state(CH_OCP, "充电器OCP");
        return 1;
    }

    return 0;
}

/**
  * @brief  判断当前BMS状态是否为温度类异常。
  */
static bit ch_bms_temp_fault_active(void)
{
    if((uart_1_wire.charge_status & BMS_TEMP_FAULT_MASK) != 0U)
    {
        return 1;
    }

    return 0;
}

/**
  * @brief  BMS 是否存在非温度类锁定异常。
  */
static bit ch_bms_other_fault_active(void)
{
    u8 st;

    st = uart_1_wire.charge_status;

    if((st & (U1W_B4_OV |
              U1W_B4_MOS_HOT |
              U1W_B4_OCP |
              U1W_B4_SHORT |
              U1W_B4_TIMEOUT |
              U1W_B4_FAIL)) != 0U)
    {
        return 1;
    }

    return 0;
}

/**
  * @brief  检查 BMS 充电状态异常，并按异常类型切状态。
  *
  * @return 1: 已经检测到 BMS 异常并切换状态
  *         0: 当前没有 BMS 异常
  *
  * 说明：
  * 1. BMS 充电低温/高温进入 BMS_TEMP_ERR，温度恢复后允许继续充电。
  * 2. 其它异常统一进入 BMS_ERR，但日志原因要分开，便于现场判断。
  * 3. BMS_ERR 为锁定类异常，只等拔电池恢复。
  */
static bit ch_bms_fault_check(void)
{
    u8 st;

    if(uart_1_wire_is_online() == 0)
    {
        return 0;
    }

    st = uart_1_wire.charge_status;

    /* 温度类异常：允许恢复后继续充电 */
    if((st & U1W_B4_LOW_TEMP) != 0U)
    {
        uart_1_wire_set_charge_enable(0);
        ch_set_state(BMS_TEMP_ERR, "BMS充电低温");
        return 1;
    }

    if((st & U1W_B4_HIGH_TEMP) != 0U)
    {
        uart_1_wire_set_charge_enable(0);
        ch_set_state(BMS_TEMP_ERR, "BMS充电高温");
        return 1;
    }

    /*
     * 其它异常：锁定到 BMS_ERR，只等拔电池恢复。
     * 多个异常同时存在时，按安全优先级只打印第一个最关键原因。
     */
    if((st & U1W_B4_FAIL) != 0U)
    {
        uart_1_wire_set_charge_enable(0);
        ch_set_state(BMS_ERR, "BMS电池包失效");
        return 1;
    }

    if((st & U1W_B4_SHORT) != 0U)
    {
        uart_1_wire_set_charge_enable(0);
        ch_set_state(BMS_ERR, "BMS短路保护");
        return 1;
    }

    if((st & U1W_B4_OCP) != 0U)
    {
        uart_1_wire_set_charge_enable(0);
        ch_set_state(BMS_ERR, "BMS过流保护");
        return 1;
    }

    if((st & U1W_B4_MOS_HOT) != 0U)
    {
        uart_1_wire_set_charge_enable(0);
        ch_set_state(BMS_ERR, "BMS充电MOS过温");
        return 1;
    }

    if((st & U1W_B4_OV) != 0U)
    {
        uart_1_wire_set_charge_enable(0);
        ch_set_state(BMS_ERR, "BMS单节过充");
        return 1;
    }

    if((st & U1W_B4_TIMEOUT) != 0U)
    {
        uart_1_wire_set_charge_enable(0);
        ch_set_state(BMS_ERR, "BMS充电超时");
        return 1;
    }

    return 0;
}

/**
  * @brief  获取目标充电电压。
  */
static u16 ch_get_target_voltage_mv(void)
{
    u16 target_voltage_mv;

    target_voltage_mv = uart_1_wire_get_target_voltage_mv();
    if(target_voltage_mv > SET_vMAX)
    {
        target_voltage_mv = SET_vMAX;
    }

    return target_voltage_mv;
}

/**
  * @brief  获取目标充电电流。
  */
static u16 ch_get_target_current_ma(void)
{
    u16 target_current_ma;

    target_current_ma = uart_1_wire_get_target_current_ma();
    if(target_current_ma > iMAX)
    {
        target_current_ma = iMAX;
    }

    return target_current_ma;
}

/**
  * @brief  关闭所有充电输出，进入安全输出状态。
  *
  * 说明：
  * 硬件输出关闭时，同步要求下一次B6关闭电池包充电MOS。
  */
static void ch_output_all_off(void)
{
    uart_1_wire_set_charge_enable(0);

    relay_off();
    repair_output_off();
    vadj_low();
    fan_off();
    dummy_load_off();
    set_Curr_Duty(0);
    set_Vol_Duty(0);
}

/**
  * @brief  保护状态下等待电池端电压恢复到空载判据。
  */
static void idle_ck(void)
{
    if(val.vout < vRESET)
    {
        if(++s_cut[2] >= 50U)
        {
            uart_1_wire_reset_link();
            ch_set_state(CH_IDLE, "电池拔出，转空载");
        }
    }
    else
    {
        s_cut[2] = 0;
    }
}

/**
  * @brief  一线通信调度。
  *
  * 说明：
  * 1. 每10ms调用poll，负责接收、校验、解析和超时判断。
  * 2. 发送间隔由主充电流程控制，满足帧间空闲要求。
  * 3. 连续失败超过上限后，只置retry_over；本函数不自动复位，让状态机切入BMS_ERR。
  */
static void ch_uart_1wire_task_10ms(void)
{
    static u16 idata send_deadline_ms = 0U;

    uart_1_wire_poll_10ms();

    /*
     * BMS_ERR 是锁定类异常：只等拔电池恢复。
     * 进入该状态后不再主动发送B1/B3/B4/B6，避免异常状态下持续通讯。
     * BMS_TEMP_ERR 不能停通讯，因为温度恢复依赖B4状态更新。
     */
    if(ch_state == BMS_ERR)
    {
        uart_1_wire_set_charge_enable(0);
        return;
    }

    if(send_deadline_ms == 0U)
    {
        send_deadline_ms = timer_deadline_ms(U1W_CH_SEND_INTERVAL_MS);
    }

    if(timer_period_elapsed(&send_deadline_ms, U1W_CH_SEND_INTERVAL_MS) == 0)
    {
        return;
    }

    if(uart_1_wire_is_retry_over() != 0)
    {
        return;
    }

    if(uart_1_wire_can_send() != 0)
    {
        uart_1_wire_send_next();
    }
}

void usr_ch_func(void)
{
    u16 target_voltage_mv;
    u16 target_current_ma;
    u16 next_10ms;

    ch_state = CH_IDLE;
    last_state = CH_IDLE;
    s_tx_auto_tim = 0;

    next_10ms = timer_deadline_ms(TASK_10MS);
    uart_1_wire_reset_link();
    uart_printf("充电程序\n");

    while(flg_cal_mode == 0)
    {
        if(timer_period_elapsed(&next_10ms, TASK_10MS) != 0)
        {
            flg_10ms = 0;
            s_tx_auto_tim++;
            wdt_feed();

            /* DEBUG 口收到 *RST 后切入校准流程，保持 54.6V 行为。 */
            if(pc_uart_func(s_tx_auto_tim) == 1U)
            {
                flg_cal_mode = 1;
                break;
            }

            ch_uart_1wire_task_10ms();

            /* 每 10ms 更新 ADC 工程量和保护标志。 */
            adc_sample_all();
            ch_err_ck();

            /* 充电器自身保护优先级高于BMS保护。 */
            if(ch_check_protect_state() != 0)
            {
                ch_output_all_off();
            }

            target_voltage_mv = ch_get_target_voltage_mv();
            target_current_ma = ch_get_target_current_ma();
            
            if(last_state != ch_state)
            {
                ch_state = last_state;
                s_cut[0] = 0;
                s_cut[1] = 0;
                s_cut[2] = 0;
                s_cut[3] = 0;
                Tim.ms = 0;
                Tim.s = 0;
                Tim.min = 0;
            }

            switch(ch_state)
            {
            default:
            case CH_IDLE:
                /*
                 * 空载/待机：
                 * - 继电器、VADJ、PWM、风扇全部关闭；
                 * - 等待电池电压超过起充阈值；
                 * - 检测到电池后重新从A0开始握手，并进入BMS握手等待状态。
                 */
                ch_output_all_off();
                red_led_on();
                green_led_off();
                if(val.vout >= vSTART)
                {
                    if(++s_cut[0] >= 50U)
                    {
                        uart_1_wire_reset_link();
                        uart_1_wire_set_charge_enable(0);
                        ch_set_state(BMS_HANDSHAKE, "检测到电池接入，开始通信");
                    }
                }
                else
                {
                    s_cut[0] = 0;
                }
                break;

            case BMS_HANDSHAKE:
                /*
                 * 等待BMS握手成功：
                 * - 输出保持关闭；
                 * - 等待协议层完成 A0/A1/A4/A6/A7；
                 * - 通信连续失败进入BMS_ERR；
                 * - 握手期间如BMS已回复温度/其它异常，则按BMS异常分类处理；
                 * - 握手成功后才进入CH_Check，由CH_Check决定预充或CCCV。
                 */
                ch_output_all_off();
                red_led_on();
                green_led_off();

                if(val.vout < vSTART)
                {
                    if(++s_cut[0] >= 50U)
                    {
                        ch_set_state(CH_UVP, "等待握手时电池电压低于起充阈值");
                    }
                }
                else
                {
                    s_cut[0] = 0U;

                    if(ch_bms_fault_check() != 0)
                    {
                        /* 已经明确切入BMS_TEMP_ERR或BMS_ERR。 */
                    }
                    else if(uart_1_wire_is_online() != 0)
                    {
                        ch_set_state(CH_Check, "BMS握手成功");
                    }
                    else
                    {
                        /* 继续等待A0/A1/A4/A6/A7完成。 */
                    }
                }
                break;

            case CH_Check:
                /*
                 * 检测状态：
                 * - 此状态默认已经完成BMS握手；
                 * - 输出保持关闭；
                 * - 若通信被复位或尚未在线，则退回BMS_HANDSHAKE等待；
                 * - BMS无明确异常后，再根据电压进入预充或CCCV。
                 */
                ch_output_all_off();
                red_led_on();
                green_led_off();

                if(val.vout < vSTART)
                {
                    if(++s_cut[0] >= 50U)
                    {
                        ch_set_state(CH_UVP, "电池电压低于起充阈值");
                    }
                }
                else
                {
                    /*
                     * 电压已恢复到起充阈值以上，清掉欠压确认计数。
                     * 避免前面临界波动累计的s_cut[0]残留，后续再次波动时误进欠压。
                     */
                    s_cut[0] = 0U;

                    if(ch_bms_fault_check() != 0)
                    {
                        /* 已经明确切入BMS_TEMP_ERR或BMS_ERR。 */
                    }
                    else if(uart_1_wire_is_online() == 0)
                    {
                        ch_set_state(BMS_HANDSHAKE, "等待BMS握手成功");
                    }
                    else if(val.vout < vPRE1)
                    {
                        uart_1_wire_set_charge_enable(1);
                        ch_set_state(CH_Pre1, "通信正常，进入修复/预充");
                    }
                    else
                    {
                        uart_1_wire_set_charge_enable(1);
                        ch_set_state(CH_CCCV, "通信正常，进入恒流恒压");
                    }
                }
                break;

            case CH_Pre1:
                /*
                 * 修复/预充：
                 * - 正常充电前先检查BMS异常；
                 * - BMS温度异常可恢复，其它BMS异常锁定到拔电池。
                 */
                if(ch_bms_fault_check() != 0)
                {
                    break;
                }

                /*
                 * 不再使用 uart_1_wire_is_ready()==0 这种笼统判断。
                 * 充电中只按明确原因处理：
                 * - 连续通信失败超过上限：ch_bms_fault_check() -> BMS_ERR；
                 * - BMS过/欠温：ch_bms_fault_check() -> BMS_TEMP_ERR；
                 * - BMS其它异常：ch_bms_fault_check() -> BMS_ERR。
                 * 单帧等待/临时未更新不立即判故障，交给协议层重试计数处理。
                 */
                uart_1_wire_set_charge_enable(1);
                relay_on();
                repair_output_off();
                vadj_high();
                fan_on();
                Ged_Flash(50);
                TimCut();
                pwm_set_current_ref_ma(iPRE);

                if(Tim.min >= TIM_PRE)
                {
                    uart_1_wire_set_charge_enable(0);
                    ch_set_state(CH_TimOut, "预充超时");
                }
                else if(val.vout >= vPRE1)
                {
                    if(++s_cut[0] >= 50U)
                    {
                        ch_set_state(CH_CCCV, "预充转CCCV");
                    }
                }
                else
                {
                    s_cut[0] = 0;
                }
                break;

            case CH_CCCV:
                /*
                 * 恒流恒压：
                 * - 按协议目标电压/电流设置PWM；
                 * - 满电判断使用协议目标电压，而不是固定SET_vMAX；
                 * - BMS温度异常恢复后继续充电，其它BMS异常锁定到拔电池。
                 */
                if(ch_bms_fault_check() != 0)
                {
                    break;
                }

                /*
                 * 不再使用 uart_1_wire_is_ready()==0 这种笼统判断。
                 * 充电中只按明确原因处理：
                 * - 连续通信失败超过上限：ch_bms_fault_check() -> BMS_ERR；
                 * - BMS过/欠温：ch_bms_fault_check() -> BMS_TEMP_ERR；
                 * - BMS其它异常：ch_bms_fault_check() -> BMS_ERR。
                 * 单帧等待/临时未更新不立即判故障，交给协议层重试计数处理。
                 */
                uart_1_wire_set_charge_enable(1);
                relay_on();
                repair_output_off();
                vadj_high();
                fan_on();
                red_led_off();
                Ged_Flash(50);
                TimCut();
                set_Vol_Duty(SET_VOL(target_voltage_mv));
                set_Curr_Duty(SET_CURR(target_current_ma));

                if(Tim.min >= TIM_CCCV)
                {
                    uart_1_wire_set_charge_enable(0);
                    ch_set_state(CCCV_TimOut, "CCCV超时");
                }
                else if((val.vout >= target_voltage_mv) && (val.curr <= iGED))
                {
                    if(++s_cut[0] >= 100U)
                    {
                        uart_1_wire_set_charge_enable(0);
                        uart_1_wire_start_full_display();
                        ch_set_state(CH_FULL, "CCCV转FULL");
                    }
                }
                else
                {
                    s_cut[0] = 0;
                }
                break;

            case CH_FULL:
                /*
                 * 满电：
                 * - 停止输出并点亮绿灯；
                 * - 满电后由协议层在B6中显示电量3分钟；
                 * - 电压跌到再充阈值以下，确认后回检测状态。
                 */
                ch_output_all_off();
                red_led_off();
                green_led_on();
                if(val.vout < vCH20)
                {
                    if(++s_cut[0] >= 50U)
                    {
                        uart_1_wire_stop_full_display();
                        ch_set_state(CH_Check, "满电后电压回落，重新检测");
                    }
                }
                else
                {
                    s_cut[0] = 0;
                }
                break;

            case BMS_TEMP_ERR:
                /*
                 * BMS过/欠温：
                 * - 输出关闭；
                 * - 保持通信，持续获取B4状态；
                 * - 温度位恢复且没有其它BMS异常后，回检测状态继续充电。
                 */
                ch_output_all_off();
                uart_1_wire_set_charge_enable(0);
                RGed_Flash(50);

                if(ch_bms_other_fault_active() != 0)
                {
                    ch_set_state(BMS_ERR, "BMS温度恢复前出现其它异常");
                }
                else if((uart_1_wire_is_online() != 0) && (ch_bms_temp_fault_active() == 0))
                {
                    uart_1_wire_clear_error();
                    ch_set_state(CH_Check, "BMS温度恢复，继续充电");
                }
                break;

            case BMS_ERR:
                /*
                 * BMS非温度异常/通信连续失败：
                 * - 输出关闭；
                 * - 不因BMS状态恢复自动继续；
                 * - 只等拔电池，即BT+电压降到空载判据后回CH_IDLE。
                 */
                ch_output_all_off();
                Red_Flash(50);
                idle_ck();
                break;

            case CH_OTP:
                /*
                 * 充电器高温保护：温度恢复后回检测状态。
                 */
                ch_output_all_off();
                RGed_Flash(50);
                if(ch_flag.ch_hotErr == 0)
                {
                    ch_set_state(CH_Check, "OTP恢复");
                }
                if(ch_fault_flag_active() == 0)
                {
                    idle_ck();
                }
                break;

            case CH_TimOut:
            case CCCV_TimOut:
            case CH_UVP:
            case CH_OVP:
            case CH_OCP:
                /*
                 * 电压/电流/定时类保护：输出关闭，等电池拔出后恢复空载。
                 */
                ch_output_all_off();
                Red_Flash(50);
                if(ch_fault_flag_active() == 0)
                {
                    idle_ck();
                }
                break;

            case NTC_ERR:
            case HW_ERR:
                /*
                 * NTC/硬件异常：输出关闭，等待电压释放或人工处理。
                 */
                ch_output_all_off();
                RGed_Flash(50);
                if(ch_fault_flag_active() == 0)
                {
                    idle_ck();
                }
                break;

            case CH_AGING:
                /*
                 * 老化模式预留。
                 */
                relay_on();
                vadj_high();
                fan_on();
                set_Curr_Duty(SET_CURR(iMAX));
                Ged_Flash(50);
                break;
            }
        }
    }
}
