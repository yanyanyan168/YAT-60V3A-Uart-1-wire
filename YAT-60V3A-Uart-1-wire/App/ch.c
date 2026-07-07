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

static u16 idata s_cut[4];          /* 状态内确认计数，防止临界点抖动。 */

static u16 idata s_cccv_curr_limit_ma;        /* CCCV 实际限流电流，单位 mA。 */
static u8  idata s_cccv_derate_cnt;           /* CCCV 降流间隔计数。 */
static u8  idata s_remove_cnt;                /* 拔电池确认计数。 */
static u16 idata s_no_current_cnt;             /* 充电中有压无流异常确认计数。 */
static u16 idata s_vout_probe_period_10ms;    /* 满电/异常时分压检测间隔计数。 */
static u8  idata s_vout_probe_on_10ms;        /* 满电/异常时分压检测开窗计数。 */
static bit s_full_recharge;                  /* 满电回充复检阶段保持满电显示。 */
u16 idata s_dummy_load_10ms;                   /* DUMMY_LOAD hold counter, unit 10ms. */

u16 idata cccv_timeout_min;


u16 idata pack_uvp_mv;                    // 电池包欠压保护阈值
u16 idata pack_repair_mv;                 // 电池包欠压修复成功阈值
u16 idata pack_pre_to_cc_mv;              // 预充转恒流阈值
u16 idata pack_poweron_full_mv;           // 上电满电判断阈值
u16 idata pack_recharge_mv;               // 回充电压阈值

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
    uart_printf("状态:%s -> %s :%s V=%umV I=%umA\n",
                ch_state_name(ch_state),
                ch_state_name(next_state),
                (reason != 0) ? reason : "无",
                val.vout,
                val.curr);

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



// 获取电池包电压阈值，单位：mV。
static u16 ch_get_pack_mv(u16 cell_mv)
{
    u8 series;

    series = uart_1_wire.cell_series;
    if((series < 5U) || (series > 20U))
    {
        series = BAT_SERIES;
    }

    return (u16)(cell_mv * (u16)series);
}

static void ch_set_wait_led(void)
{
    if(s_full_recharge != 0)
    {
        RLED = 0;
        GLED = 1;
    }
    else
    {
        RLED = 1;
        GLED = 0;
    }
}


