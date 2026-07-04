/**
  ******************************************************************************
  * @file    ch.c
  * @brief   主充电流程。
  *
  * 说明：
  * 1. 本文件按 60V3A/63V 项目需求实现，负责充电状态流转和输出控制。
  * 2. P30 一线通信协议在 uart_1_wire.c/.h 中实现，本文件只读取目标电压、电流、BMS状态。
  * 3. 每次状态变化必须打印中文日志，便于 DEBUG 串口现场判断流程走向。
  * 4. 电压、电流和计时阈值统一来自 usr_cfg.h，不在流程中散落硬编码。
  *
  * 通信处理边界：
  * 1. BMS_HANDSHAKE 等待 A0/A1/A4/A6/A7/B1/B3/B4 全部收齐。
  * 2. 充电相关状态只检查通信超时，不解析协议帧细节。
  * 3. B6 由协议层在充电阶段固定要求打开充电 MOS；BMS状态由本文件分类处理。
  * 4. 满电/普通异常由协议层主动拉低 COM；BMS温度异常继续通信检查恢复。
  * 5. 拔电池全局处理：继电器闭合时用“小电流+无通信”，输出关闭时用电压检测。
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

static u16 idata s_cccv_curr_limit_ma;        /* CCCV 实际限流电流，单位 mA。 */
static u8  idata s_cccv_derate_cnt;           /* CCCV 降流间隔计数。 */
static u8  idata s_remove_cnt;                /* 拔电池确认计数。 */
static u16 idata s_idle_low_last_mv;          /* 待机低压候选上次电压。 */
static u16 idata s_vout_probe_period_10ms;    /* 满电/异常时分压检测间隔计数。 */
static u8  idata s_vout_probe_on_10ms;        /* 满电/异常时分压检测开窗计数。 */
static bit s_vout_sample_valid;               /* 本周期 val.vout 是否允许用于判断。 */

#define BMS_HANDSHAKE_TIMEOUT_S      (20U)
#define CH_VOUT_PROBE_PERIOD_10MS    (100U)   /* 满电/异常时每1秒打开一次分压检测 */
#define CH_VOUT_PROBE_ON_10MS        (10U)    /* 每次打开100ms */
#define CH_VOUT_PROBE_VALID_10MS     (5U)     /* 打开50ms后认为ADC有效 */
#define CH_BMS_TEMP_MASK             (U1W_B4_LOW_TEMP | U1W_B4_HIGH_TEMP | U1W_B4_MOS_HOT)
#define CH_BMS_ERR_MASK              (U1W_B4_OCP | U1W_B4_SHORT | U1W_B4_TIMEOUT | U1W_B4_FAIL)

/*
 * 状态名表：
 * 1. 下标必须与 CH_STATUS_Types 枚举顺序一致。
 * 2. 只用于串口日志显示，不参与状态机判断。
 * 3. 直接索引比“状态值+字符串”的结构表更省 code，也少一次循环比较。
 */
static char * code s_ch_state_name[] =
{
    "空载",
    "检测",
    "等BMS握手",
    "预充",
    "恒流恒压",
    "满电",
    "高压保护",
    "预充超时",
    "高温保护",
    "电流保护",
    "NTC异常",
    "硬件异常",
    "欠压保护",
    "CCCV超时",
    "BMS温度异常",
    "BMS异常",
    "超低压修复",
    "老化",
};

/**
  * @brief  按状态属性表获取中文状态名。
  */
