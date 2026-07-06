当前修改状态：已完成，待 ChatGPT/用户复核
下一步交给 ChatGPT 确认：是
确认入口：.ai-bridge/总结/程序修改报告.md
说明：本轮已完成 CH_REPAIR、DUMMY_LOAD、满电滞后恢复的小范围源码修改；已用 Keil UV4 Rebuild 通过，Program Size: data=157.3 xdata=315 const=146 code=15444，0 Error(s), 0 Warning(s)。`App/cal.c` 为用户已有改动，勿恢复、勿混入本轮提交。

---

# YAT-60V3A 程序修改与回报要求

Updated: 2026-07-06T03:06:38.236Z
Workspace: D:\Users\CODE
Target agent: Codex (codex)

## Plan

# 给 CODEX 的执行要求：YAT-60V3A 程序修改与回报

## 1. 先读文件

动手前必须先读：

```text
AGENTS.md
USER.md
中文编码安全处理Skill.md
.ai-bridge/交接/2026-07-05-当前交接.md
.ai-bridge/履历/63V 3A 充电器规格.html
.ai-bridge/记忆/2026-07-05-当前记忆.md
```

其中 `.ai-bridge/交接/2026-07-05-当前交接.md` 后续用户可能会重命名为《程序修改计划》。如已重命名，以《程序修改计划》为准。

## 2. 编码硬规则

修改旧 MCU 源码前，必须先检测编码。

```text
App/ch.c
App/usr_cfg.h
App/pc_uart.c
Bsp/common.h
其他 .c/.h 旧源码
```

必须按原编码读取和写回。不能用乱码内容分析中文注释，不能把源码转成 UTF-8。写回后检查 diff，不能出现整文件重写、乱码、换行符大面积变化。

## 3. 本轮只允许按《程序修改计划》小范围改

重点修改方向：

```text
1. CH_REPAIR 通路修正：只开 REPAIR_OUTPUT，不闭合 JK1，不调用 set_Curr_Duty(SET_CURR(iREPAIR))；`set_Curr_Duty(PWMMAX/2)` 按用户确认保留为硬件电流环/光耦保活，离开 CH_REPAIR 关闭 REPAIR_OUTPUT。
2. DUMMY_LOAD：不再作为复杂“突断”判断；上电/进入充电流程打开 5 秒，CCCV 关输出相关路径打开 5 秒，CH_Pre1 不开启。
3. 满电恢复：不能只用 val.vout < vCH60；优先按 B1 最高单节 cell_max_mv <4000mV 附近判断。
4. 温度保护/恢复：按 18650 / 21700 区分阈值。
5. LED：待机、充电、电压异常、温度异常、满电逐状态对齐参数表。
6. 定时关断：核对是否按容量 / 电流 * 60min * 1.25 + 30min，不要只依赖固定 TIM_CCCV。
```

不要批量格式化，不要重构大块状态机，不要新增大量函数/字符串。当前 code 空间很紧。

## 4. 改完必须写给 ChatGPT/用户确认的文档

改完后必须新增或更新以下文件：

```text
.ai-bridge/总结/程序修改报告.md
```

该报告必须包含以下章节，缺一不可：

### 4.1 修改范围

列出实际修改的文件，例如：

```text
YAT-60V3A-Uart-1-wire/App/ch.c
YAT-60V3A-Uart-1-wire/App/usr_cfg.h
YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
YAT-60V3A-Uart-1-wire/App/uart_1_wire.h
```

如果某个计划项没有改，必须说明为什么没改。

### 4.2 编码检查结果

必须逐文件写明：

```text
文件名：
修改前编码：
写回编码：
是否保持原编码：是/否
是否出现乱码：是/否
是否出现整文件重写：是/否
```

### 4.3 按规格逐项对齐说明

按以下条目逐项说明“已改/未改/不涉及”：