/**
  * @brief  获取预充结束电压，单位：mV。
  *
  * @note   作用：
  *         用于判断预充阶段什么时候结束，什么时候可以进入 CCCV 阶段。
  *
  * @note   数据来源：
  *         1. 优先使用 A4 协议下发的单节预充截止电压 CELL_2500MV。
  *         2. 根据 A0 协议下发的串数 cell_series，换算成整包预充结束电压。
  *         3. 如果协议数据异常，则回退使用 2.5V*N。
  *
  * @note   保护原则：
  *         1. 串数异常时，使用本机默认串数 BAT_SERIES。
  *         2. 单节预充截止电压异常时，直接回退 2.5V*N。
  *         3. 换算后的整包电压超过 SET_vMAX 风险时，直接回退 2.5V*N。
  *         4. 换算后的整包电压不能低于 2.0V*N。
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
     *  CELL_2000MV 2V ~ CELL_FULL_MV 4.2V
     *
     * 如果低于修复电压，说明值太低；
     * 如果高于单节满电电压，说明值太高；
     * 两种情况都认为协议数据异常，回退到 2.5V*N。
     */
    cell_pre_mv = uart_1_wire.cell_pre_mv;
    if((cell_pre_mv < CELL_2000MV) || (cell_pre_mv > CELL_4200MV))
    {
        return pack_pre_to_cc_mv;
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
         * 此时不使用协议计算值，直接回退 2.5V*N。
         */
        if(pack_mv > (u16)(SET_vMAX - cell_pre_mv))
        {
            return pack_pre_to_cc_mv;
        }

        pack_mv += cell_pre_mv;
    }

    /*
     * 换算后的整包预充结束电压，不能低于 2.0V*N。
     *
     * 如果低于 2.0V*N，说明协议给出的预充结束点偏低，
     * 可能导致过早退出预充阶段，因此回退到 2.5V*N。
     */
    if(pack_mv < pack_repair_mv)
    {
        return pack_pre_to_cc_mv;
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
void ch_get_cccv_timeout_min(void)
{
    u8 i;
    u8 parallel;          /* 电池并数，来自 A0 协议 */
    u16 cell_cap_01ah;    /* 单节容量，单位 0.1Ah，来自 A1 协议 */
    u16 pack_cap_01ah;    /* 按并数估算后的电池包容量，单位 0.1Ah */
    u16 current_100ma;    /* 充电电流换算为 100mA 单位，避免使用浮点 */

    /*
     * 目标电流太小，或者还没有获取到电芯容量时，
     * 不进行动态估算，直接使用默认 CCCV 时间。
     */
    if(( uart_1_wire.target_current_ma < 100U) || (uart_1_wire.cell_cap_01ah == 0U))
    {
        cccv_timeout_min = TIM_CCCV;
        return;
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
    cccv_timeout_min =  uart_1_wire.target_current_ma / 100U;
    if(cccv_timeout_min == 0U)
    {
        cccv_timeout_min = TIM_CCCV;
        return;
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
    cccv_timeout_min = (u16)((pack_cap_01ah * 75U) / current_100ma);

    /*
     * 再额外增加 30 分钟保护余量，
     * 防止部分电池尾段时间较长时提前超时。
     */
    cccv_timeout_min += 30U;
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
    DUMMY_LOAD = 0;       // 关闭假负载
    BATT_DIVIDER_EN = 0;  // 满电/异常默认关闭电池分压，只在检测窗口短时打开，降低漏电和倒灌风险
    set_Curr_Duty(PWMMAX/2);
}

/**
  * @brief  判断BMS是否正在报告充电温度类异常。
  */
static bit ch_bms_temp_fault_active(void)
{
    if((uart_1_wire.charge_status & CH_BMS_TEMP_MASK) != 0U)
    {
        return 1;
    }

    if(uart_1_wire.cell_type == U1W_CELL_TYPE_21700)
    {
        if((uart_1_wire.batt_temp_degc < BATT_21700_CHG_TEMP_LOW_C) ||
           (uart_1_wire.batt_temp_degc > BATT_21700_CHG_TEMP_HIGH_C))
        {
            return 1;
        }
    }
    else
    {
        if((uart_1_wire.batt_temp_degc < BATT_18650_CHG_TEMP_LOW_C) ||
           (uart_1_wire.batt_temp_degc > BATT_18650_CHG_TEMP_HIGH_C))
        {
            return 1;
        }
    }

    return 0;
}

static bit ch_bms_temp_recovered(void)
{
    if((uart_1_wire.charge_status & CH_BMS_TEMP_MASK) != 0U)
    {
        return 0;
    }

    if(uart_1_wire.cell_type == U1W_CELL_TYPE_21700)
    {
        if((uart_1_wire.batt_temp_degc >= BATT_21700_REC_TEMP_LOW_C) &&
           (uart_1_wire.batt_temp_degc <= BATT_21700_REC_TEMP_HIGH_C))
        {
            return 1;
        }
    }
    else
    {
        if((uart_1_wire.batt_temp_degc >= BATT_18650_REC_TEMP_LOW_C) &&
           (uart_1_wire.batt_temp_degc <= BATT_18650_REC_TEMP_HIGH_C))
        {
            return 1;
        }
    }

    return 0;
}

static bit ch_bms_status_check(void)
{
    if((uart_1_wire.charge_status & CH_BMS_ERR_MASK) != 0U)
    {
        ch_set_state(BMS_ERR, "BMS异常");
        return 1;
    }
    if(ch_bms_temp_fault_active() != 0)
    {
        ch_set_state(BMS_TEMP_ERR, "BMS温度异常");
        return 1;
    }
    if((uart_1_wire.charge_status & U1W_B4_OV) != 0U)
    {
        ch_set_state(CH_FULL, "BMS满电");
        return 1;
    }

    return 0;
}

/**
  * @brief  充电中有输出电压但长期无电流，判为充电器/回路异常。
  * @note   只在 CH_Pre1/CH_CCCV 的 case 中调用；接近满电时由调用处允许跳过误判。
  */
static bit ch_no_current_fault_check_10ms(u16 target_voltage_mv, u16 target_current_ma, u8 full_margin_en)
{
    u16 no_current_ma;

    if((uart_1_wire.comm_timeout != 0U) || (uart_1_wire.no_rx_10ms >= 100U))  /* 1秒无一线帧时交给拔电池/通信异常判断 */
    {
        s_no_current_cnt = 0U;
        return 0;
    }

    if(((u16)Tim.s * 100U + (u16)Tim.ms) < 100U)  /* 充电开始1秒内不判 */
    {
        s_no_current_cnt = 0U;
        return 0;
    }

    if(target_current_ma == 0U)  /* 没有目标电流时，不做有压无流判断 */
    {
        s_no_current_cnt = 0U;
        return 0;
    }

    if(target_current_ma <= 200U)
    {
        no_current_ma = 30U;     /* 修复小电流 */
    }
    else
    {
        no_current_ma = 100U;    /* 预充/CCCV */
    }

    if((full_margin_en != 0U) && (((u32)val.vout + 500U) >= target_voltage_mv))  /* 距目标电压500mV内，按满电末端处理 */
    {
        s_no_current_cnt = 0U;
        return 0;
    }

    if((val.vout > pack_uvp_mv) && (val.curr < no_current_ma))
    {
        if(++s_no_current_cnt >= 100U)                          /* 连续1秒有压无流 */
        {
            ch_output_all_off();
            s_no_current_cnt = 0U;
            ch_set_state(HW_ERR, "有压无流");
            return 1;
        }
    }
    else
    {
        s_no_current_cnt = 0U;
    }

    return 0;
}



void usr_ch_func(void)
{
    u16 target_voltage_mv;
    u16 pre_end_voltage_mv;
    u16 target_current_ma;
    u8 vout_valid;

    ch_state = CH_IDLE;
    last_state = CH_IDLE;

    next_10ms = timer_deadline_ms(TASK_10MS);
    uart_1_wire_reset_link();
    uart_printf("充电流程启动\n");

    while(flg_cal_mode == 0)
    {
        if(timer_period_elapsed(&next_10ms, TASK_10MS) != 0)
        {
            wdt_feed();

            if(pc_uart_func() == 1U)  /* DEBUG收到*RST，退出充电流程进入校准 */
            {
                flg_cal_mode = 1;
                break;
            }

            target_voltage_mv = uart_1_wire.target_voltage_mv;
            if(target_voltage_mv > SET_vMAX)
            {
                target_voltage_mv = SET_vMAX;
            }

            target_current_ma = uart_1_wire.target_current_ma;
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
                s_no_current_cnt = 0U;
                s_vout_probe_period_10ms = 100U;  /* 满电/异常进入后尽快打开一次分压检测 */
                s_vout_probe_on_10ms = 0U;
                Tim.ms = 0;
                Tim.s = 0;
                Tim.min = 0;

                if((ch_state != BMS_HANDSHAKE) && (ch_state != CH_Check))
                {
                    s_full_recharge = 0;
                }

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
                 * 待机：输出全关，分压常开看电池接入。
                 * 欠压阈值在握手/检查/预充阶段按串数动态判断。
                 */
                s_dummy_load_10ms = 500U;
                uart_1_wire_set_stage(U1W_STAGE_STOP);
                uart_1_wire_poll_10ms();
                ch_output_all_off();
                BATT_DIVIDER_EN = 1;
                adc_sample_all();
                ch_err_ck();
                if(ch_check_protect_state() != 0)
                {
                    ch_output_all_off();
                    break;
                }

                RLED = 1;
                GLED = 0;
                if(val.vout > vRESET)
                {
                    if(++s_cut[0] >= 100U)        /* 1000ms确认电池已接入 */
                    {
                        uart_1_wire_reset_link();
                        ch_set_state(BMS_HANDSHAKE, "插电池");
                    }
                }
                else 
                {
                    s_cut[0] = 0U;
                }
                break;

            case BMS_HANDSHAKE:
                /* 握手：输出关闭，分压常开，等待A0/A1/A4/A6/A7/B1/B3/B4。 */
                s_dummy_load_10ms = 500U;
                uart_1_wire_set_stage(U1W_STAGE_HANDSHAKE);
                uart_1_wire_poll_10ms();
                ch_output_all_off();
                BATT_DIVIDER_EN = 1;
                adc_sample_all();
                ch_err_ck();
                if(ch_check_protect_state() != 0)
                {
                    ch_output_all_off();
                    break;
                }

                ch_set_wait_led();
                TimCut();
                if(Tim.s >= 20U)                 /* 握手最多等待20秒 */
                {
                    ch_set_state(BMS_ERR, "握手超时");
                }
                else if(val.vout < pack_uvp_mv)
                {
                     ch_set_state(CH_UVP, "握手低压");
                }
                else
                {
                    if(uart_1_wire.comm_timeout != 0U)
                    {
                        ch_set_state(BMS_ERR, "BMS通信超时");
                    }
                    else if(uart_1_wire.handshake_ok != 0U)
                    {
                        pack_uvp_mv          = ch_get_pack_mv(CELL_1000MV);
                        pack_repair_mv       = ch_get_pack_mv(CELL_2000MV);
                        pack_pre_to_cc_mv    = ch_get_pack_mv(CELL_2500MV);
                        pack_poweron_full_mv = ch_get_pack_mv(CELL_4100MV);
                        pack_recharge_mv     = ch_get_pack_mv(CELL_4000MV);
                        ch_get_cccv_timeout_min();
                        pc_uart_print_batt();
                        
                        if(val.vout > pack_poweron_full_mv )
                        {
                            ch_set_state(CH_FULL, "转满电");
                        }
                        else
                        {
                            ch_set_state(CH_Check, "握手成功");
                        }
                    }
                }
                break;

            case CH_Check:
                /* 检查：输出关闭，分压常开，根据电池电压进入修复/预充/CCCV。 */
                s_dummy_load_10ms = 500U;
                uart_1_wire_set_stage(U1W_STAGE_CHARGE);
                uart_1_wire_poll_10ms();
                ch_output_all_off();
                BATT_DIVIDER_EN = 1;
                adc_sample_all();
                ch_err_ck();
                if(ch_check_protect_state() != 0)
                {
                    ch_output_all_off();
                    break;
                }

                ch_set_wait_led();
                if(val.vout < pack_uvp_mv)
                {
                    if(++s_cut[0] >= 50U)        /* 500ms确认电池低压 */
                    {
                        ch_set_state(CH_UVP, "电池低压");
                    }
                }
                else
                {
                    s_cut[0] = 0U;
                    pre_end_voltage_mv = ch_get_pre_end_voltage_mv();

                    if(uart_1_wire.comm_timeout != 0U)
                    {
                        ch_set_state(BMS_ERR, "BMS通信超时");
                    }
                    else if(ch_bms_status_check() != 0)
                    {
                        /* BMS状态函数已完成状态切换。 */
                    }
                    else if(uart_1_wire.handshake_ok == 0U)
                    {
                        uart_1_wire_reset_link();
                        ch_set_state(BMS_HANDSHAKE, "重新握手");
                    }
                    else if(val.vout < pack_repair_mv)
                    {
                        ch_set_state(CH_REPAIR, "通信OK，修复");
                    }
                    else if(val.vout < pre_end_voltage_mv)
                    {
                        ch_set_state(CH_Pre1, "通信OK，预充");
                    }
                    else
                    {
                        ch_set_state(CH_CCCV, "通信OK，CCCV");
                    }
                }
                break;

            case CH_REPAIR:
                /* 低压修复：继电器断开，只开修复小电流，到2.0V*N后转预充。 */
                s_dummy_load_10ms = 500U;
                uart_1_wire_set_stage(U1W_STAGE_CHARGE);
                uart_1_wire_poll_10ms();
                BATT_DIVIDER_EN = 1;
                adc_sample_all();
                ch_err_ck();
                if(ch_check_protect_state() != 0)
                {
                    ch_output_all_off();
                    break;
                }
                if(uart_1_wire.comm_timeout != 0U)
                {
                    ch_set_state(BMS_ERR, "BMS通信超时");
                    break;
                }
                if(ch_bms_status_check() != 0)
                {
                    break;
                }

                DCJK = 0;
                REPAIR_OUTPUT = 1;
                VADJ = 0;
                FAN = 0;
                Ged_Flash(100);
                TimCut();
                set_Curr_Duty(PWMMAX/2);       /* Keep primary/opto current loop alive; not repair current control. */
                if(Tim.min >= TIM_PRE)
                {
                    ch_set_state(CH_TimOut, "修复超时");
                }
                else if(val.vout >= pack_repair_mv)
                {
                    if(++s_cut[0] >= 300U)        /* 3000ms确认修复完成 */
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
                 * 预充：继电器闭合、分压常开。
                 * 这里直接处理充电中拔电池和有压无流，避免外部状态判断分散。
                 */
                s_dummy_load_10ms = 500U;
                uart_1_wire_set_stage(U1W_STAGE_CHARGE);
                uart_1_wire_poll_10ms();

                BATT_DIVIDER_EN = 1;
                adc_sample_all();
                ch_err_ck();
                if(val.vout < vRESET)
                {
                    uart_1_wire_reset_link();
                    ch_set_state(CH_IDLE, "拔电池");
                    break;
                }
                if(ch_check_protect_state() != 0)
                {
                    ch_output_all_off();
                    break;
                }

                if(ch_no_current_fault_check_10ms(target_voltage_mv, iPRE, 0U) != 0)  /* 预充实际按 iPRE 输出，不能因协议目标电流偏小而漏判 */
                {
                    break;
                }
                if(uart_1_wire.comm_timeout != 0U)
                {
                    ch_set_state(BMS_ERR, "BMS通信超时");
                    break;
                }
                if(ch_bms_status_check() != 0)
                {
                    break;
                }

                DCJK = 1;
                REPAIR_OUTPUT = 0;
                VADJ = 1;
                FAN = (val.curr > iPRE/2) ? 1 : 0;
                Ged_Flash(100);
                TimCut();
                set_Curr_Duty(SET_CURR(iPRE));
                pre_end_voltage_mv = ch_get_pre_end_voltage_mv();

                if(Tim.min >= TIM_PRE)
                {
                    ch_set_state(CH_TimOut, "预充超时");
                }
                else if(val.vout >= pre_end_voltage_mv)
                {
                    if(++s_cut[0] >= 50U)        /* 500ms确认预充完成 */
                    {
                        ch_set_state(CH_CCCV, "预充完成");
                    }
                }
                else
                {
                    s_cut[0] = 0;
                }
                
                if(val.vout < pack_uvp_mv)
                {
                    if(++s_cut[1] >= 50U)        /* 500ms确认电池低压 */
                    {
                        ch_set_state(CH_UVP, "电池低压");
                    }
                }
                else
                {
                    s_cut[1] = 0U;
                }
                break;
                
                

            case CH_CCCV:
                /*
                 * CCCV：继电器闭合、分压常开。
                 * 这里直接处理拔电池、有压无流、满电。
                 */
                s_dummy_load_10ms = 500U;
                uart_1_wire_set_stage(U1W_STAGE_CHARGE);
                uart_1_wire_poll_10ms();

                BATT_DIVIDER_EN = 1;
                adc_sample_all();
                ch_err_ck();
                if(val.vout < vRESET)
                {
                    uart_1_wire_reset_link();
                    ch_set_state(CH_IDLE, "拔电池");
                    break;
                }
                
                if(s_cut[0] == 50)
                {
                    if(++s_cut[1] >= 10U)
                    {
                        s_cut[1] = 0;
                        if(s_cccv_curr_limit_ma > val.curr + 100)
                        {
                            s_cccv_curr_limit_ma -=  100;
                        }
                        
                        if(s_cccv_curr_limit_ma < iGED+100)
                        {
                            s_cccv_curr_limit_ma = iGED+100;
                        }
                    }
                }
                else
                {
                    if(val.vout >= target_voltage_mv)
                    {
                        if(val.curr >= target_current_ma-200)
                        {
                            if(s_cut[0] < 50U)
                            {
                                s_cut[0]++;
                            }
                        }
                    }
                    s_cccv_curr_limit_ma =  target_current_ma;
                }

                if(ch_check_protect_state() != 0)
                {
                    ch_output_all_off();
                    break;
                }

                if(ch_no_current_fault_check_10ms(target_voltage_mv, s_cccv_curr_limit_ma, 1U) != 0)
                {
                    break;
                }
                if(uart_1_wire.comm_timeout != 0U)
                {
                    ch_set_state(BMS_ERR, "BMS通信超时");
                    break;
                }
                if(ch_bms_status_check() != 0)
                {
                    break;
                }

                DCJK = 1;
                REPAIR_OUTPUT = 0;
                VADJ = 1;
                FAN = (val.curr > iGED/2) ? 1 : 0;
                RLED = 0;
                Ged_Flash(100);
                TimCut();

                set_Curr_Duty(SET_CURR(s_cccv_curr_limit_ma));

                if(Tim.min >= cccv_timeout_min)
                {
                    ch_set_state(CCCV_TimOut, "CCCV超时");
                }
                else if((val.vout >= target_voltage_mv) && (val.curr <= iGED))
                {
                    if(++s_cut[2] >= 100U)       /* 1秒确认满电 */
                    {
                        ch_set_state(CH_FULL, "转满电");
                    }
                }
                else
                {
                    s_cut[2] = 0;
                }
                break;

            case CH_FULL:
                /*
                 * 满电：输出关闭，分压默认关。
                 * 每1秒短开一次，20ms后判断拔电池或重新握手。
                 */
                uart_1_wire_set_stage(U1W_STAGE_FULL_DISPLAY);
                uart_1_wire_poll_10ms();
                ch_output_all_off();
                RLED = 0;
                GLED = 1;

                vout_valid = 0U;
                if(s_vout_probe_on_10ms != 0U)
                {
                    BATT_DIVIDER_EN = 1;
                    adc_sample_all();
                    if(s_vout_probe_on_10ms >= 2U)   /* 打开20ms后ADC有效 */
                    {
                        vout_valid = 1U;
                    }
                    s_vout_probe_on_10ms++;
                    if(s_vout_probe_on_10ms > 10U)   /* 最多打开100ms，降低漏电/倒灌风险 */
                    {
                        s_vout_probe_on_10ms = 0U;
                        s_vout_probe_period_10ms = 0U;
                        BATT_DIVIDER_EN = 0;
                    }
                }
                else
                {
                    BATT_DIVIDER_EN = 0;
                    adc_sample_all();
                    if(s_vout_probe_period_10ms < 100U)  /* 每1秒打开一次分压检测 */
                    {
                        s_vout_probe_period_10ms++;
                    }
                    else
                    {
                        s_vout_probe_on_10ms = 1U;
                        BATT_DIVIDER_EN = 1;
                    }
                }

                if(vout_valid != 0U)
                {
                    if(val.vout < vRESET)
                    {
                        if(++s_remove_cnt >= 2U)      /* 连续2次有效采样确认拔电池 */
                        {
                            uart_1_wire_reset_link();
                            ch_set_state(CH_IDLE, "拔电池");
                        }
                    }
                    else if (val.vout < pack_recharge_mv)
                    {
                        s_remove_cnt = 0U;
                        if(++s_cut[0] >= 2U)          /* Prefer B1 max cell; fallback to pack voltage only without B1. */
                        {
                            s_full_recharge = 1;
                            uart_1_wire_reset_link();
                            ch_set_state(BMS_HANDSHAKE, "满电回充");
                        }
                    }
                    else
                    {
                        s_remove_cnt = 0U;
                        s_cut[0] = 0U;
                    }
                }
                break;

            case BMS_TEMP_ERR:
                /* BMS温度异常：输出关闭，周期短开分压；温度恢复后重新握手。 */
                uart_1_wire_set_stage(U1W_STAGE_TEMP_WAIT);
                uart_1_wire_poll_10ms();
                ch_output_all_off();
                RGed_Flash(100);
                goto stopped_state_probe;

            case BMS_ERR:
            case CH_TimOut:
            case CCCV_TimOut:
            case CH_UVP:
            case CH_OVP:
            case CH_OCP:
            case HW_ERR:
                /* 通信/超时/电压电流保护：输出关闭，周期短开分压；拔电池后回待机。 */
                uart_1_wire_set_stage(U1W_STAGE_PULL_LOW);
                uart_1_wire_poll_10ms();
                ch_output_all_off();
                Red_Flash(100);
                goto stopped_state_probe;

            case CH_OTP:
            case NTC_ERR:
                /* 温度/NTC/硬件异常：输出关闭，周期短开分压；拔电池或温度恢复后处理。 */
                uart_1_wire_set_stage(U1W_STAGE_PULL_LOW);
                uart_1_wire_poll_10ms();
                ch_output_all_off();
                RGed_Flash(100);

stopped_state_probe:
                vout_valid = 0U;
                if(s_vout_probe_on_10ms != 0U)
                {
                    BATT_DIVIDER_EN = 1;
                    adc_sample_all();
                    ch_err_ck();
                    if(s_vout_probe_on_10ms >= 2U)   /* 打开20ms后ADC有效 */
                    {
                        vout_valid = 1U;
                    }
                    s_vout_probe_on_10ms++;
                    if(s_vout_probe_on_10ms > 10U)   /* 最多打开100ms */
                    {
                        s_vout_probe_on_10ms = 0U;
                        s_vout_probe_period_10ms = 0U;
                        BATT_DIVIDER_EN = 0;
                    }
                }
                else
                {
                    BATT_DIVIDER_EN = 0;
                    adc_sample_all();
                    ch_err_ck();
                    if(s_vout_probe_period_10ms < 100U)  /* 每1秒打开一次分压检测 */
                    {
                        s_vout_probe_period_10ms++;
                    }
                    else
                    {
                        s_vout_probe_on_10ms = 1U;
                        BATT_DIVIDER_EN = 1;
                    }
                }

                if((vout_valid != 0U) && (val.vout < vRESET))
                {
                    if(++s_remove_cnt >= 2U)          /* 连续2次有效采样确认拔电池 */
                    {
                        uart_1_wire_reset_link();
                        ch_set_state(CH_IDLE, "拔电池");
                    }
                }
                else if(vout_valid != 0U)
                {
                    s_remove_cnt = 0U;
                }

                if(ch_state == BMS_TEMP_ERR)
                {
                    if(uart_1_wire.comm_timeout != 0U)
                    {
                        ch_set_state(BMS_ERR, "温度通信超时");
                    }
                    else if((uart_1_wire.charge_status & CH_BMS_ERR_MASK) != 0U)
                    {
                        ch_set_state(BMS_ERR, "BMS异常");
                    }
                    else if(ch_bms_temp_recovered() != 0)
                    {
                        uart_1_wire_reset_link();
                        ch_set_state(BMS_HANDSHAKE, "BMS温度恢复");
                    }
                }
                else if((ch_state == CH_OTP) && (ch_flag.ch_hotErr == 0))  /* 充电器温度恢复后重新握手 */
                {
                    uart_1_wire_reset_link();
                    ch_set_state(BMS_HANDSHAKE, "OTP恢复，重新握手");
                }
                break;

            case CH_AGING:
                /* 老化：强制满功率输出，分压常开，便于观察电压电流。 */
                uart_1_wire_set_stage(U1W_STAGE_HANDSHAKE);
                uart_1_wire_poll_10ms();
                BATT_DIVIDER_EN = 1;
                adc_sample_all();
//                ch_err_ck();
//                if(ch_check_protect_state() != 0)
//                {
//                    ch_output_all_off();
//                    break;
//                }

                DCJK = 1;
                VADJ = 1;
                FAN = 1;
                set_Curr_Duty(SET_CURR(iMAX));
                Ged_Flash(100);
                break;
            }

            if(s_dummy_load_10ms != 0U)
            {
                DUMMY_LOAD = 1;
                s_dummy_load_10ms--;
            }
            else
            {
                DUMMY_LOAD = 0;
            }
        }
    }
}