static char *ch_state_name(CH_STATUS_Types state)
{
    if((u8)state < ARRAY_SIZE(s_ch_state_name))
    {
        return s_ch_state_name[(u8)state];
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

    /*
     * 客户端现场仍依赖中文日志判断流程。
     * 这里保留完整信息，但统一为一条 printf：
     * 1. 减少多次 printf 调用和重复格式字符串，降低 C51 代码体积。
     * 2. NTC 正常范围在 int 内，按 s16 打印，避免引入 long printf 格式。
     */
    uart_printf("状态:%s -> %s :%s V=%umV I=%umA NTC=%d\n",
                ch_state_name(ch_state),
                ch_state_name(next_state),
                (reason != 0) ? reason : "无",
                val.vout,
                val.curr,
                (s16)val.i_ntc);

    ch_state = next_state;
}

/**
  * @brief  保护抢占检查。
  * @return 1 表示本周期处于充电器硬件保护态。
  */
static bit ch_check_protect_state(void)
{
    if(ch_flag.ch_ntcErr != 0)
    {
        ch_set_state(NTC_ERR, "NTC开短路");
        return 1;
    }
    else if(ch_flag.ch_hotErr != 0)
    {
        ch_set_state(CH_OTP, "CH高温");
        return 1;
    }
    else if(ch_flag.ch_ovp != 0)
    {
        ch_set_state(CH_OVP, "CH高压");
        return 1;
    }
    else if(ch_flag.ch_ocp != 0)
    {
        ch_set_state(CH_OCP, "CH OCP");
        return 1;
    }

    return 0;
}

/**
  * @brief  获取 CCCV 阶段实际使用的目标电流，单位：mA。
  * @param  target_current_ma  协议和本机限幅后的目标电流。
  *
  * @note   降流条件：
  *         当 B1 上报的最高单节电压 cell_max_mv >= 4.195V 时，
  *         认为电池已经接近满充，需要缓慢降低充电电流。
  *
  * @note   降流速度：
  *         每 0.2 秒最多降低 0.1A，避免在 10ms 任务周期内连续快速降流。
  *
  * @note   电流下限：
  *         最低只降到 iGED，不会降到 0。
  *         iGED 是满电/转灯电流，继续低于该值意义不大。
  *
  * @note   恢复策略：
  *         如果最高单节电压低于 4.195V，本函数只停止继续降流，
  *         不主动升流，避免在阈值附近反复升降造成电流抖动。
  *
  * @retval 本次 CCCV 实际使用的目标电流，单位 mA。
  */
static u16 ch_get_cccv_work_current_ma(u16 target_current_ma)
{
    /*
     * 初次进入 CCCV 时，s_cccv_curr_limit_ma 为 0，
     * 先使用协议/本机限幅后的目标电流。
     *
     * 如果协议目标电流变小，则同步降低当前限流值，
     * 防止动态限流值高于新的协议目标值。
     */
    if((s_cccv_curr_limit_ma == 0U) || (s_cccv_curr_limit_ma > target_current_ma))
    {
        s_cccv_curr_limit_ma = target_current_ma;
        s_cccv_derate_cnt = 0U;
    }

    /*
     * B1 上报最高单节电压达到 4.195V 后开始缓慢降流。
     */
    if(uart_1_wire.cell_max_mv >= (4195U))  /* 4.195V */
    {
        /*
         * 10ms 调用一次，累计 20 次约等于 0.2 秒。
         * 未到时间前不降流。
         */
        if(s_cccv_derate_cnt < 20)
        {
            s_cccv_derate_cnt++;
        }
        else
        {
            s_cccv_derate_cnt = 0U;

            /*
             * 每次降低 100mA，最低限制到 iGED。
             */
            if(s_cccv_curr_limit_ma > (u16)(iGED + 500))
            {
                s_cccv_curr_limit_ma -= 500;
            }
            else
            {
                s_cccv_curr_limit_ma = iGED+100;
            }
        }
    }
    else
    {
        /*
         * 低于 4.195V 时，不继续降流；
         * 也不主动升流，避免电压在阈值附近抖动时电流来回变化。
         */
        s_cccv_derate_cnt = 0U;
    }

    return s_cccv_curr_limit_ma;
}


/**
  * @brief  获取预充结束电压，单位：mV。
  *
  * @note   作用：
  *         用于判断预充阶段什么时候结束，什么时候可以进入 CCCV 阶段。
  *
  * @note   数据来源：
  *         1. 优先使用 A4 协议下发的单节预充截止电压 cell_pre_mv。
  *         2. 根据 A0 协议下发的串数 cell_series，换算成整包预充结束电压。
  *         3. 如果协议数据异常，则回退使用本机默认预充结束电压 vPRE_37V5。
  *
  * @note   保护原则：
  *         1. 串数异常时，使用本机默认串数 BAT_SERIES。
  *         2. 单节预充截止电压异常时，直接回退 vPRE_37V51。
  *         3. 换算后的整包电压超过 SET_vMAX 风险时，直接回退 vPRE_37V5。
  *         4. 换算后的整包电压不能低于本机最低预充阈值 vPRE_30V。
  *
  * @retval 整包预充结束电压，单位 mV。
  */
static u16 ch_get_pre_end_voltage_mv(void)
{
    u8 i;
    u8 series;          /* 电池串数，来自 A0 协议 */
    u16 cell_pre_mv;    /* 单节预充截止电压，单位 mV，来自 A4 协议 */
    u16 pack_mv;        /* 换算后的整包预充结束电压，单位 mV */

    /*
     * 读取电池串数。
     *
     * 正常情况下，串数来自 A0 协议。
     * 为防止协议异常，这里限制有效范围为 5~20 串。
     *
     * 如果 A0 返回的串数为 0、过小或过大，
     * 则使用本机默认串数 BAT_SERIES。
     */
    series = uart_1_wire.cell_series;
    if((series < 5U) || (series > 20U))
    {
        series = BAT_SERIES;
    }

    /*
     * 读取 A4 协议给出的单节预充截止电压。
     *
     * cell_pre_mv 单位：mV。
     *
     * 例如：
     *  2500 表示 2.500V/节
     *  3000 表示 3.000V/节
     *
     * 合理范围限制在：
     *  CELL_REPAIR_MV 2V ~ CELL_FULL_MV 4.2V
     *
     * 如果低于修复电压，说明值太低；
     * 如果高于单节满电电压，说明值太高；
     * 两种情况都认为协议数据异常，回退到本机默认 vPRE_37V5。
     */
    cell_pre_mv = uart_1_wire.cell_pre_mv;
    if((cell_pre_mv < CELL_REPAIR_MV) || (cell_pre_mv > CELL_FULL_MV))
    {
        return vPRE_37V5;
    }

    /*
     * 根据单节预充截止电压和串数，换算整包预充结束电压。
     *
     * 不直接使用：
     *  pack_mv = cell_pre_mv * series;
     *
     * 是为了避免乘法结果异常扩大，也方便在累加过程中判断是否超过 SET_vMAX。
     */
    pack_mv = 0U;
    for(i = 0U; i < series; i++)
    {
        /*
         * 如果继续累加会超过 SET_vMAX，
         * 说明 A4 电压值或 A0 串数存在异常风险。
         *
         * 此时不使用协议计算值，直接回退本机默认 vPRE_37V5。
         */
        if(pack_mv > (u16)(SET_vMAX - cell_pre_mv))
        {
            return vPRE_37V5;
        }

        pack_mv += cell_pre_mv;
    }

    /*
     * 换算后的整包预充结束电压，不能低于本机最低预充阈值 vPRE_30V。
     *
     * 如果低于 vPRE，说明协议给出的预充结束点偏低，
     * 可能导致过早退出预充阶段，因此回退到默认 vPRE_37V5。
     */
    if(pack_mv < vPRE_30V)
    {
        return vPRE_37V5;
    }

    return pack_mv;
}

/**
  * @brief  获取 CCCV 阶段最长充电时间，单位：分钟。
  *
  * @param  target_current_ma
  *         当前准备使用的目标充电电流，单位 mA。
  *
  * @note   计算依据：
  *         1. A0 协议中得到电池并数 cell_parallel。
  *         2. A1 协议中得到单节容量 cell_cap_01ah，单位 0.1Ah。
  *         3. 按：电池包容量 / 充电电流 估算基础充电时间。
  *         4. 最后额外增加 30 分钟余量。
  *
  * @note   协议未定义多个电池包聚合容量，本函数只按：
  *         单节容量 × 并数
  *         来估算电池包容量。
  *
  * @note   为防止异常协议数据导致时间过大：
  *         1. 并数限制为 1~16，异常时按 1 并处理。
  *         2. 单节容量最大限制为 80.0Ah。
  *         3. 电池包总容量最大限制为 80.0Ah。
  *
  * @retval CCCV 最长充电时间，单位：分钟。
  */
static u16 ch_get_cccv_timeout_min(u16 target_current_ma)
{
    u8 i;
    u8 parallel;          /* 电池并数，来自 A0 协议 */
    u16 cell_cap_01ah;    /* 单节容量，单位 0.1Ah，来自 A1 协议 */
    u16 pack_cap_01ah;    /* 按并数估算后的电池包容量，单位 0.1Ah */
    u16 current_100ma;    /* 充电电流换算为 100mA 单位，避免使用浮点 */
    u16 timeout_min;      /* 计算得到的 CCCV 超时时间，单位分钟 */

    /*
     * 目标电流太小，或者还没有获取到电芯容量时，
     * 不进行动态估算，直接使用默认 CCCV 时间。
     */
    if((target_current_ma < 100U) || (uart_1_wire.cell_cap_01ah == 0U))
    {
        return TIM_CCCV;
    }

    /*
     * 读取 A0 协议给出的并数。
     * 正常范围限制为 1~16。
     * 如果协议数据异常，例如 0 或超过 16，则按 1 并处理。
     */
    parallel = uart_1_wire.cell_parallel;
    if((parallel == 0U) || (parallel > 16U))
    {
        parallel = 1U;
    }

    /*
     * 读取 A1 协议给出的单节容量。
     * 单位：0.1Ah。
     *
     * 例如：
     *  50  = 5.0Ah
     *  200 = 20.0Ah
     *  800 = 80.0Ah
     *
     * 这里最大限制为 800，即 80.0Ah，
     * 防止异常协议值导致后面时间计算过大。
     */
    cell_cap_01ah = uart_1_wire.cell_cap_01ah;
    if(cell_cap_01ah > 800U)
    {
        cell_cap_01ah = 800U;
    }

    /*
     * 根据并数累加得到电池包容量。
     *
     * 不直接使用：
     *  pack_cap_01ah = cell_cap_01ah * parallel;
     *
     * 是为了避免乘法结果过大，也方便在累加过程中做上限保护。
     * 最终 pack_cap_01ah 也限制为最大 800，即 80.0Ah。
     */
    pack_cap_01ah = 0U;
    for(i = 0U; i < parallel; i++)
    {
        /*
         * 如果继续累加会超过 800，则直接钳位到 800。
         * 这样可以避免容量异常放大，导致 CCCV 超时时间异常变长。
         */
        if(pack_cap_01ah > (u16)(800U - cell_cap_01ah))
        {
            pack_cap_01ah = 800U;
            break;
        }

        pack_cap_01ah += cell_cap_01ah;
    }

    /*
     * 将目标电流从 mA 换算成 100mA 单位。
     *
     * 例如：
     *  3000mA -> 30
     *  5000mA -> 50
     *
     * 这样后面可以用整数计算：
     *  0.1Ah / 0.1A = 1 小时
     */
    current_100ma = target_current_ma / 100U;
    if(current_100ma == 0U)
    {
        return TIM_CCCV;
    }

    /*
     * 超时时间估算：
     *
     * pack_cap_01ah 单位是 0.1Ah
     * current_100ma 单位是 0.1A
     *
     * pack_cap_01ah / current_100ma 得到的是小时。
     *
     * 理论上转分钟应乘以 60。
     * 这里使用 75，相当于在理论充电时间基础上放大 1.25 倍，
     * 给恒压尾段、电流下降、通信误差、容量误差留出余量。
     */
    timeout_min = (u16)((pack_cap_01ah * 75U) / current_100ma);

    /*
     * 再额外增加 30 分钟保护余量，
     * 防止部分电池尾段时间较长时提前超时。
     */
    timeout_min += 30U;

    /*
     * CCCV 最长时间不能比预充最长时间还短。
     * 如果算出来太小，则至少使用 TIM_PRE。
     */
    if(timeout_min < TIM_PRE)
    {
        timeout_min = TIM_PRE;
    }

    return timeout_min;
}

/**
  * @brief  关闭所有充电输出，进入安全输出状态。
  */
static void ch_output_all_off(void)
{

    DCJK = 0;
    REPAIR_OUTPUT = 0;   // 低预充关闭
    VADJ = 0;
    FAN = 0;
    DUMMY_LOAD = 0;  // 关闭假负载
    set_Curr_Duty(PWMMAX/2);
}

/**
  * @brief  判断BMS是否正在报告充电温度类异常。
  */
static bit ch_bms_temp_fault_active(void)
{
    if((u1w_info.charge_status & CH_BMS_TEMP_MASK) != 0U)
    {
        return 1;
    }

    return 0;
}

static bit ch_bms_status_check(void)
{
    if((u1w_info.charge_status & CH_BMS_ERR_MASK) != 0U)
    {
        ch_set_state(BMS_ERR, "BMS异常");
        return 1;
    }
    if(ch_bms_temp_fault_active() != 0)
    {
        ch_set_state(BMS_TEMP_ERR, "BMS温度异常");
        return 1;
    }
    if((u1w_info.charge_status & U1W_B4_OV) != 0U)
    {
        ch_set_state(CH_FULL, "BMS满电");
        return 1;
    }

    return 0;
}

/**
  * @brief  判断当前是否属于继电器闭合的正常充电阶段。
  * @note   这些状态拔电池时不能用电压判断，因为 val.vout 可能是充电器自身输出。
  */
static bit ch_relay_charge_state(void)
{
    if((ch_state == CH_Pre1) || (ch_state == CH_CCCV))
    {
        return 1;
    }

    return 0;
}

/**
  * @brief  满电/异常锁止状态是否需要间歇打开电压检测。
  */
static bit ch_need_vout_probe(void)
{
    switch(ch_state)
    {
    case CH_FULL:
    case BMS_TEMP_ERR:
    case BMS_ERR:
    case CH_OTP:
    case CH_TimOut:
    case CCCV_TimOut:
    case CH_UVP:
    case CH_OVP:
    case CH_OCP:
    case NTC_ERR:
    case HW_ERR:
        return 1;

    default:
        return 0;
    }
}

static void ch_vout_probe_reset(void)
{
    s_vout_probe_period_10ms = CH_VOUT_PROBE_PERIOD_10MS;
    s_vout_probe_on_10ms = 0U;
    s_vout_sample_valid = 0;
}

/**
  * @brief  每10ms准备一次电池电压采样使能。
  * @note   正常充电相关状态常开；满电/异常状态间歇开窗，避免分压长期放电。
  */
static void ch_prepare_vout_sample_10ms(void)
{
    s_vout_sample_valid = 0;

    if((ch_state == CH_IDLE) ||
       (ch_state == BMS_HANDSHAKE) ||
       (ch_state == CH_Check) ||
       (ch_state == CH_REPAIR) ||
       (ch_state == CH_Pre1) ||
       (ch_state == CH_CCCV) ||
       (ch_state == CH_AGING))
    {
        BATT_DIVIDER_EN = 1;
        s_vout_sample_valid = 1;
        s_vout_probe_period_10ms = 0U;
        s_vout_probe_on_10ms = 0U;
        return;
    }

    if(ch_need_vout_probe() == 0)
    {
        BATT_DIVIDER_EN = 0;
        return;
    }

    if(s_vout_probe_on_10ms != 0U)
    {
        BATT_DIVIDER_EN = 1;

        if(s_vout_probe_on_10ms >= CH_VOUT_PROBE_VALID_10MS)
        {
            s_vout_sample_valid = 1;
        }

        s_vout_probe_on_10ms++;
        if(s_vout_probe_on_10ms > CH_VOUT_PROBE_ON_10MS)
        {
            s_vout_probe_on_10ms = 0U;
            s_vout_probe_period_10ms = 0U;
            BATT_DIVIDER_EN = 0;
            s_vout_sample_valid = 0;
        }
        return;
    }

    BATT_DIVIDER_EN = 0;
    if(s_vout_probe_period_10ms < CH_VOUT_PROBE_PERIOD_10MS)
    {
        s_vout_probe_period_10ms++;
    }
    else
    {
        s_vout_probe_on_10ms = 1U;
        BATT_DIVIDER_EN = 1;
    }
}

/**
  * @brief  除待机外，全局判断电池是否拔出。
  * @return 1 表示本周期由拔除检测接管。
  */
static bit ch_battery_removed_check_10ms(void)
{
    u8 need_cnt;

    if(ch_state == CH_IDLE)
    {
        s_remove_cnt = 0U;
        return 0;
    }

    if(ch_relay_charge_state() != 0)
    {
        /*
         * 继电器闭合时，val.vout 可能是充电器自身输出，不能作为拔电池依据。
         * 1秒无合法通信帧且电流小于满电电流后，先关输出，约20ms后再看电池端电压。
         */
        if(s_remove_cnt != 0U)
        {
            ch_output_all_off();
            if(++s_remove_cnt >= 3U)
            {
                if(val.vout < vRESET)
                {
                    BATT_DIVIDER_EN = 0;
                    uart_1_wire_reset_link();
                    ch_vout_probe_reset();
                    ch_set_state(CH_IDLE, "拔电池");
                    return 1;
                }
                s_remove_cnt = 0U;
                ch_set_state(BMS_ERR, "拔出异常");
                return 1;
            }
            return 1;
        }

        if((val.curr < iGED) && (u1w_info.no_rx_10ms >= 100U))
        {
            ch_output_all_off();
            s_remove_cnt = 1U;
            return 1;
        }
        return 0;
    }

    if((s_vout_sample_valid != 0) && (val.vout < vRESET))
    {
        if(ch_need_vout_probe() != 0)
        {
            need_cnt = 2U;      /* 间歇检测窗口下，连续2次有效窗口确认拔出。 */
        }
        else
        {
            need_cnt = 50U;     /* 常开检测下，约500ms确认拔出。 */
        }

        if(++s_remove_cnt >= need_cnt)
        {
            ch_output_all_off();
            BATT_DIVIDER_EN = 0;
            uart_1_wire_reset_link();
            ch_vout_probe_reset();
            ch_set_state(CH_IDLE, "拔电池");
            return 1;
        }
    }
    else if(s_vout_sample_valid != 0)
    {
        s_remove_cnt = 0U;
    }

    return 0;
}




void usr_ch_func(void)
{
    u16 target_voltage_mv;
    u16 target_current_ma;
    u16 pre_end_voltage_mv;
    u16 cccv_timeout_min;
    u8 u1w_stage;

    ch_state = CH_IDLE;
    last_state = CH_IDLE;

    next_10ms = timer_deadline_ms(TASK_10MS);
    uart_1_wire_reset_link();
    uart_printf("充电程序\n");

    while(flg_cal_mode == 0)
    {
        if(timer_period_elapsed(&next_10ms, TASK_10MS) != 0)
        {
            wdt_feed();

            /* DEBUG 口收到 *RST 后切入校准流程，保持 54.6V 行为。 */
            if(pc_uart_func() == 1U)
            {
                flg_cal_mode = 1;
                break;
            }

            /*
             * 一线通信阶段由充电流程直接决定：
             * - 待机：释放 COM，不通信；
             * - BMS_HANDSHAKE：轮询握手帧；
             * - 正常充电：轮询 B1/B3/B4/B6；
             * - 满电：按协议发送 B6 03 SOC，3 分钟后主动拉低 COM；
             * - BMS温度异常：继续轮询 B3/B4 等待恢复；
             * - 普通异常：主动拉低 COM，告知 BMS 主机断开。
             */
            switch(ch_state)
            {
            case BMS_HANDSHAKE:
                u1w_stage = U1W_STAGE_HANDSHAKE;
                break;

            case CH_Check:
            case CH_REPAIR:
            case CH_Pre1:
            case CH_CCCV:
                u1w_stage = U1W_STAGE_CHARGE;
                break;

            case CH_FULL:
                u1w_stage = U1W_STAGE_FULL_DISPLAY;
                break;

            case BMS_TEMP_ERR:
                u1w_stage = U1W_STAGE_TEMP_WAIT;
                break;

            case BMS_ERR:
            case CH_OTP:
            case CH_TimOut:
            case CCCV_TimOut:
            case CH_UVP:
            case CH_OVP:
            case CH_OCP:
            case NTC_ERR:
            case HW_ERR:
                u1w_stage = U1W_STAGE_PULL_LOW;
                break;

            default:
                u1w_stage = U1W_STAGE_STOP;
                break;
            }
            uart_1_wire_set_stage(u1w_stage);
            uart_1_wire_poll_10ms();

            /* 先按当前状态准备电压采样使能，再更新ADC工程量。 */
            ch_prepare_vout_sample_10ms();

            /* 每 10ms 更新 ADC 工程量和保护标志。 */
            adc_sample_all();
            ch_err_ck();

            /* 充电器自身保护优先级高于BMS保护。 */
            if(ch_check_protect_state() != 0)
            {
                ch_output_all_off();
            }

            if(ch_battery_removed_check_10ms() != 0)
            {
                continue;
            }

            target_voltage_mv = u1w_info.target_voltage_mv;
            if(target_voltage_mv > SET_vMAX)
            {
                target_voltage_mv = SET_vMAX;
            }

            target_current_ma = u1w_info.target_current_ma;
            if(target_current_ma > iMAX)
            {
                target_current_ma = iMAX;
            }
            if(last_state != ch_state)
            {
                last_state = ch_state;
                s_cut[0] = 0;
                s_cut[1] = 0;
                s_cut[2] = 0;
                s_cut[3] = 0;
                s_remove_cnt = 0U;
                ch_vout_probe_reset();
                Tim.ms = 0;
                Tim.s = 0;
                Tim.min = 0;

                if(ch_state == CH_CCCV)
                {
                    s_cccv_curr_limit_ma = 0U;
                    s_cccv_derate_cnt = 0U;
                }
            }

//            ch_state = CH_AGING;

            switch(ch_state)
            {
            default:
            case CH_IDLE:
                /*
                 * 空载/待机：
                 * - 继电器、VADJ、PWM、风扇全部关闭；
                 * - 电压超过起充阈值后进入BMS握手；
                 * - 1V~15V 需稳定确认，避免残压或插入瞬态误判低压。
                 */
                ch_output_all_off();
                RLED = 1;
                GLED = 0;
                BATT_DIVIDER_EN = 1;          /* 打开电池分压，保证 AIN3 采样有效。 */
                if(val.vout >= vSTART)
                {
                    s_cut[1] = 0U;
                    s_idle_low_last_mv = 0U;
                    if(++s_cut[0] >= 50U)
                    {
                        uart_1_wire_reset_link();
                        ch_set_state(BMS_HANDSHAKE, "插电池");
                    }
                }
                else if(val.vout > vRESET)
                {
                    s_cut[0] = 0U;
                    if(s_cut[1] == 0U)
                    {
                        s_idle_low_last_mv = val.vout;
                        s_cut[1] = 1U;
                    }
                    else
                    {
                        if(((val.vout > s_idle_low_last_mv) &&
                            ((u16)(val.vout - s_idle_low_last_mv) > 100U)) ||
                           ((s_idle_low_last_mv > val.vout) &&
                            ((u16)(s_idle_low_last_mv - val.vout) > 100U)))
                        {
                            s_cut[1] = 1U;
                        }
                        else if(s_cut[1] < 200U)
                        {
                            s_cut[1]++;
                        }

                        s_idle_low_last_mv = val.vout;
                        if(s_cut[1] >= 200U)
                        {
                            ch_set_state(CH_UVP, "电池低压");
                        }
                    }
                }
                else
                {
                    s_cut[0] = 0U;
                    s_cut[1] = 0U;
                    s_idle_low_last_mv = 0U;
                }
                break;

            case BMS_HANDSHAKE:
                /*
                 * 等待握手成功：
                 * - 输出保持关闭；
                 * - 等待协议层完成 A0/A1/A4/A6/A7/B1/B3/B4；
                 * - 通信超时进入 BMS_ERR；
                 * - 握手成功后才进入 CH_Check，由 CH_Check 决定修复/预充/CCCV。
                 */
                ch_output_all_off();
                RLED = 1;
                GLED = 0;

                TimCut();
                if(Tim.s >= BMS_HANDSHAKE_TIMEOUT_S)
                {
                    ch_set_state(BMS_ERR, "握手超时");
                }
                else if(val.vout < vSTART)
                {
                    if(++s_cut[0] >= 50U)
                    {
                        ch_set_state(CH_UVP, "握手低压");
                    }
                }
                else
                {
                    s_cut[0] = 0U;

                    if(u1w_info.comm_timeout != 0U)
                    {
                        ch_set_state(BMS_ERR, "BMS通信超时");
                    }
                    else if(u1w_info.handshake_ok != 0U)
                    {
                        pc_uart_print_batt();
                        ch_set_state(CH_Check, "握手成功");
                    }
                    else
                    {
                        /* 继续等待 A0/A1/A4/A6/A7/B1/B3/B4 收齐。 */
                    }
                }
                break;

            case CH_Check:
                /*
                 * 检测状态：
                 * - 此状态默认已经完成BMS握手；
                 * - 输出保持关闭；
                 * - 若通信被复位或尚未在线，则退回BMS_HANDSHAKE等待；
                 * - 通信正常后，再根据电压进入修复、预充或CCCV。
                 */
                ch_output_all_off();
                RLED = 1;
                GLED = 0;

                if(val.vout < vSTART)
                {
                    if(++s_cut[0] >= 50U)
                    {
                        ch_set_state(CH_UVP, "电池低压");
                    }
                }
                else
                {
                    /*
                     * 电压已恢复到起充阈值以上，清掉欠压确认计数。
                     * 避免前面临界波动累计的s_cut[0]残留，后续再次波动时误进欠压。
                     */
                    s_cut[0] = 0U;
                    pre_end_voltage_mv = ch_get_pre_end_voltage_mv();

                    if(u1w_info.comm_timeout != 0U)
                    {
                        ch_set_state(BMS_ERR, "BMS通信超时");
                    }
                    else if(ch_bms_status_check() != 0)
                    {
                        /* 状态已切换。 */
                    }
                    else if(u1w_info.handshake_ok == 0U)
                    {
                        uart_1_wire_reset_link();
                        ch_set_state(BMS_HANDSHAKE, "等握手");
                    }
                    else if(val.vout < vPRE_30V)
                    {
                        ch_set_state(CH_REPAIR, "通信OK进修复");
                    }
                    else if(val.vout < pre_end_voltage_mv)
                    {
                        ch_set_state(CH_Pre1, "通信OK进预充");
                    }
                    else
                    {
                        ch_set_state(CH_CCCV, "通信OK进CCCV");
                    }
                }
                break;

            case CH_REPAIR:
                /*
                 * 超低压修复：
                 * - 继电器断开，只打开修复输出；
                 * - 电流使用 iREPAIR，保持小电流修复；
                 * - 到 vPRE_30V 后转入预充，超时进入预充超时异常。
                 */
                if(u1w_info.comm_timeout != 0U)
                {
                    ch_set_state(BMS_ERR, "BMS通信超时");
                    break;
                }
                if(ch_bms_status_check() != 0)
                {
                    break;
                }
                DCJK = 0;
                VADJ = 0;
                if(DCJK == 0)
                {
                    REPAIR_OUTPUT = 1;
                }
                FAN = 1;
                Ged_Flash(50);
                TimCut();
                set_Curr_Duty(SET_CURR(iREPAIR));

                if(Tim.min >= TIM_PRE)
                {
                    ch_set_state(CH_TimOut, "修复超时");
                }
                else if(val.vout >= vPRE_30V)
                {
                    if(++s_cut[0] >= 50U)
                    {
                        ch_set_state(CH_Pre1, "修复完成");
                    }
                }
                else
                {
                    s_cut[0] = 0;
                }
                break;

            case CH_Pre1:
                /*
                 * 预充：
                 * - 正常预充时持续检查通信超时；
                 * - B4 状态位按满电、温度异常、普通异常分类处理。
                 */
                if(u1w_info.comm_timeout != 0U)
                {
                    ch_set_state(BMS_ERR, "BMS通信超时");
                    break;
                }
                if(ch_bms_status_check() != 0)
                {
                    break;
                }

                /*
                 * 异步通信版只在这里处理通信超时。
                 * B4 状态位按满电、温度异常、普通异常分类处理。
                 */
                DCJK = 1;
                REPAIR_OUTPUT = 0;
                VADJ = 1;
                FAN = 1;
                Ged_Flash(50);
                TimCut();
                set_Curr_Duty(SET_CURR(iPRE));
                pre_end_voltage_mv = ch_get_pre_end_voltage_mv();

                if(Tim.min >= TIM_PRE)
                {
                    ch_set_state(CH_TimOut, "预充超时");
                }
                else if(val.vout >= pre_end_voltage_mv)
                {
                    if(++s_cut[0] >= 50U)
                    {
                        ch_set_state(CH_CCCV, "预充完成");
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
                 * - 充电中持续检查通信超时和B4状态。
                 */
                if(u1w_info.comm_timeout != 0U)
                {
                    ch_set_state(BMS_ERR, "BMS通信超时");
                    break;
                }
                if(ch_bms_status_check() != 0)
                {
                    break;
                }

                /*
                 * 异步通信版只在这里处理通信超时。
                 * B4 状态位按满电、温度异常、普通异常分类处理。
                 */
                DCJK = 1;
                REPAIR_OUTPUT = 0;
                VADJ = 1;
                FAN = 1;
                RLED = 0;
                Ged_Flash(50);
                TimCut();

                /*
                 * 最高单节电压达到 4.195V 后，CCCV 电流每 0.2 秒最多降低 0.1A。
                 * 该限流值只在本次 CCCV 阶段内保持，重新进入 CCCV 后会重新初始化。
                 */
                target_current_ma = ch_get_cccv_work_current_ma(target_current_ma);
                set_Curr_Duty(SET_CURR(target_current_ma));
                cccv_timeout_min = ch_get_cccv_timeout_min(target_current_ma);

                if(Tim.min >= cccv_timeout_min)
                {
                    ch_set_state(CCCV_TimOut, "CCCV超时");
                }
                else if((val.vout >= target_voltage_mv) && (val.curr <= iGED))
                {
                    if(++s_cut[0] >= 100U)
                    {
                        ch_set_state(CH_FULL, "转满电");
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
                 * - 按协议发送 B6 03 SOC，3分钟后由通信层主动拉低COM；
                 * - FULL_DISPLAY 不再轮询B1，回充暂用整包 vCH60 近似判断。
                 */
                ch_output_all_off();
                RLED = 0;
                GLED = 1;
                if((s_vout_sample_valid != 0) && (val.vout < vCH60))
                {
                    if(++s_cut[0] >= 2U)
                    {
                        uart_1_wire_reset_link();
                        ch_set_state(BMS_HANDSHAKE, "满电回落重新握手");
                    }
                }
                else if(s_vout_sample_valid != 0)
                {
                    s_cut[0] = 0;
                }
                break;

            case BMS_TEMP_ERR:
                /*
                 * BMS温度异常：
                 * - 输出关闭；
                 * - 不主动拉低COM；
                 * - 继续轮询B3/B4，等BMS温度状态恢复后重新握手。
                 */
                ch_output_all_off();
                RGed_Flash(50);
                if(u1w_info.comm_timeout != 0U)
                {
                    ch_set_state(BMS_ERR, "温度通信超时");
                }
                else if((u1w_info.charge_status & CH_BMS_ERR_MASK) != 0U)
                {
                    ch_set_state(BMS_ERR, "BMS异常");
                }
                else if(ch_bms_temp_fault_active() == 0)
                {
                    uart_1_wire_reset_link();
                    ch_set_state(BMS_HANDSHAKE, "BMS温度恢复");
                }
                break;

            case BMS_ERR:
                /*
                 * BMS通信异常：
                 * - 输出关闭；
                 * - 主动拉低COM，告知BMS主机断开；
                 * - 拔电池由全局拔出检测统一处理。
                 */
                ch_output_all_off();
                Red_Flash(50);
                break;

            case CH_OTP:
                /*
                 * 高温保护：温度恢复后回检测状态。
                 */
                ch_output_all_off();
                RGed_Flash(50);
                if(ch_flag.ch_hotErr == 0)
                {
                    uart_1_wire_reset_link();
                    ch_set_state(BMS_HANDSHAKE, "OTP恢复重新握手");
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
                break;

            case NTC_ERR:
            case HW_ERR:
                /*
                 * NTC/硬件异常：输出关闭，等待电压释放或人工处理。
                 */
                ch_output_all_off();
                RGed_Flash(50);
                break;

            case CH_AGING:
                /*
                 * 老化模式预留。
                 */
                DCJK = 1;
                VADJ = 1;
                FAN = 1;
                set_Curr_Duty(SET_CURR(iMAX));
                Ged_Flash(50);
                break;
            }
        }
    }
}