```text
3.21 待机红常亮绿灭，电压判断插入
3.22 低于 1*N V 关输出，红灯闪烁
3.23 1*N~2.5*N 低压充电，30min 不到 2.5*N 以上则异常
3.24 >2.5*N 进入大电流 CC，红灭绿闪
3.25 4.20*N + 截止电流后满电，完全切断充电，绿常亮
3.26/3.27 温度异常可自动恢复，电压/电流/通信异常必须拔插恢复
参数表：温度保护/恢复按 18650 / 21700
参数表：滞后恢复按最高单节 <4.0±0.05V
参数表：定时关断公式
```

### 4.4 CH_REPAIR 专项说明

必须明确写：

```text
REPAIR_OUTPUT 如何控制
JK1 是否保持断开
PWM 是否关闭
是否删除 set_Curr_Duty(SET_CURR(iREPAIR))
离开 CH_REPAIR 时如何关闭 REPAIR_OUTPUT
```

### 4.5 DUMMY_LOAD 专项说明

必须明确写：

```text
负载突然断开的触发条件是什么
DUMMY_LOAD 何时置 1
5 秒计时变量是什么
5 秒后在哪里置 0
哪些状态会强制关闭 DUMMY_LOAD
```

### 4.6 usr_cfg.h 参数依据说明

如果修改了 `usr_cfg.h`，必须列出每个阈值的依据。格式：

```text
参数名：
当前值：
规格依据：
用途：
是否规格直接给出：是/否
备注：
```

至少覆盖：

```text
BAT_SERIES
vSTART / CELL_LOW_MV
vPRE_30V / CELL_REPAIR_MV
vPRE_37V5 / CELL_PRE_MV
SET_vMAX / CELL_FULL_MV
vCH60 / CELL_RECHG_MV
vDCOVP
iREPAIR
iPRE
iMAX
iGED
iOCP
iOCP_OK
TIM_PRE
TIM_CCCV
```

### 4.7 编译结果

必须写明：

```text
是否编译：是/否
编译命令或方式：
0 Error(s), 0 Warning(s) 是否达成：是/否
Program Size：
剩余 code/const 空间估算：
```

如果无法编译，必须写明原因和卡点。

### 4.8 Git diff 摘要

必须写明：

```text
哪些文件有 diff
每个文件改了什么
是否有构建日志/临时文件混入
是否有非预期变更
```

### 4.9 待用户/ChatGPT确认问题

把不确定项列成编号问题，格式：

```text
1. xxx 是否按 A 处理？
2. xxx 需要现场确认吗？
3. xxx 是否允许后续再改？
```

不要写模糊的“待确认”。必须编号。

## 5. 改完后还要更新计划文件

改完后还要更新：

```text
.ai-bridge/current-plan.md
```

在文件顶部写明：

```text
当前修改状态：已完成 / 未完成 / 部分完成
下一步交给 ChatGPT 确认：是
确认入口：.ai-bridge/总结/程序修改报告.md
```

## 6. 禁止事项

```text
不要提交 git。
不要推送。
不要删除旧源码。
不要批量格式化。
不要批量转码。
不要把 ch.c 转 UTF-8。
不要改和本轮规格对齐无关的功能。
不要把乱码内容写回文件。
不要把构建日志加入提交范围。
```

## 7. 完成后的交付口径

完成后回复用户时必须只说：

```text
已完成修改，请让 ChatGPT 读取：
1. .ai-bridge/总结/程序修改报告.md
2. .ai-bridge/current-plan.md
3. git diff / show_changes
```

不要只说“改好了”，必须给出上面三个确认入口。

## Implementation contract

- Work from this plan in small, reviewable steps.
- Keep edits scoped to the requested task and existing project conventions.
- Run focused verification before handing work back.
- Update .ai-bridge/agent-status.md with files touched, checks run, results, blockers, and review notes.
- Save the final review diff to .ai-bridge/implementation-diff.patch when practical.
- Append notable execution events to .ai-bridge/execution-log.jsonl when the implementation agent supports logging.
