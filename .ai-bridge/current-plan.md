# 按PDF修正充电保护与拔除判断

Updated: 2026-06-27T10:25:51.372Z
Workspace: D:\Users\CODE
Target agent: Codex (codex)

## Plan

# 执行计划：按 PDF 修正充电保护与拔除判断

## 0. 重要说明

本计划覆盖 `.ai-bridge/current-plan.md`。此前追加内容里有一句错误表述：

```text
充电中继电器闭合状态：不靠 val.vout 判断拔除，继续使用“小电流 + 1秒无合法一线通信帧”的方式判断拔除。
```

这句话不能作为最终实现依据。

最终以用户最新确认的流程为准：

```text
充电中拔出判断：
通信中断大于 1 秒 + 电流小于 IEND
=> 先关闭输出
=> 延时 20ms
=> 再判断 vout 是否小于 1V
   是：确认拔电池，回 CH_IDLE
   否：不是正常拔除，进入异常
```

## 1. 资料来源

本计划综合：

1. 《一线通信代码实现思路.md》
2. 《一线通信异步轮询协议重构总结.md》
3. `63V 3A CHARGER SPECIFICATION 102003001204-V1.0.pdf`
4. 用户已提供的源码包 `YAT-60V3A-Uart-1-wire`
5. 用户最新确认的低压插入、拔除、充电中拔除判断逻辑

PDF 是 63V/3A 充电器规格书，重点核对充电规格、保护、LED、恢复逻辑。

## 2. 必须保持的架构原则

继续保持一线通信异步轮询模型：

- 发送不等待对应回复。
- 每 10ms 调用任务，内部约 100ms 发送一帧。
- 不恢复“发一帧、等一帧回复、再发下一帧”的同步模型。
- 不恢复 `wait_cmd / wait_len / wait_start_ms / retry_count / busy / can_send` 这类同步等待变量。
- 接收只扫描合法帧，数据是否新鲜由计时器判断。

职责边界保持：

- `uart_1_wire.c` 只负责一线通信协议收发、解析、计时、主动拉低/释放 COM。
- `uart_1_wire.c` 不直接切换充电状态。
- `ch.c` 负责充电状态机、保护、满电、温度异常、电池插入/拔除。
- `ch.c` 不解析协议帧原始字节。

## 3. PDF 关键要求

请按 PDF 重点核对：

- 满电截止电压/电流：`63V±0.20V，500±200mA`。
- 额定输出电流：`3±0.3A`。
- 过流保护：超过 `3.5±0.3A` 关断输出并报错。
- 低压保护：低于 `1.0±0.2*N V`，关断输出、报错。
- 预充：`1.0*N ~ 2.5*N V`，`500±200mA`，`30min±5%`。
- 涓流/低压修复：`1.0*N ~ 2.0*N V`，`100mA` 以下，`30min±5%`。
- 过压保护：超过 `4.2±0.02*N V`，关断输出，显示充满。
- 通信异常保护：电池插入时通信异常，切断输出。
- 温度异常恢复后可自动重新开始充电。
- 电流、电压、通信异常时，必须重新插拔电池后才能再次运行。
- 保护动作时不得有输出电压和电流，即完全关闭。
- LED：待机红常亮绿灭；充电绿 1Hz；电压异常红 1Hz；温度异常红绿 1Hz 交替；满电绿常亮。

## 4. 当前源码中已确认的关键宏

用户已提供源码包，不要再写“无法确认 IEND”。

当前工程没有独立 `IEND` 宏，已有满电/转灯电流阈值：

```c
#define iGED (500U)
```

因此用户口头说的 `IEND`，本工程按 `iGED = 500mA` 理解。

当前重要电压边界：

```text
vSTART = 15V：待机正常插入/起握手门槛。
vRESET = 1V：其它非充电输出状态确认拔除回待机门槛。
vPRE_30V = 30V：低压修复/涓流上限。
vPRE_37V5 = 37.5V：预充上限。
SET_vMAX = 63V：满电目标。
vCH60 = 60V：当前整包回充参考，需要核对是否应改为最高单节 <4.0V。
```

## 5. 待机 CH_IDLE 插入判断

### 5.1 正常插入

```text
val.vout >= vSTART，也就是 >=15V：
    按原逻辑连续确认后进入 BMS_HANDSHAKE。
```

### 5.2 低压电池插入

用户确认：待机可以读取 `val.vout` 判断低压电池插入。

```text
vRESET < val.vout < vSTART，也就是 1V～15V：
    可能是真实低压电池，
    也可能是刚拔电池后的滤波电容/104 电容残压缓降，
    也可能是刚插电池时采样节点未稳定。
```

因此不能一看到 `1V～15V` 就立刻报低压异常。

必须增加独立的待机低压稳定确认逻辑：

```text
1. 进入 1V～15V 区间后，启动低压候选计数。
2. 连续一段时间保持在 1V～15V，且电压变化不再明显下降/上升，再判定为真实低压电池。
3. 如果电压继续下降到 <=1V，认为是残压放空/无电池，清计数，保持待机。
4. 如果电压上升到 >=15V，认为是正常电池插入，清低压候选计数，走原握手流程。
5. 低压确认后进入低压异常状态，输出关闭，红灯 1Hz 闪烁，绿灯熄灭。
```

限制：

- 低压候选计数不要复用 `s_remove_cnt`。
- 不要把刚插入瞬间的采样过渡值判为低压异常。
- 不要把刚拔出后的残压缓降判为低压异常。

## 6. 其它非充电输出状态拔除判断

保持原原则：

```text
val.vout < vRESET，也就是 <1V，才确认电池拔除并回 CH_IDLE。
```

这与待机 1V～15V 低压插入识别不冲突，因为场景不同。

## 7. 充电中继电器闭合状态拔除判断：最终以本节为准

### 7.1 禁止使用的旧逻辑

不要再使用下面旧逻辑作为最终判断：

```text
小电流 + 1 秒无合法一线通信帧 => 直接确认拔除并回待机
```

也不要再写：

```text
充电中继电器闭合状态不靠 val.vout 判断拔除，继续使用小电流 + 1秒无通信判断拔除。
```

这不是最终需求。

### 7.2 正确的新流程

用户最新确认的稳定流程：

```text
通信中断 > 1 秒
并且电流小于 IEND
=> 先关闭输出
=> 延时 20ms
=> 再判断 vout 是否小于 1V
   是：确认电池拔除，回 CH_IDLE
   否：不是正常拔除，进入异常
```

本工程中 `IEND` 使用：

```c
iGED = 500U
```

所以疑似拔除条件应为：

```c
s_u1w_info.no_rx_10ms >= 100U
val.curr < iGED
```

不要再使用旧条件：

```c
val.curr < (u16)(iGED / 2U)
```

### 7.3 两阶段实现要求

建议实现为两阶段，避免在一个 10ms 周期内直接判断：

```text
阶段 1：疑似拔除
    条件：no_rx_10ms >= 100U，并且 val.curr < iGED。
    动作：ch_output_all_off();
          启动 20ms 等待计数。

阶段 2：关闭输出后确认
    等待 20ms 后，确认 ADC/val.vout 已更新。
    如果 val.vout < vRESET，也就是 <1V：
        确认拔除；
        BATT_DIVIDER_EN = 0；
        uart_1_wire_reset_link();
        ch_vout_probe_reset();
        ch_set_state(CH_IDLE, "拔电池");
    否则：
        认为不是正常拔除；
        保持输出关闭；
        进入 BMS_ERR 或当前工程等价普通异常状态；
        后续通信阶段走 PULL_LOW。
```

注意：如果 20ms 内 ADC 采样未必更新，应按工程 ADC 周期选择最小可靠等待时间，但交付说明必须写明。

## 8. B4 状态位处理

B4 状态位按以下分类处理：

```text
bit0 单节过充：按 PDF，切断输出并显示充满，优先进入 CH_FULL 或等价满电状态。
bit1 低温：进入 BMS_TEMP_ERR，继续通信等待恢复，不拉低 COM。
bit2 高温：进入 BMS_TEMP_ERR，继续通信等待恢复，不拉低 COM。
bit3 MOS 过温：进入 BMS_TEMP_ERR，继续通信等待恢复，不拉低 COM。
bit4 过流：进入普通异常/BMS_ERR，输出关闭，PULL_LOW。
bit5 短路：进入普通异常/BMS_ERR，输出关闭，PULL_LOW。
bit6 充电超时：进入普通异常/BMS_ERR，输出关闭，PULL_LOW。
bit7 电池包失效：进入普通异常/BMS_ERR，输出关闭，PULL_LOW。
```

限制：

- 不新增“根据 B4 异常发送 B6 关闭 BMS MOS”的动作。
- BMS 普通异常由 BMS 自己处理 MOS。
- 充电器只停止自己的充电输出，并按状态机进入满电、温度等待或普通异常。

## 9. 满电与回充

保持满电流程：

- 关闭充电输出。
- LED 满电：红灯灭，绿灯常亮。
- 通信进入 `FULL_DISPLAY`，周期发送 `B6 03 SOC`，持续 3 分钟。
- 3 分钟后主动拉低 COM。

回充判断需核对 PDF：

```text
滞后恢复充电电压：最高单节电压小于 4.0±0.05V。
```

如果一线通信中有最高单节电压字段，优先按最高单节 `< CELL_RECHG_MV` 判断。若由于满电后停止通信无法实时获取最高单节，只能用整包电压近似，必须在 `agent-status.md` 说明差异。

回充前必须重新握手，不允许复用旧数据继续充。

## 10. LED 1Hz

检查 `Ged_Flash / Red_Flash / RGed_Flash` 的实际周期。

必须符合 PDF：

```text
待机：红灯常亮，绿灯灭。
充电：绿灯 1Hz 闪烁，红灯灭。
电压异常：红灯 1Hz 闪烁，绿灯灭。
温度异常：红绿 1Hz 交替闪烁。
满电：红灯灭，绿灯常亮。
```

如果 `Flash(50)` 实际不是 1Hz，应做最小修改。优先只改参数或 Flash 内部周期定义，不要大改 LED 状态机。

## 11. 不允许做的事

- 不要实现 `A2 / A3 / A5 / B0 / B2 / B5 / B7 / B8`。
- 不要扩大到整份 X6 V1.3 协议实现。
- 不要修改字节序，用户已确认低字节在前。
- 不要修改 `u1w_get_u16_bus()` 的低字节在前逻辑。
- 不要恢复同步等待/单帧重试模型。
- 不要新增复杂状态机。
- 不要让协议层直接跳充电状态。
- 不要让 `ch.c` 解析协议帧字节。
- 不要新增 B6 关闭 BMS MOS 动作。
- 不要默认打开协议收发日志。
- 不要自动格式化整文件。
- 不要整文件重写。
- 不要转码源码。
- 不要改 UART 波特率、半双工方向、帧格式、CHK 算法。

## 12. 编码和 Git 保护

开始源码修改前必须：

1. 读取并遵守 `AGENTS.md` 和 `USER.md`。
2. 查看 Git 状态，确认已有改动，不能覆盖用户改动。
3. 对目标 `.c/.h` 文件做编码预检。
4. 源码中文注释可能是 GBK/CP936/ANSI，禁止普通 UTF-8 写回导致乱码。
5. 保持原编码、原 BOM、原行尾。
6. 只做最小修改。
7. 修改后检查 diff，确认没有整文件转码、乱码、格式化。

## 13. 建议修改文件

优先修改：

```text
YAT-60V3A-Uart-1-wire/App/ch.c
```

必要时才修改：

```text
YAT-60V3A-Uart-1-wire/App/uart_1_wire.h
YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
YAT-60V3A-Uart-1-wire/App/usr_cfg.h
```

一般不要修改：

```text
Bsp/uart.c
Bsp/uart.h
fifo.c/fifo.h
```

## 14. 验收要求

完成后至少验证：

1. Keil Rebuild，若无法构建，说明原因。
2. `TASK_10MS` 是否确实 10ms。
3. 满电截止：63V、500mA 是否仍对齐。
4. 预充：500mA、30min 是否仍对齐。
5. 涓流/低压修复：15V～30V、100mA 是否符合当前设计。
6. 低压插入：待机 1V～15V 是否稳定确认后报低压异常。
7. 低压插入是否避开刚拔电池残压缓降。
8. 低压插入是否避开刚插电池采样过渡。
9. 其它非充电输出状态是否仍保持 `<1V` 回待机。
10. 充电中拔除是否已改为：通信中断 >1 秒 + 电流 < iGED -> 关输出 -> 等待 20ms -> vout 判断。
11. 20ms 后 `vout <1V` 是否回 `CH_IDLE`。
12. 20ms 后 `vout >=1V` 是否进入异常。
13. 是否没有继续使用 `iGED / 2` 作为充电中拔除电流阈值。
14. B4 单节过充是否显示满电。
15. B4 温度类故障是否进入 `BMS_TEMP_ERR`，继续通信，不拉低 COM。
16. B4 过流/短路/充电超时/失效是否进入普通异常/PULL_LOW。
17. 是否没有新增 B6 关闭 BMS MOS 动作。
18. LED 是否符合 1Hz 要求。
19. 保护动作时是否保持无输出电压和电流。
20. 电压/电流/通信异常是否必须重新插拔电池后才能再次运行。
21. 满电回充是否按最高单节电压，或明确说明当前用整包电压近似的差异。
22. Git diff 是否只包含必要修改。
23. 编码是否未变化。

## 15. 交付输出要求

完成后请在 `.ai-bridge/agent-status.md` 用中文说明：

1. 修改了哪些文件。
2. 充电中拔除新流程具体如何实现。
3. `IEND` 实际使用 `iGED=500mA`。
4. 20ms 后 vout 判断结果对应的状态去向。
5. 待机低压插入稳定确认条件。
6. 如何避免刚拔电池残压误判。
7. 如何避免刚插电池采样过渡误判。
8. B4 每一类故障如何处理。
9. PDF 每个关键条款的对齐结果。
10. 是否保持异步轮询和低字节在前。
11. 是否没有新增 B6 关闭 BMS MOS。
12. Keil Rebuild 或检查结果。
13. 编码保护结果。
14. Git diff 摘要。
15. 无法确认的问题必须列出来，不要擅自猜。

## Implementation contract

- Work from this plan in small, reviewable steps.
- Keep edits scoped to the requested task and existing project conventions.
- Run focused verification before handing work back.
- Update .ai-bridge/agent-status.md with files touched, checks run, results, blockers, and review notes.
- Save the final review diff to .ai-bridge/implementation-diff.patch when practical.
- Append notable execution events to .ai-bridge/execution-log.jsonl when the implementation agent supports logging.

---

# 追加16K容量约束

Updated: 2026-06-27T10:30:16.812Z
Workspace: D:\Users\CODE
Target agent: Codex (codex)

## Plan

# 追加硬约束：单片机 16K 容量限制

用户确认：当前单片机程序空间只有 16K，校准存储还会占用一部分空间。后续修改必须把代码容量作为硬约束，不能按 PC 程序思路大改。

## 1. 容量约束

本任务所有修改必须遵守：

```text
单片机总容量约 16K。
校准存储已经占用一部分空间。
必须避免写超程序空间。
```

因此实现时必须优先选择“小补丁、少代码、少 RAM、少字符串”的方案。

## 2. 禁止膨胀代码

禁止为了实现本任务而引入明显增大代码体积的写法：

- 不要新增复杂状态机框架。
- 不要新增大表格、大数组、大量查表。
- 不要新增大量 debug 字符串。
- 不要新增大量 `uart_printf` / 日志打印。
- 不要新增长中文字符串到源码。
- 不要为了可读性拆出太多小函数，导致调用和代码体积增加。
- 不要整文件重构。
- 不要把简单判断写成复杂通用框架。
- 不要引入浮点计算。
- 不要引入动态内存。
- 不要新增无关功能。

## 3. 推荐实现方式

优先采用最省空间的写法：

- 尽量复用现有变量、计数器和状态枚举，但不能造成逻辑混乱。
- 必须新增计数器时，优先使用 `u8`，确认范围足够后再用 `u16`。
- 低压插入稳定确认、充电中拔除两阶段确认，尽量在现有 `ch.c` 流程里用少量静态变量实现。
- B4 故障分类优先用简单位判断，不要做复杂映射表。
- LED 1Hz 修正优先改参数或现有函数，不新增新 LED 框架。
- 日志只保留必要状态，不新增刷屏日志。
- 所有新增注释简短，不要写大段说明进 `.c` 文件。

## 4. 编译后必须看容量

完成修改后，必须检查 Keil 编译输出中的程序容量/ROM 使用情况。

交付时在 `agent-status.md` 写清楚：

1. 编译是否通过。
2. Code/ROM 使用量是多少。
3. 距离 16K 还剩多少余量。
4. 校准存储是否可能受影响。
5. 如果已经接近 16K，必须优先删减新增日志、字符串和非必要函数。

## 5. 代码取舍优先级

如果容量不够，按以下优先级取舍：

```text
必须保留：安全保护、关输出、拔除判断、通信异常、低压异常、温度异常。
可以压缩：日志、长注释、重复函数、重复状态字符串。
不能牺牲：保护动作时无输出、充电中拔除判断、B4 故障处理、编码不转码。
```

## 6. 对 Codex 的特别提醒

这不是桌面软件，也不是资源充足的 MCU。代码要保守，能少写就少写，能复用就复用，能用简单 if 判断就不要做通用框架。最终以能编译、容量不超、保护可靠为第一优先级。

## Implementation contract

- Work from this plan in small, reviewable steps.
- Keep edits scoped to the requested task and existing project conventions.
- Run focused verification before handing work back.
- Update .ai-bridge/agent-status.md with files touched, checks run, results, blockers, and review notes.
- Save the final review diff to .ai-bridge/implementation-diff.patch when practical.
- Append notable execution events to .ai-bridge/execution-log.jsonl when the implementation agent supports logging.

---

# 追加资料优先级规则

Updated: 2026-06-27T10:33:25.574Z
Workspace: D:\Users\CODE
Target agent: Codex (codex)

## Plan

# 追加硬约束：两份一线通信 MD 必须阅读，但冲突以当前计划为准

用户确认：Codex 修改前必须阅读以下两份资料：

```text
一线通信代码实现思路.md
一线通信异步轮询协议重构总结.md
```

这两份资料是理解一线通信异步轮询架构、职责边界、通信阶段、发送/接收模型的重要背景，必须阅读。

## 1. 两份 MD 的作用

两份 MD 用于理解：

- 为什么一线通信改成异步轮询。
- 为什么 `uart_1_wire.c` 只负责协议。
- 为什么 `ch.c` 只负责充电状态机。
- 为什么不恢复同步等待应答模型。
- 为什么发送按阶段轮询，不等回复。
- 为什么接收只扫描合法帧并刷新数据。
- 为什么 BMS 普通异常不由充电器主动关闭 BMS MOS。
- 为什么温度异常继续通信等待恢复。
- 为什么满电使用 `FULL_DISPLAY` 发送 `B6 03 SOC`。

这些原则仍然有效。

## 2. 优先级规则

如果两份 MD 与当前 `current-plan.md` 或用户最新确认内容发生冲突，必须按以下优先级执行：

```text
第一优先级：用户本轮最新明确确认的要求。
第二优先级：当前 .ai-bridge/current-plan.md 中的最终计划。
第三优先级：PDF 规格书。
第四优先级：两份一线通信 MD 的背景原则。
第五优先级：当前源码已有实现。
```

也就是说：

```text
两份 MD 必须看，但不能覆盖当前最新计划。
与当前计划冲突时，以当前计划为准。
```

## 3. 已知必须以当前计划为准的冲突点

### 3.1 充电中拔出判断

如果两份 MD 或旧计划里还有类似说法：

```text
充电中继电器闭合状态不靠 val.vout 判断拔除，继续使用“小电流 + 1秒无合法一线通信帧”判断拔除。
```

这条已经作废，不能执行。

最终以当前计划为准：

```text
通信中断 > 1 秒
并且电流 < IEND，也就是当前工程 iGED=500mA
=> 先关闭输出
=> 延时 20ms
=> 再判断 vout 是否小于 1V
   vout < 1V：确认拔电池，回 CH_IDLE
   vout >= 1V：不是正常拔除，进入异常
```

### 3.2 待机低压插入判断

如果两份 MD 没有提到待机 `1V～15V` 低压电池识别，不代表不需要做。

最终以当前计划为准：

```text
CH_IDLE 待机可以用 val.vout 判断 1V～15V 低压电池插入，
但必须避开刚拔电池后的残压缓降，
也必须避开刚插电池时采样未稳定的过渡区，
确认稳定后才进入低压异常。
```

### 3.3 16K 容量限制

如果两份 MD 更偏向结构清晰、拆函数、加注释，本项目仍必须优先遵守 16K 容量限制。

最终以当前计划为准：

```text
小改、少变量、少字符串、少函数、少日志，
不得写超 16K，
Keil 编译后必须检查 Code/ROM 使用量。
```

## 4. Codex 执行要求

Codex 开始修改前请先阅读两份 MD，但执行时必须以当前计划为最终约束。遇到冲突不要自行猜，按当前计划；如果仍无法判断，写入 `agent-status.md` 作为问题，不要硬改。

## Implementation contract

- Work from this plan in small, reviewable steps.
- Keep edits scoped to the requested task and existing project conventions.
- Run focused verification before handing work back.
- Update .ai-bridge/agent-status.md with files touched, checks run, results, blockers, and review notes.
- Save the final review diff to .ai-bridge/implementation-diff.patch when practical.
- Append notable execution events to .ai-bridge/execution-log.jsonl when the implementation agent supports logging.
