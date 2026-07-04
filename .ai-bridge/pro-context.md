# YAT-60V3A-Uart-1-wire 当前交接

Generated: 2026-07-04T08:50:37.182Z
Workspace: D:\Users\CODE
Workspace ID: ws_4eb4de4a7c358b9a6f97f743
Write mode: workspace
Bash mode: full
Tool mode: full

Purpose: paste this bundle into a high-context ChatGPT model when that model cannot call the CodexPro MCP tools directly.
Instruction for ChatGPT: use this as repository context, produce a narrow Codex execution plan, and avoid inventing files or runtime facts not shown here.

## Repository Tree

.
├── YAT-21V4A-6A/
│   ├── Libraries/
│   │   ├── Include/
│   │   ├── Source/
│   │   └── Startup/
│   ├── Project/
│   │   └── Keil/
│   └── User/
│       ├── _1_wire.c
│       ├── _1_wire.h
│       ├── adc.c
│       ├── adc.h
│       ├── common.c
│       ├── common.h
│       ├── fifo.c
│       ├── fifo.h
│       ├── flash.c
│       ├── flash.h
│       ├── kalman.c
│       ├── kalman.h
│       ├── main.c
│       ├── main.h
│       ├── ntc_tab.h
│       ├── NTC.c
│       ├── NTC.h
│       ├── pc_uart.c
│       ├── pwm.c
│       ├── pwm.h
│       ├── stm32_crc.c
│       ├── stm32_crc.h
│       ├── timer.c
│       ├── timer.h
│       ├── uart.c
│       ├── uart.h
│       └── usr_cfg.h
├── YAT-60V3A-Uart-1-wire/
│   ├── App/
│   │   ├── cal.c
│   │   ├── cal.h
│   │   ├── ch.c
│   │   ├── ch.h
│   │   ├── main.c
│   │   ├── pc_uart.c
│   │   ├── pc_uart.h
│   │   ├── uart_1_wire_debug.c
│   │   ├── uart_1_wire_debug.h
│   │   ├── uart_1_wire.c
│   │   ├── uart_1_wire.h
│   │   └── usr_cfg.h
│   ├── Bsp/
│   │   ├── adc.c
│   │   ├── adc.h
│   │   ├── bsp_init.c
│   │   ├── bsp_init.h
│   │   ├── common.c
│   │   ├── common.h
│   │   ├── fifo.c
│   │   ├── fifo.h
│   │   ├── flash_layout.h
│   │   ├── flash.c
│   │   ├── flash.h
│   │   ├── Function_Init.H
│   │   ├── gpio.c
│   │   ├── gpio.h
│   │   ├── isr.c
│   │   ├── isr.h
│   │   ├── ntc_table.h
│   │   ├── ntc.c
│   │   ├── ntc.h
│   │   ├── pwm.c
│   │   ├── pwm.h
│   │   ├── stm32_crc.c
│   │   ├── stm32_crc.h
│   │   ├── timer.c
│   │   ├── timer.h
│   │   ├── uart.c
│   │   └── uart.h
│   ├── Libraries/
│   │   ├── Include/
│   │   ├── Source/
│   │   └── Startup/
│   ├── Project/
│   │   └── Keil/
│   ├── 60V 3A 通信协议.pdf
│   ├── 63V 3A 充电器规格.pdf
│   ├── memory.md
│   └── X6_BMS_Communication_Protocol_V1.3_FullContent.html
├── 一线通信代码实现思路.md
├── 一线通信异步轮询协议重构总结.md
├── 中文编码安全处理Skill.md
├── AGENTS.md
├── push_to_github.bat
└── USER.md

## Git Status

```text
## master...origin/master
 M .ai-bridge/codex-status.md
 M .ai-bridge/current-plan.md
 M .ai-bridge/execution-log.jsonl
 M .ai-bridge/implementation-diff.patch
 M .ai-bridge/session-log.jsonl
 D CHECK_CODEXPRO_WORKSPACE_ASCII_STAY.bat
 M YAT-60V3A-Uart-1-wire/App/ch.c
 M YAT-60V3A-Uart-1-wire/App/main.c
 M YAT-60V3A-Uart-1-wire/App/pc_uart.c
 M YAT-60V3A-Uart-1-wire/App/pc_uart.h
 M YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
 M YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
 M YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt
 M YAT-60V3A-Uart-1-wire/Project/Keil/config.ini
 D "YAT-60V3A-Uart-1-wire/\346\234\254\350\275\256\345\274\202\346\255\245\345\215\217\350\256\256\344\270\216\347\212\266\346\200\201\346\234\272\350\247\204\345\210\231\350\257\264\346\230\216.md"
?? YAT-21V4A-6A/
?? "YAT-60V3A-Uart-1-wire/60V 3A \351\200\232\344\277\241\345\215\217\350\256\256.pdf"
?? "YAT-60V3A-Uart-1-wire/63V 3A \345\205\205\347\224\265\345\231\250\350\247\204\346\240\274.pdf"
?? YAT-60V3A-Uart-1-wire/Project/Keil/build_codex.log
?? YAT-60V3A-Uart-1-wire/Project/Keil/codex_build_debug_on.log
?? YAT-60V3A-Uart-1-wire/Project/Keil/codex_build_debug_on2.log
?? YAT-60V3A-Uart-1-wire/Project/Keil/codex_build_default.log
?? YAT-60V3A-Uart-1-wire/Project/Keil/codex_build_tmp.log
?? "\344\270\255\346\226\207\347\274\226\347\240\201\345\256\211\345\205\250\345\244\204\347\220\206Skill.md"
```

## Recent Commits

```text
d38ae88 (HEAD -> master, origin/master) 优化一线通信快照与调试日志
2574a0a checkpoint before plan execution
1b86ed9 补注释，改宏名
f30a52d 补充注释
026cd78 fix: CCCV阶段按最高单节电压缓慢降流
4fe4140 1. 补充 B6 发送策略注释。 2. 明确满电后 3 分钟内周期性发送 B6 03 SOC。 3. 说明 MOS 状态变化时优先发送 B6 01。 4. 不修改通信逻辑和状态机。
1357ce6 修改串口名，应该是UART0的
3ff3b7d config: 简化校准Flash布局并取消Code CRC保护
```

## Git Diff

```diff
diff --git a/.ai-bridge/codex-status.md b/.ai-bridge/codex-status.md
index 8dd477b..fe2a6d8 100644
--- a/.ai-bridge/codex-status.md
+++ b/.ai-bridge/codex-status.md
@@ -1,3 +1,111 @@
 # Codex Status
 
-No Codex status written yet.
+更新时间: 2026-06-27
+
+## 当前状态
+- 已先提交当前工作区，提交: 2574a0a checkpoint before plan execution
+- 当前分支: master
+- 工作区: 提交后干净
+- 已读取: AGENTS.md, USER.md, .ai-bridge/current-plan.md
+- 已读取参考文档: 一线通信代码实现思路.md, 一线通信异步轮询协议重构总结.md
+
+## CodeGraph
+- 检测到 .codegraph
+- codegraph status 失败: PowerShell 执行策略阻止脚本；cmd /c codegraph status 报 unable to open database file
+- 处理方式: 暂按计划使用 rg 和源码人工复核，修改前输出影响范围判断
+
+## 当前计划步骤
+1. 修正充电中拔电池判定: 1s 无通信且电流小于 iGED 后，先关输出，等待约 20ms，再用 vout < vRESET 确认拔电池，否则进入 BMS_ERR。
+2. 检查并补齐 CH_IDLE 低压电池插入稳定确认，避免残压下降或插入瞬态误判。
+3. 复核 B4 状态分类、FULL_DISPLAY、复充、LED 1Hz 是否符合 current-plan。
+4. 每步修改后运行 Keil 编译/链接验证，并记录 code size。
+
+## 风险与约束
+- 严格遵守 ENCODING_GUARD + GIT_GUARD。
+- 不改串口收发流程为同步等待。
+- 不默认打开协议日志。
+- 不引入大宏、大日志、大字符串，优先小代码。
+## Step 1 结果: 充电中拔电池判定
+- 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
+- 修改点: `val.curr < iGED` 且 `no_rx_10ms >= 100` 后先关输出，约 20ms 后检查 `val.vout < vRESET`。
+- 正常拔出: 复位一线链路并回到 CH_IDLE。
+- 非正常拔出: 进入 BMS_ERR。
+- 编码: ch.c 保持 CP936, 无 BOM, LF。
+- 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
+- Program Size: data=155.4 xdata=315 const=146 code=14713。
+- 备注: Git 提示该文件受 autocrlf 影响，下次 Git 触碰可能转换 CRLF；本次写回仍保持 LF。
+## Step 2 结果: CH_IDLE 低压插入稳定确认
+- 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
+- 修改点: 待机时 `vRESET < val.vout < vSTART` 不立即报错，需连续约 2s 且相邻 10ms 变化不超过 100mV。
+- 正常插入: `val.vout >= vSTART` 仍按原 50 次确认后进 BMS_HANDSHAKE。
+- 残压放空: `val.vout <= vRESET` 清低压候选，保持 CH_IDLE。
+- 低压确认: 进入 CH_UVP，输出保持关闭。
+- 编码: ch.c 保持 CP936, 无 BOM, LF。
+- 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
+- Program Size: data=157.4 xdata=315 const=146 code=14893。
+## Step 3 结果: B4 分类、满电显示、LED、协议禁项复核
+- 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
+- B4 bit0: 进入 CH_FULL，等效满电处理。
+- B4 bit1/2/3: 进入 BMS_TEMP_ERR，输出关闭，继续轮询 B3/B4，不主动拉低 COM。
+- B4 bit4/5/6/7: 进入 BMS_ERR，输出关闭，通信阶段为 PULL_LOW。
+- BMS_TEMP_ERR 中若后续出现 bit4~7，也会转 BMS_ERR。
+- FULL_DISPLAY: 保持协议层发送 B6 03 SOC，3 分钟后拉低 COM；未改 uart_1_wire.c 收发流程。
+- 复充: FULL_DISPLAY 阶段不轮询 B1，当前仍用整包 vCH60 近似判断，并已在代码注释说明。
+- LED: Red_Flash/Ged_Flash/RGed_Flash(50) 在 10ms 调度下 500ms 翻转一次，完整周期约 1Hz；低压/普通异常为红灯约 1Hz。
+- 协议日志: U1W_DEBUG_EN=0, U1W_DEBUG_PARSE_EN=0，默认关闭。
+- 禁项复核: 未新增 A2/A3/A5/B0/B2/B5/B7/B8；未恢复 wait_cmd/wait_len/wait_start_ms/retry_count/busy/can_send 同步等待模型。
+- 编码: ch.c 保持 CP936, 无 BOM, LF。
+- 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
+- Program Size: data=157.4 xdata=315 const=146 code=14940。
+## Final Check
+- 最终 Keil Build: 0 Error(s), 0 Warning(s)。
+- 最终 Program Size: data=157.4 xdata=315 const=146 code=14940。
+- 16K code 上限 0x3F80/16256 字节，当前剩余约 1316 字节。
+- ch.c 编码: CP936/GBK, 无 BOM, LF, 未发现 emoji。
+- codex-status.md 编码: UTF-8 无 BOM, LF。
+- implementation-diff.patch 已生成。
+- Keil 构建更新了 Project/Keil/TXLinkLog.txt，未擅自回退。
+## Remove Return Fix
+- 计划来源: 重新读取 .ai-bridge/current-plan.md，目标为修复充电中拔除等待期间重新开输出问题。
+- 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
+- CodeGraph: 已 sync，索引最新；目标函数 ch_battery_removed_check_10ms 只有 usr_ch_func 一个上游调用者。
+- 修复点: 疑似拔除启动后立即 return 1，让主循环 continue，避免本轮继续执行 CH_REPAIR/CH_Pre1/CH_CCCV 重新开输出。
+- 修复点: 20ms 等待期间每轮 ch_output_all_off() 后 return 1，后续状态逻辑不会运行。
+- 修复点: 20ms 后 vout >= vRESET 进入 BMS_ERR 后 return 1，不再继续执行后续充电逻辑。
+- 检查: 未发现 iGED / 2 作为充电中拔除阈值。
+- 编码: ch.c 保持 CP936/GBK, 无 BOM, LF, 未发现 emoji。
+- 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
+- Program Size: data=157.4 xdata=315 const=146 code=14957，距 16K 上限约 1299 字节。
+## Current Plan Run: Remove Wait + C51 Chinese FD Check
+- 计划来源: 已重新读取 .ai-bridge/current-plan.md，本次以该计划为准。
+- 任务1结果: 已确认 ch_battery_removed_check_10ms 修复疑似拔除后 return 0 导致后续充电状态重新开输出的问题。
+- 疑似拔除启动: `val.curr < iGED` 且 `no_rx_10ms >= 100U` 后，立即 ch_output_all_off(), s_remove_cnt=1U, return 1。
+- 20ms 等待: s_remove_cnt != 0U 分支每轮先 ch_output_all_off()，未到 20ms return 1。
+- 20ms 后: `val.vout < vRESET` 回 CH_IDLE；`val.vout >= vRESET` 进入 BMS_ERR；两条路径都 return 1，不继续执行后续充电逻辑。
+- 检查: 未发现 `iGED / 2` 作为充电中拔除阈值。
+- 任务2结果: 已检查 uart_printf/printf/pc_uart_print_current_line/u1w_dbg_put_str 等运行时中文打印字符串中的 C51 0xFD/\xFD 问题。
+- 已补 \xFD: YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c 三处 `" 数据="` 改为 `" 数据\xFD="`。
+- 已确认保留中文，未改英文/ASCII，未修改普通中文注释。
+- 复扫结果: 运行时打印里 GBK 第二字节为 FD 的中文位置均已有 \xFD；pc_uart.c 原有 `过\xFD流恢复`、`放大倍数\xFD` 保持不动。
+- 默认协议日志: U1W_DEBUG_EN=0, U1W_DEBUG_PARSE_EN=0，未打开。
+- CodeGraph: 已 sync；目标函数唯一上游为 usr_ch_func，影响范围集中在 ch.c 状态机接管逻辑。
+- Keil Build: 0 Error(s), 0 Warning(s)。
+- Program Size: data=157.4 xdata=315 const=146 code=14957；按 16K 上限 16256 计算，剩余约 1299 字节。
+- 编码: ch.c 与 uart_1_wire_debug.c 均保持 CP936/GBK、无 BOM、LF，未发现 emoji。
+## 2026-06-28 本轮进度: uart_1_wire 握手标志修复
+- 工作目录: D:\Users\CODE。
+- 已按要求先读取 YAT-60V3A-Uart-1-wire/memory.md，再读取 .ai-bridge/current-plan.md。
+- 已只回退 App/uart_1_wire.c 中 ChatGPT 刚才新增的 u1w_mark_valid_frame(u8 cmd) 及相关调用结构；未回退其它源码文件。
+- 最终 App/uart_1_wire.c 只保留最小补丁: handshake_mask 只在 s_u1w.stage == U1W_STAGE_HANDSHAKE 时更新。
+- 校验失败帧不会调用 u1w_parse_frame()，因此不会更新 handshake_mask；长度不够、未知 CMD 也不会更新 handshake_mask。
+- 非 HANDSHAKE 阶段收到合法帧只刷新数据和 key_age，不再增加 handshake_mask。
+- App/uart_1_wire.c 保持 CP936/GBK、无 BOM、CRLF；未新增中文注释，未批量格式化，未批量转码。
+- 按用户最新规则，后续默认 U1W_DEBUG_EN=1U、U1W_DEBUG_PARSE_EN=1U，用于查看最大 CODE 占用；U1W_DEBUG_CMD_FILTER=0U；需要改 0 由用户自己改。
+- 未改 pc_uart；未改 DBG_BAUD=115200UL；未改 Flash/EEPROM 布局。
+- Keil Build 命令: D:\Keil_v5\UV4\UV4.exe -b Project.uvproj -j0。
+- Keil Build 结果: 0 Error(s), 0 Warning(s)。
+- Program Size: data=163.4 xdata=315 const=146 code=15784。
+- 按 code + const + 128 计算: 15784 + 146 + 128 = 16058，小于 16384，剩余约 326 字节。
+- 已将“后续默认调试打开，需要改 0 由用户自己改”的规则写入 YAT-60V3A-Uart-1-wire/memory.md。
+- memory.md 为未跟踪文件，写入前已备份: YAT-60V3A-Uart-1-wire/memory.md.bak_encoding_20260628_105912。
+- 真机风险: 仍需实测坏校验帧不会推进握手完成、HANDSHAKE 完成后正常切阶段、打开一线日志时串口时序是否影响现场通信。
diff --git a/.ai-bridge/current-plan.md b/.ai-bridge/current-plan.md
index b228b3a..f498d20 100644
--- a/.ai-bridge/current-plan.md
+++ b/.ai-bridge/current-plan.md
@@ -1,593 +1,184 @@
-# 按PDF修正充电保护与拔除判断
+# 只补本轮干净 implementation-diff.patch
 
-Updated: 2026-06-27T10:25:51.372Z
+Updated: 2026-06-28T04:17:43.204Z
 Workspace: D:\Users\CODE
 Target agent: Codex (codex)
 
 ## Plan
 
-# 执行计划：按 PDF 修正充电保护与拔除判断
+# 本轮任务：只补干净的 implementation-diff.patch，不再改源码
 
-## 0. 重要说明
+## 0. 当前状态
 
-本计划覆盖 `.ai-bridge/current-plan.md`。此前追加内容里有一句错误表述：
+ChatGPT Pro 已经检查过本轮源码实现：
 
-```text
-充电中继电器闭合状态：不靠 val.vout 判断拔除，继续使用“小电流 + 1秒无合法一线通信帧”的方式判断拔除。
-```
-
-这句话不能作为最终实现依据。
-
-最终以用户最新确认的流程为准：
-
-```text
-充电中拔出判断：
-通信中断大于 1 秒 + 电流小于 IEND
-=> 先关闭输出
-=> 延时 20ms
-=> 再判断 vout 是否小于 1V
-   是：确认拔电池，回 CH_IDLE
-   否：不是正常拔除，进入异常
-```
-
-## 1. 资料来源
-
-本计划综合：
-
-1. 《一线通信代码实现思路.md》
-2. 《一线通信异步轮询协议重构总结.md》
-3. `63V 3A CHARGER SPECIFICATION 102003001204-V1.0.pdf`
-4. 用户已提供的源码包 `YAT-60V3A-Uart-1-wire`
-5. 用户最新确认的低压插入、拔除、充电中拔除判断逻辑
-
-PDF 是 63V/3A 充电器规格书，重点核对充电规格、保护、LED、恢复逻辑。
-
-## 2. 必须保持的架构原则
-
-继续保持一线通信异步轮询模型：
-
-- 发送不等待对应回复。
-- 每 10ms 调用任务，内部约 100ms 发送一帧。
-- 不恢复“发一帧、等一帧回复、再发下一帧”的同步模型。
-- 不恢复 `wait_cmd / wait_len / wait_start_ms / retry_count / busy / can_send` 这类同步等待变量。
-- 接收只扫描合法帧，数据是否新鲜由计时器判断。
-
-职责边界保持：
-
-- `uart_1_wire.c` 只负责一线通信协议收发、解析、计时、主动拉低/释放 COM。
-- `uart_1_wire.c` 不直接切换充电状态。
-- `ch.c` 负责充电状态机、保护、满电、温度异常、电池插入/拔除。
-- `ch.c` 不解析协议帧原始字节。
-
-## 3. PDF 关键要求
-
-请按 PDF 重点核对：
-
-- 满电截止电压/电流：`63V±0.20V，500±200mA`。
-- 额定输出电流：`3±0.3A`。
-- 过流保护：超过 `3.5±0.3A` 关断输出并报错。
-- 低压保护：低于 `1.0±0.2*N V`，关断输出、报错。
-- 预充：`1.0*N ~ 2.5*N V`，`500±200mA`，`30min±5%`。
-- 涓流/低压修复：`1.0*N ~ 2.0*N V`，`100mA` 以下，`30min±5%`。
-- 过压保护：超过 `4.2±0.02*N V`，关断输出，显示充满。
-- 通信异常保护：电池插入时通信异常，切断输出。
-- 温度异常恢复后可自动重新开始充电。
-- 电流、电压、通信异常时，必须重新插拔电池后才能再次运行。
-- 保护动作时不得有输出电压和电流，即完全关闭。
-- LED：待机红常亮绿灭；充电绿 1Hz；电压异常红 1Hz；温度异常红绿 1Hz 交替；满电绿常亮。
-
-## 4. 当前源码中已确认的关键宏
-
-用户已提供源码包，不要再写“无法确认 IEND”。
-
-当前工程没有独立 `IEND` 宏，已有满电/转灯电流阈值：
-
-```c
-#define iGED (500U)
-```
-
-因此用户口头说的 `IEND`，本工程按 `iGED = 500mA` 理解。
-
-当前重要电压边界：
-
-```text
-vSTART = 15V：待机正常插入/起握手门槛。
-vRESET = 1V：其它非充电输出状态确认拔除回待机门槛。
-vPRE_30V = 30V：低压修复/涓流上限。
-vPRE_37V5 = 37.5V：预充上限。
-SET_vMAX = 63V：满电目标。
-vCH60 = 60V：当前整包回充参考，需要核对是否应改为最高单节 <4.0V。
-```
-
-## 5. 待机 CH_IDLE 插入判断
-
-### 5.1 正常插入
-
-```text
-val.vout >= vSTART，也就是 >=15V：
-    按原逻辑连续确认后进入 BMS_HANDSHAKE。
-```
-
-### 5.2 低压电池插入
-
-用户确认：待机可以读取 `val.vout` 判断低压电池插入。
-
-```text
-vRESET < val.vout < vSTART，也就是 1V～15V：
-    可能是真实低压电池，
-    也可能是刚拔电池后的滤波电容/104 电容残压缓降，
-    也可能是刚插电池时采样节点未稳定。
-```
-
-因此不能一看到 `1V～15V` 就立刻报低压异常。
-
-必须增加独立的待机低压稳定确认逻辑：
-
-```text
-1. 进入 1V～15V 区间后，启动低压候选计数。
-2. 连续一段时间保持在 1V～15V，且电压变化不再明显下降/上升，再判定为真实低压电池。
-3. 如果电压继续下降到 <=1V，认为是残压放空/无电池，清计数，保持待机。
-4. 如果电压上升到 >=15V，认为是正常电池插入，清低压候选计数，走原握手流程。
-5. 低压确认后进入低压异常状态，输出关闭，红灯 1Hz 闪烁，绿灯熄灭。
-```
-
-限制：
-
-- 低压候选计数不要复用 `s_remove_cnt`。
-- 不要把刚插入瞬间的采样过渡值判为低压异常。
-- 不要把刚拔出后的残压缓降判为低压异常。
-
-## 6. 其它非充电输出状态拔除判断
-
-保持原原则：
-
-```text
-val.vout < vRESET，也就是 <1V，才确认电池拔除并回 CH_IDLE。
-```
-
-这与待机 1V～15V 低压插入识别不冲突，因为场景不同。
-
-## 7. 充电中继电器闭合状态拔除判断：最终以本节为准
+1. `YAT-60V3A-Uart-1-wire/App/uart_1_wire.c` 当前逻辑基本正确；
+2. `u1w_key_index()` 已删除；
+3. `u1w_parse_frame()` 中 A0/A1/A4/A6/A7/B1/B3/B4 已在各自 case 内直接设置 `key`；
+4. `key_age_10ms` 在 switch 之后统一刷新；
+5. `handshake_mask` 只在 `s_u1w.stage == U1W_STAGE_HANDSHAKE` 时更新；
+6. `u1w_rx_task()` 的校验流程没有被修改，仍然只有校验正确才调用 `u1w_parse_frame()`；
+7. Keil 编译结果已确认：0 Error(s), 0 Warning(s)，Program Size 为 `data=163.4 xdata=315 const=146 code=15752`；
+8. `code + const + 128 = 15752 + 146 + 128 = 16026`，小于 16384，剩余 358 字节。
 
-### 7.1 禁止使用的旧逻辑
+所以当前不要再改源码。
 
-不要再使用下面旧逻辑作为最终判断：
+## 1. 本轮只允许做什么
 
-```text
-小电流 + 1 秒无合法一线通信帧 => 直接确认拔除并回待机
-```
-
-也不要再写：
+本轮只允许补交付文件：
 
 ```text
-充电中继电器闭合状态不靠 val.vout 判断拔除，继续使用小电流 + 1秒无通信判断拔除。
+.ai-bridge/implementation-diff.patch
+.ai-bridge/codex-status.md
 ```
 
-这不是最终需求。
+其中重点是重新生成 `.ai-bridge/implementation-diff.patch`。
 
-### 7.2 正确的新流程
+## 2. 本轮禁止做什么
 
-用户最新确认的稳定流程：
+严禁修改以下源码或配置：
 
 ```text
-通信中断 > 1 秒
-并且电流小于 IEND
-=> 先关闭输出
-=> 延时 20ms
-=> 再判断 vout 是否小于 1V
-   是：确认电池拔除，回 CH_IDLE
-   否：不是正常拔除，进入异常
-```
-
-本工程中 `IEND` 使用：
-
-```c
-iGED = 500U
-```
-
-所以疑似拔除条件应为：
-
-```c
-s_u1w_info.no_rx_10ms >= 100U
-val.curr < iGED
+YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
+YAT-60V3A-Uart-1-wire/App/ch.c
+YAT-60V3A-Uart-1-wire/App/pc_uart.c
+YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
+YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
+YAT-60V3A-Uart-1-wire/App/usr_cfg.h
+YAT-60V3A-Uart-1-wire/memory.md
+YAT-60V3A-Uart-1-wire/memory.md.bak_encoding_20260628_105912
 ```
 
-不要再使用旧条件：
+严禁再做：
 
-```c
-val.curr < (u16)(iGED / 2U)
-```
+1. 重新改 `u1w_parse_frame()`；
+2. 重新改握手逻辑；
+3. 删除或修改 `u1w_dbg_parse_ok()`；
+4. 改 `ch.c`；
+5. 改 `pc_uart.c`；
+6. 改默认调试开关；
+7. 改编码；
+8. 运行会自动改源码的格式化工具；
+9. 混入旧任务的 `ch.c` 大 diff。
 
-### 7.3 两阶段实现要求
+## 3. 为什么要补 patch
 
-建议实现为两阶段，避免在一个 10ms 周期内直接判断：
+ChatGPT Pro 发现当前 `.ai-bridge/implementation-diff.patch` 不是本轮干净 diff，里面混入了旧任务的 `ch.c` 大 diff。
 
-```text
-阶段 1：疑似拔除
-    条件：no_rx_10ms >= 100U，并且 val.curr < iGED。
-    动作：ch_output_all_off();
-          启动 20ms 等待计数。
-
-阶段 2：关闭输出后确认
-    等待 20ms 后，确认 ADC/val.vout 已更新。
-    如果 val.vout < vRESET，也就是 <1V：
-        确认拔除；
-        BATT_DIVIDER_EN = 0；
-        uart_1_wire_reset_link();
-        ch_vout_probe_reset();
-        ch_set_state(CH_IDLE, "拔电池");
-    否则：
-        认为不是正常拔除；
-        保持输出关闭；
-        进入 BMS_ERR 或当前工程等价普通异常状态；
-        后续通信阶段走 PULL_LOW。
-```
+这会影响后续审查和交接，所以要重新生成一个只对应本轮的干净 patch。
 
-注意：如果 20ms 内 ADC 采样未必更新，应按工程 ADC 周期选择最小可靠等待时间，但交付说明必须写明。
+## 4. implementation-diff.patch 应包含什么
 
-## 8. B4 状态位处理
+新的 `.ai-bridge/implementation-diff.patch` 只应包含本轮相关内容：
 
-B4 状态位按以下分类处理：
+必须包含：
 
 ```text
-bit0 单节过充：按 PDF，切断输出并显示充满，优先进入 CH_FULL 或等价满电状态。
-bit1 低温：进入 BMS_TEMP_ERR，继续通信等待恢复，不拉低 COM。
-bit2 高温：进入 BMS_TEMP_ERR，继续通信等待恢复，不拉低 COM。
-bit3 MOS 过温：进入 BMS_TEMP_ERR，继续通信等待恢复，不拉低 COM。
-bit4 过流：进入普通异常/BMS_ERR，输出关闭，PULL_LOW。
-bit5 短路：进入普通异常/BMS_ERR，输出关闭，PULL_LOW。
-bit6 充电超时：进入普通异常/BMS_ERR，输出关闭，PULL_LOW。
-bit7 电池包失效：进入普通异常/BMS_ERR，输出关闭，PULL_LOW。
+YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
 ```
 
-限制：
-
-- 不新增“根据 B4 异常发送 B6 关闭 BMS MOS”的动作。
-- BMS 普通异常由 BMS 自己处理 MOS。
-- 充电器只停止自己的充电输出，并按状态机进入满电、温度等待或普通异常。
-
-## 9. 满电与回充
-
-保持满电流程：
-
-- 关闭充电输出。
-- LED 满电：红灯灭，绿灯常亮。
-- 通信进入 `FULL_DISPLAY`，周期发送 `B6 03 SOC`，持续 3 分钟。
-- 3 分钟后主动拉低 COM。
-
-回充判断需核对 PDF：
+可以包含，如果你确实更新了状态：
 
 ```text
-滞后恢复充电电压：最高单节电压小于 4.0±0.05V。
+.ai-bridge/codex-status.md
 ```
 
-如果一线通信中有最高单节电压字段，优先按最高单节 `< CELL_RECHG_MV` 判断。若由于满电后停止通信无法实时获取最高单节，只能用整包电压近似，必须在 `agent-status.md` 说明差异。
-
-回充前必须重新握手，不允许复用旧数据继续充。
-
-## 10. LED 1Hz
-
-检查 `Ged_Flash / Red_Flash / RGed_Flash` 的实际周期。
-
-必须符合 PDF：
+可以包含，如果 current-plan 本身被更新后需要记录：
 
 ```text
-待机：红灯常亮，绿灯灭。
-充电：绿灯 1Hz 闪烁，红灯灭。
-电压异常：红灯 1Hz 闪烁，绿灯灭。
-温度异常：红绿 1Hz 交替闪烁。
-满电：红灯灭，绿灯常亮。
+.ai-bridge/current-plan.md
 ```
 
-如果 `Flash(50)` 实际不是 1Hz，应做最小修改。优先只改参数或 Flash 内部周期定义，不要大改 LED 状态机。
-
-## 11. 不允许做的事
-
-- 不要实现 `A2 / A3 / A5 / B0 / B2 / B5 / B7 / B8`。
-- 不要扩大到整份 X6 V1.3 协议实现。
-- 不要修改字节序，用户已确认低字节在前。
-- 不要修改 `u1w_get_u16_bus()` 的低字节在前逻辑。
-- 不要恢复同步等待/单帧重试模型。
-- 不要新增复杂状态机。
-- 不要让协议层直接跳充电状态。
-- 不要让 `ch.c` 解析协议帧字节。
-- 不要新增 B6 关闭 BMS MOS 动作。
-- 不要默认打开协议收发日志。
-- 不要自动格式化整文件。
-- 不要整文件重写。
-- 不要转码源码。
-- 不要改 UART 波特率、半双工方向、帧格式、CHK 算法。
-
-## 12. 编码和 Git 保护
-
-开始源码修改前必须：
+但不要把历史旧任务混进来。
 
-1. 读取并遵守 `AGENTS.md` 和 `USER.md`。
-2. 查看 Git 状态，确认已有改动，不能覆盖用户改动。
-3. 对目标 `.c/.h` 文件做编码预检。
-4. 源码中文注释可能是 GBK/CP936/ANSI，禁止普通 UTF-8 写回导致乱码。
-5. 保持原编码、原 BOM、原行尾。
-6. 只做最小修改。
-7. 修改后检查 diff，确认没有整文件转码、乱码、格式化。
-
-## 13. 建议修改文件
-
-优先修改：
+尤其不要包含：
 
 ```text
 YAT-60V3A-Uart-1-wire/App/ch.c
-```
-
-必要时才修改：
-
-```text
-YAT-60V3A-Uart-1-wire/App/uart_1_wire.h
-YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
+YAT-60V3A-Uart-1-wire/App/pc_uart.c
 YAT-60V3A-Uart-1-wire/App/usr_cfg.h
+YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
+YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
+YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt
+YAT-60V3A-Uart-1-wire/Project/Keil/*.log
+YAT-60V3A-Uart-1-wire/memory.md
 ```
 
-一般不要修改：
-
-```text
-Bsp/uart.c
-Bsp/uart.h
-fifo.c/fifo.h
-```
-
-## 14. 验收要求
-
-完成后至少验证：
-
-1. Keil Rebuild，若无法构建，说明原因。
-2. `TASK_10MS` 是否确实 10ms。
-3. 满电截止：63V、500mA 是否仍对齐。
-4. 预充：500mA、30min 是否仍对齐。
-5. 涓流/低压修复：15V～30V、100mA 是否符合当前设计。
-6. 低压插入：待机 1V～15V 是否稳定确认后报低压异常。
-7. 低压插入是否避开刚拔电池残压缓降。
-8. 低压插入是否避开刚插电池采样过渡。
-9. 其它非充电输出状态是否仍保持 `<1V` 回待机。
-10. 充电中拔除是否已改为：通信中断 >1 秒 + 电流 < iGED -> 关输出 -> 等待 20ms -> vout 判断。
-11. 20ms 后 `vout <1V` 是否回 `CH_IDLE`。
-12. 20ms 后 `vout >=1V` 是否进入异常。
-13. 是否没有继续使用 `iGED / 2` 作为充电中拔除电流阈值。
-14. B4 单节过充是否显示满电。
-15. B4 温度类故障是否进入 `BMS_TEMP_ERR`，继续通信，不拉低 COM。
-16. B4 过流/短路/充电超时/失效是否进入普通异常/PULL_LOW。
-17. 是否没有新增 B6 关闭 BMS MOS 动作。
-18. LED 是否符合 1Hz 要求。
-19. 保护动作时是否保持无输出电压和电流。
-20. 电压/电流/通信异常是否必须重新插拔电池后才能再次运行。
-21. 满电回充是否按最高单节电压，或明确说明当前用整包电压近似的差异。
-22. Git diff 是否只包含必要修改。
-23. 编码是否未变化。
-
-## 15. 交付输出要求
-
-完成后请在 `.ai-bridge/agent-status.md` 用中文说明：
-
-1. 修改了哪些文件。
-2. 充电中拔除新流程具体如何实现。
-3. `IEND` 实际使用 `iGED=500mA`。
-4. 20ms 后 vout 判断结果对应的状态去向。
-5. 待机低压插入稳定确认条件。
-6. 如何避免刚拔电池残压误判。
-7. 如何避免刚插电池采样过渡误判。
-8. B4 每一类故障如何处理。
-9. PDF 每个关键条款的对齐结果。
-10. 是否保持异步轮询和低字节在前。
-11. 是否没有新增 B6 关闭 BMS MOS。
-12. Keil Rebuild 或检查结果。
-13. 编码保护结果。
-14. Git diff 摘要。
-15. 无法确认的问题必须列出来，不要擅自猜。
-
-## Implementation contract
-
-- Work from this plan in small, reviewable steps.
-- Keep edits scoped to the requested task and existing project conventions.
-- Run focused verification before handing work back.
-- Update .ai-bridge/agent-status.md with files touched, checks run, results, blockers, and review notes.
-- Save the final review diff to .ai-bridge/implementation-diff.patch when practical.
-- Append notable execution events to .ai-bridge/execution-log.jsonl when the implementation agent supports logging.
-
----
-
-# 追加16K容量约束
-
-Updated: 2026-06-27T10:30:16.812Z
-Workspace: D:\Users\CODE
-Target agent: Codex (codex)
-
-## Plan
-
-# 追加硬约束：单片机 16K 容量限制
-
-用户确认：当前单片机程序空间只有 16K，校准存储还会占用一部分空间。后续修改必须把代码容量作为硬约束，不能按 PC 程序思路大改。
-
-## 1. 容量约束
-
-本任务所有修改必须遵守：
-
-```text
-单片机总容量约 16K。
-校准存储已经占用一部分空间。
-必须避免写超程序空间。
-```
-
-因此实现时必须优先选择“小补丁、少代码、少 RAM、少字符串”的方案。
-
-## 2. 禁止膨胀代码
-
-禁止为了实现本任务而引入明显增大代码体积的写法：
-
-- 不要新增复杂状态机框架。
-- 不要新增大表格、大数组、大量查表。
-- 不要新增大量 debug 字符串。
-- 不要新增大量 `uart_printf` / 日志打印。
-- 不要新增长中文字符串到源码。
-- 不要为了可读性拆出太多小函数，导致调用和代码体积增加。
-- 不要整文件重构。
-- 不要把简单判断写成复杂通用框架。
-- 不要引入浮点计算。
-- 不要引入动态内存。
-- 不要新增无关功能。
-
-## 3. 推荐实现方式
-
-优先采用最省空间的写法：
-
-- 尽量复用现有变量、计数器和状态枚举，但不能造成逻辑混乱。
-- 必须新增计数器时，优先使用 `u8`，确认范围足够后再用 `u16`。
-- 低压插入稳定确认、充电中拔除两阶段确认，尽量在现有 `ch.c` 流程里用少量静态变量实现。
-- B4 故障分类优先用简单位判断，不要做复杂映射表。
-- LED 1Hz 修正优先改参数或现有函数，不新增新 LED 框架。
-- 日志只保留必要状态，不新增刷屏日志。
-- 所有新增注释简短，不要写大段说明进 `.c` 文件。
-
-## 4. 编译后必须看容量
-
-完成修改后，必须检查 Keil 编译输出中的程序容量/ROM 使用情况。
-
-交付时在 `agent-status.md` 写清楚：
-
-1. 编译是否通过。
-2. Code/ROM 使用量是多少。
-3. 距离 16K 还剩多少余量。
-4. 校准存储是否可能受影响。
-5. 如果已经接近 16K，必须优先删减新增日志、字符串和非必要函数。
-
-## 5. 代码取舍优先级
-
-如果容量不够，按以下优先级取舍：
-
-```text
-必须保留：安全保护、关输出、拔除判断、通信异常、低压异常、温度异常。
-可以压缩：日志、长注释、重复函数、重复状态字符串。
-不能牺牲：保护动作时无输出、充电中拔除判断、B4 故障处理、编码不转码。
-```
-
-## 6. 对 Codex 的特别提醒
-
-这不是桌面软件，也不是资源充足的 MCU。代码要保守，能少写就少写，能复用就复用，能用简单 if 判断就不要做通用框架。最终以能编译、容量不超、保护可靠为第一优先级。
-
-## Implementation contract
-
-- Work from this plan in small, reviewable steps.
-- Keep edits scoped to the requested task and existing project conventions.
-- Run focused verification before handing work back.
-- Update .ai-bridge/agent-status.md with files touched, checks run, results, blockers, and review notes.
-- Save the final review diff to .ai-bridge/implementation-diff.patch when practical.
-- Append notable execution events to .ai-bridge/execution-log.jsonl when the implementation agent supports logging.
+## 5. 推荐生成方式
 
----
+不要直接用全仓库 `git diff` 覆盖 patch，因为工作区里有很多历史未提交内容。
 
-# 追加资料优先级规则
-
-Updated: 2026-06-27T10:33:25.574Z
-Workspace: D:\Users\CODE
-Target agent: Codex (codex)
-
-## Plan
-
-# 追加硬约束：两份一线通信 MD 必须阅读，但冲突以当前计划为准
-
-用户确认：Codex 修改前必须阅读以下两份资料：
+推荐只针对本轮文件生成：
 
 ```text
-一线通信代码实现思路.md
-一线通信异步轮询协议重构总结.md
+git diff -- YAT-60V3A-Uart-1-wire/App/uart_1_wire.c .ai-bridge/codex-status.md > .ai-bridge/implementation-diff.patch
 ```
 
-这两份资料是理解一线通信异步轮询架构、职责边界、通信阶段、发送/接收模型的重要背景，必须阅读。
-
-## 1. 两份 MD 的作用
-
-两份 MD 用于理解：
-
-- 为什么一线通信改成异步轮询。
-- 为什么 `uart_1_wire.c` 只负责协议。
-- 为什么 `ch.c` 只负责充电状态机。
-- 为什么不恢复同步等待应答模型。
-- 为什么发送按阶段轮询，不等回复。
-- 为什么接收只扫描合法帧并刷新数据。
-- 为什么 BMS 普通异常不由充电器主动关闭 BMS MOS。
-- 为什么温度异常继续通信等待恢复。
-- 为什么满电使用 `FULL_DISPLAY` 发送 `B6 03 SOC`。
+如果你的工具不允许重定向，就用等价方式写入，但内容必须只来自这两个路径的 diff。
 
-这些原则仍然有效。
-
-## 2. 优先级规则
-
-如果两份 MD 与当前 `current-plan.md` 或用户最新确认内容发生冲突，必须按以下优先级执行：
+生成后必须检查 patch 内容，确认没有出现：
 
 ```text
-第一优先级：用户本轮最新明确确认的要求。
-第二优先级：当前 .ai-bridge/current-plan.md 中的最终计划。
-第三优先级：PDF 规格书。
-第四优先级：两份一线通信 MD 的背景原则。
-第五优先级：当前源码已有实现。
+App/ch.c
+App/pc_uart.c
+App/usr_cfg.h
+uart_1_wire_debug.c
+uart_1_wire_debug.h
+Project/Keil
+memory.md
 ```
 
-也就是说：
-
-```text
-两份 MD 必须看，但不能覆盖当前最新计划。
-与当前计划冲突时，以当前计划为准。
-```
+如果出现这些路径，说明 patch 不干净，必须重新生成。
 
-## 3. 已知必须以当前计划为准的冲突点
+## 6. 对本轮源码的复核要求，只读，不写
 
-### 3.1 充电中拔出判断
+生成 patch 后，只读确认下面几项：
 
-如果两份 MD 或旧计划里还有类似说法：
+1. 搜索 `u1w_key_index`，应无结果；
+2. 读取 `u1w_parse_frame()` 片段，确认 `key = U1W_KEY_MAX;` 在 switch 前；
+3. 确认 A0/A1/A4/A6/A7/B1/B3/B4 的 case 内各自设置 `key`；
+4. 确认 B6 不设置 `key`；
+5. 确认 switch 后统一执行：
 
-```text
-充电中继电器闭合状态不靠 val.vout 判断拔除，继续使用“小电流 + 1秒无合法一线通信帧”判断拔除。
-```
-
-这条已经作废，不能执行。
-
-最终以当前计划为准：
-
-```text
-通信中断 > 1 秒
-并且电流 < IEND，也就是当前工程 iGED=500mA
-=> 先关闭输出
-=> 延时 20ms
-=> 再判断 vout 是否小于 1V
-   vout < 1V：确认拔电池，回 CH_IDLE
-   vout >= 1V：不是正常拔除，进入异常
+```c
+if(key < U1W_KEY_MAX)
+{
+    s_u1w.key_age_10ms[key] = 0U;
+    if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
+    {
+        uart_1_wire.handshake_mask |= (u8)(1U << key);
+    }
+}
 ```
 
-### 3.2 待机低压插入判断
+这里只读检查，不允许改源码。
 
-如果两份 MD 没有提到待机 `1V～15V` 低压电池识别，不代表不需要做。
+## 7. 本轮报告格式
 
-最终以当前计划为准：
+完成后只报告下面内容，不要贴大量命令流水账：
 
 ```text
-CH_IDLE 待机可以用 val.vout 判断 1V～15V 低压电池插入，
-但必须避开刚拔电池后的残压缓降，
-也必须避开刚插电池时采样未稳定的过渡区，
-确认稳定后才进入低压异常。
+1. 是否只更新了 .ai-bridge/implementation-diff.patch 和必要状态文件；
+2. patch 中包含哪些文件；
+3. patch 中是否排除了 ch.c、pc_uart.c、memory.md、Keil 日志等无关内容；
+4. 是否确认 u1w_key_index 已无残留；
+5. 是否确认 uart_1_wire.c 逻辑仍保持本轮结果；
+6. 是否没有再改源码；
+7. 如更新 codex-status.md，说明新增了哪一小段。
 ```
 
-### 3.3 16K 容量限制
-
-如果两份 MD 更偏向结构清晰、拆函数、加注释，本项目仍必须优先遵守 16K 容量限制。
+## 8. 额外通用规则
 
-最终以当前计划为准：
+ChatGPT Pro 已经在 `AGENTS.md` 追加第 26 节“小范围修改的高效执行规则”。
 
-```text
-小改、少变量、少字符串、少函数、少日志，
-不得写超 16K，
-Keil 编译后必须检查 Code/ROM 使用量。
-```
+本轮你应先读取 `AGENTS.md` 第 26 节，再执行本计划。
 
-## 4. Codex 执行要求
+重点记住：
 
-Codex 开始修改前请先阅读两份 MD，但执行时必须以当前计划为最终约束。遇到冲突不要自行猜，按当前计划；如果仍无法判断，写入 `agent-status.md` 作为问题，不要硬改。
+1. 小范围修改必须先限定函数范围，不能全文件替换；
+2. CP936/GBK 文件不能用默认 UTF-8 方式硬写；
+3. 计划外 BUG 只记录，不顺手改；
+4. `implementation-diff.patch` 必须对应本轮任务，不能混入旧任务 diff。
 
 ## Implementation contract
 
diff --git a/.ai-bridge/execution-log.jsonl b/.ai-bridge/execution-log.jsonl
index 6bd4ad5..3e8d5ac 100644
--- a/.ai-bridge/execution-log.jsonl
+++ b/.ai-bridge/execution-log.jsonl
@@ -8,3 +8,20 @@
 {"ts":"2026-06-27T10:25:51.406Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"按PDF修正充电保护与拔除判断","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 {"ts":"2026-06-27T10:30:16.822Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"追加16K容量约束","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 {"ts":"2026-06-27T10:33:25.582Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"追加资料优先级规则","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T14:22:17.262Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修复充电中拔除等待期间重新开输出问题","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T14:34:21.317Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"补充检查C51打印中文乱码","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T14:38:13.246Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修正打印检查要求：保留中文","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T14:44:46.655Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修复拔除等待与C51中文打印0xFD检查","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T15:34:54.953Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"优化一线协议调试短日志","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T15:38:49.833Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"优化一线协议调试可读短日志","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T16:04:36.715Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修复一线调试日志全开超16K","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T01:09:00.096Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志交接","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T01:09:11.566Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志交接追加","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T01:09:18.989Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志交接追加","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T01:09:30.864Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志最终交接","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T02:37:10.558Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"修复一线握手标志健壮性","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T02:39:57.763Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Revert ChatGPT uart_1_wire.c change then fix safely","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T03:38:13.038Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Simplify 1-wire handshake code only","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T03:38:48.503Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Simplify 1-wire handshake code only","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T03:49:49.086Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Codex restart alignment then simplify 1-wire handshake","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T04:17:43.213Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"只补本轮干净 implementation-diff.patch","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
diff --git a/.ai-bridge/implementation-diff.patch b/.ai-bridge/implementation-diff.patch
index e69de29..98885dc 100644
--- a/.ai-bridge/implementation-diff.patch
+++ b/.ai-bridge/implementation-diff.patch
@@ -0,0 +1,115 @@
+diff --git a/YAT-60V3A-Uart-1-wire/App/uart_1_wire.c b/YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
+index 058d0d9..acca033 100644
+--- a/YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
++++ b/YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
+@@ -202,22 +202,6 @@ static u8 u1w_reply_len(u8 cmd)
+     }
+ }
+ 
+-static u8 u1w_key_index(u8 cmd)
+-{
+-    switch(cmd)
+-    {
+-    case U1W_CMD_A0: return U1W_KEY_A0;
+-    case U1W_CMD_A1: return U1W_KEY_A1;
+-    case U1W_CMD_A4: return U1W_KEY_A4;
+-    case U1W_CMD_A6: return U1W_KEY_A6;
+-    case U1W_CMD_A7: return U1W_KEY_A7;
+-    case U1W_CMD_B1: return U1W_KEY_B1;
+-    case U1W_CMD_B3: return U1W_KEY_B3;
+-    case U1W_CMD_B4: return U1W_KEY_B4;
+-    default:         return U1W_KEY_MAX;
+-    }
+-}
+-
+ static void u1w_refresh_target_current(void)
+ {
+     u16 target_ma;
+@@ -254,16 +238,12 @@ static void u1w_parse_frame(u8 *frame)
+     uart_1_wire.offline_count_10ms = 0U;
+     s_u1w.any_rx_age_10ms = 0U;
+ 
+-    key = u1w_key_index(cmd);
+-    if(key < U1W_KEY_MAX)
+-    {
+-        s_u1w.key_age_10ms[key] = 0U;
+-        uart_1_wire.handshake_mask |= (u8)(1U << key);
+-    }
++    key = U1W_KEY_MAX;
+ 
+     switch(cmd)
+     {
+     case U1W_CMD_A0:
++        key = U1W_KEY_A0;
+         /* A0 ZZ XY CHK��ZZ=ʶ���룬X=�������룬Y=�������롣 */
+         uart_1_wire.pack_id = frame[1];
+         xy = frame[2];
+@@ -281,11 +261,13 @@ static void u1w_parse_frame(u8 *frame)
+         break;
+ 
+     case U1W_CMD_A1:
++        key = U1W_KEY_A1;
+         /* A1 XX YY CHK��YY=���� 0.1Ah�� */
+         uart_1_wire.cell_cap_01ah = frame[2];
+         break;
+ 
+     case U1W_CMD_A4:
++        key = U1W_KEY_A4;
+         /* A4 xxxx yyyy CHK��xxxx=Ԥ���ֹ��yyyy=���䣬��λ0.01V�� */
+         uart_1_wire.cell_pre_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[1], frame[2]));
+         pack_mv = (u32)u1w_001v_to_mv(u1w_get_u16_bus(frame[3], frame[4])) *
+@@ -298,6 +280,7 @@ static void u1w_parse_frame(u8 *frame)
+         break;
+ 
+     case U1W_CMD_A6:
++        key = U1W_KEY_A6;
+         /* A6 xxxx yyyy CHK��xxxx=����о��������0.1A�� */
+         uart_1_wire.max_charge_current_ma =
+             U1W_LIMIT_CURRENT(u1w_cell_01a_to_pack_ma(u1w_get_u16_bus(frame[1], frame[2])));
+@@ -305,6 +288,7 @@ static void u1w_parse_frame(u8 *frame)
+         break;
+ 
+     case U1W_CMD_A7:
++        key = U1W_KEY_A7;
+         /* A7 XX YY 00 ZZ CHK��XX/YY=����������ZZ=����о��������0.1A�� */
+         uart_1_wire.derate_low_degc = (s8)frame[1];
+         uart_1_wire.derate_high_degc = (s8)frame[2];
+@@ -313,12 +297,14 @@ static void u1w_parse_frame(u8 *frame)
+         break;
+ 
+     case U1W_CMD_B1:
++        key = U1W_KEY_B1;
+         /* B1 xxxx yyyy CHK��xxxx=��͵��ڣ�yyyy=��ߵ��ڣ���λ0.01V�� */
+         uart_1_wire.cell_min_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[1], frame[2]));
+         uart_1_wire.cell_max_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[3], frame[4]));
+         break;
+ 
+     case U1W_CMD_B3:
++        key = U1W_KEY_B3;
+         /* B3 XX YY CHK��XX=����¶ȣ�YY=��� MOS �¶ȡ� */
+         uart_1_wire.batt_temp_degc = (s8)frame[1];
+         uart_1_wire.mos_temp_degc = (s8)frame[2];
+@@ -326,6 +312,7 @@ static void u1w_parse_frame(u8 *frame)
+         break;
+ 
+     case U1W_CMD_B4:
++        key = U1W_KEY_B4;
+         /* B4 XX YY CHK��XX=SOC��YY=״̬λ��״̬λֻ��¼����ֱ�ӿ���B6�� */
+         uart_1_wire.soc_percent = frame[1];
+         if(uart_1_wire.soc_percent > 100U)
+@@ -343,6 +330,15 @@ static void u1w_parse_frame(u8 *frame)
+         break;
+     }
+ 
++    if(key < U1W_KEY_MAX)
++    {
++        s_u1w.key_age_10ms[key] = 0U;
++        if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
++        {
++            uart_1_wire.handshake_mask |= (u8)(1U << key);
++        }
++    }
++
+     if((uart_1_wire.handshake_mask & U1W_HANDSHAKE_MASK) == U1W_HANDSHAKE_MASK)
+     {
+         uart_1_wire.link_state = U1W_LINK_ONLINE;
diff --git a/.ai-bridge/session-log.jsonl b/.ai-bridge/session-log.jsonl
index c49e950..a71feb2 100644
--- a/.ai-bridge/session-log.jsonl
+++ b/.ai-bridge/session-log.jsonl
@@ -8,3 +8,20 @@
 {"ts":"2026-06-27T10:25:51.404Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"按PDF修正充电保护与拔除判断","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 {"ts":"2026-06-27T10:30:16.821Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"追加16K容量约束","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 {"ts":"2026-06-27T10:33:25.582Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"追加资料优先级规则","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T14:22:17.261Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修复充电中拔除等待期间重新开输出问题","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T14:34:21.317Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"补充检查C51打印中文乱码","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T14:38:13.245Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修正打印检查要求：保留中文","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T14:44:46.654Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修复拔除等待与C51中文打印0xFD检查","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T15:34:54.952Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"优化一线协议调试短日志","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T15:38:49.833Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"优化一线协议调试可读短日志","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-27T16:04:36.714Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修复一线调试日志全开超16K","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T01:09:00.095Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志交接","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T01:09:11.565Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志交接追加","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T01:09:18.988Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志交接追加","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T01:09:30.864Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志最终交接","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T02:37:10.555Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"修复一线握手标志健壮性","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T02:39:57.761Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Revert ChatGPT uart_1_wire.c change then fix safely","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T03:38:13.037Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Simplify 1-wire handshake code only","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T03:38:48.502Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Simplify 1-wire handshake code only","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T03:49:49.085Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Codex restart alignment then simplify 1-wire handshake","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
+{"ts":"2026-06-28T04:17:43.212Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"只补本轮干净 implementation-diff.patch","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
diff --git a/CHECK_CODEXPRO_WORKSPACE_ASCII_STAY.bat b/CHECK_CODEXPRO_WORKSPACE_ASCII_STAY.bat
deleted file mode 100644
index cbfe48b..0000000
--- a/CHECK_CODEXPRO_WORKSPACE_ASCII_STAY.bat
+++ /dev/null
@@ -1,104 +0,0 @@
-@echo off
-if /I "%~1" NEQ "__RUN__" (
-  start "CodexPro Check" cmd.exe /k ""%~f0" __RUN__"
-  exit /b
-)
-
-cd /d "%~dp0"
-
-set "LOG=%~dp0codexpro_workspace_check.log"
-
-echo ===============================================
-echo CodexPro local workspace check
-echo This window should NOT close automatically.
-echo Current folder: %cd%
-echo Log file: %LOG%
-echo ===============================================
-echo.
-
-echo =============================================== > "%LOG%"
-echo CodexPro local workspace check >> "%LOG%"
-echo Date: %date% %time% >> "%LOG%"
-echo Script folder: %~dp0 >> "%LOG%"
-echo Current folder: %cd% >> "%LOG%"
-echo =============================================== >> "%LOG%"
-echo. >> "%LOG%"
-
-echo [1/7] Windows and path
-echo. >> "%LOG%"
-echo --- 1. Windows and path --- >> "%LOG%"
-ver >> "%LOG%" 2>&1
-echo USERPROFILE=%USERPROFILE% >> "%LOG%"
-echo OneDrive=%OneDrive% >> "%LOG%"
-echo CURRENT=%cd% >> "%LOG%"
-echo SCRIPT=%~dp0 >> "%LOG%"
-
-echo [2/7] Workspace files
-echo. >> "%LOG%"
-echo --- 2. Workspace files --- >> "%LOG%"
-if exist ".git" (echo OK: .git found >> "%LOG%") else (echo WARN: .git not found >> "%LOG%")
-if exist "package.json" (echo OK: package.json found >> "%LOG%") else (echo INFO: package.json not found >> "%LOG%")
-if exist "AGENTS.md" (echo OK: AGENTS.md found >> "%LOG%") else (echo INFO: AGENTS.md not found >> "%LOG%")
-if exist "README.md" (echo OK: README.md found >> "%LOG%") else (echo INFO: README.md not found >> "%LOG%")
-
-echo [3/7] Git
-echo. >> "%LOG%"
-echo --- 3. Git --- >> "%LOG%"
-where git >> "%LOG%" 2>&1
-if errorlevel 1 (
-  echo FAIL: git not found >> "%LOG%"
-) else (
-  git --version >> "%LOG%" 2>&1
-  git status --short >> "%LOG%" 2>&1
-)
-
-echo [4/7] Node and npm
-echo. >> "%LOG%"
-echo --- 4. Node and npm --- >> "%LOG%"
-where node >> "%LOG%" 2>&1
-if errorlevel 1 (echo FAIL: node not found >> "%LOG%") else (node -v >> "%LOG%" 2>&1)
-
-where npm >> "%LOG%" 2>&1
-if errorlevel 1 (
-  echo FAIL: npm not found >> "%LOG%"
-) else (
-  npm -v >> "%LOG%" 2>&1
-  npm config get prefix >> "%LOG%" 2>&1
-)
-
-echo [5/7] Python
-echo. >> "%LOG%"
-echo --- 5. Python --- >> "%LOG%"
-where python >> "%LOG%" 2>&1
-if errorlevel 1 (echo INFO: python not found >> "%LOG%") else (python --version >> "%LOG%" 2>&1)
-where py >> "%LOG%" 2>&1
-if errorlevel 1 (echo INFO: py launcher not found >> "%LOG%") else (py --version >> "%LOG%" 2>&1)
-
-echo [6/7] Codex and CodexPro
-echo. >> "%LOG%"
-echo --- 6. Codex and CodexPro --- >> "%LOG%"
-where codexpro >> "%LOG%" 2>&1
-if errorlevel 1 (echo WARN: codexpro command not found >> "%LOG%") else (codexpro --version >> "%LOG%" 2>&1)
-where codex >> "%LOG%" 2>&1
-if errorlevel 1 (echo WARN: codex command not found >> "%LOG%") else (codex --version >> "%LOG%" 2>&1)
-
-echo [7/7] npm global packages
-echo. >> "%LOG%"
-echo --- 7. npm global packages --- >> "%LOG%"
-where npm >nul 2>&1
-if errorlevel 1 (
-  echo INFO: npm missing, skip global package list >> "%LOG%"
-) else (
-  npm list -g --depth=0 >> "%LOG%" 2>&1
-)
-
-echo.
-echo ===============================================
-echo DONE
-echo Log saved:
-echo %LOG%
-echo ===============================================
-echo.
-echo Please send codexpro_workspace_check.log to ChatGPT.
-echo.
-pause
diff --git a/YAT-60V3A-Uart-1-wire/App/ch.c b/YAT-60V3A-Uart-1-wire/App/ch.c
index f30d86f..dae4a3d 100644
--- a/YAT-60V3A-Uart-1-wire/App/ch.c
+++ b/YAT-60V3A-Uart-1-wire/App/ch.c
@@ -107,7 +107,7 @@ static void ch_set_state(CH_STATUS_Types next_state, char *reason)
      * 1. ���ٶ�� printf ���ú��ظ���ʽ�ַ��������� C51 ���������
      * 2. NTC ������Χ�� int �ڣ��� s16 ��ӡ���������� long printf ��ʽ��
      */
-    uart_printf("״̬:%s��%s ԭ��:%s V=%umV I=%umA NTC=%d\n",
+    uart_printf("״̬:%s -> %s :%s V=%umV I=%umA NTC=%d\n",
                 ch_state_name(ch_state),
                 ch_state_name(next_state),
                 (reason != 0) ? reason : "��",
@@ -204,13 +204,13 @@ static u16 ch_get_cccv_work_current_ma(u16 target_current_ma)
             /*
              * ÿ�ν��� 100mA��������Ƶ� iGED��
              */
-            if(s_cccv_curr_limit_ma > (u16)(iGED + 100))
+            if(s_cccv_curr_limit_ma > (u16)(iGED + 500))
             {
-                s_cccv_curr_limit_ma -= 100;
+                s_cccv_curr_limit_ma -= 500;
             }
             else
             {
-                s_cccv_curr_limit_ma = iGED;
+                s_cccv_curr_limit_ma = iGED+100;
             }
         }
     }
@@ -661,7 +661,7 @@ static bit ch_battery_removed_check_10ms(void)
                     return 1;
                 }
                 s_remove_cnt = 0U;
-                ch_set_state(BMS_ERR, "�γ��쳣");
+                ch_set_state(BMS_ERR, "�γ��쳣");
                 return 1;
             }
             return 1;
@@ -931,6 +931,7 @@ void usr_ch_func(void)
                     }
                     else if(u1w_info.handshake_ok != 0U)
                     {
+                        pc_uart_print_batt();
                         ch_set_state(CH_Check, "���ֳɹ�");
                     }
                     else
@@ -1175,7 +1176,7 @@ void usr_ch_func(void)
                 RGed_Flash(50);
                 if(u1w_info.comm_timeout != 0U)
                 {
-                    ch_set_state(BMS_ERR, "�¶ȵȴ�ͨ�ų�ʱ");
+                    ch_set_state(BMS_ERR, "�¶�ͨ�ų�ʱ");
                 }
                 else if((u1w_info.charge_status & CH_BMS_ERR_MASK) != 0U)
                 {
diff --git a/YAT-60V3A-Uart-1-wire/App/main.c b/YAT-60V3A-Uart-1-wire/App/main.c
index 491e2e0..ece1b0d 100644
--- a/YAT-60V3A-Uart-1-wire/App/main.c
+++ b/YAT-60V3A-Uart-1-wire/App/main.c
@@ -9,10 +9,447 @@
 #include "cal.h"
 #include "uart_1_wire.h"
 #include "common.h"
+#include "timer.h"
+#include "fifo.h"
+#include "uart.h"
 
 bit flg_cal_mode = 0;     /* 0: �������  1: DEBUG У׼���� */
 data u16 next_10ms;
 
+/*
+ * һ��ͨ���ֶ�����������ڡ�
+ * ��ǰ����Ӳ��ͨ��δ��ͨ�׶Σ�
+ * 1. ���ܳ�����̣�
+ * 2. ����ͨ�ų�ʱ��
+ * 3. ���Զ�����һ�����
+ * 4. ͨ�� DEBUG ���ڰ����ֶ�����ÿһ��һ�����
+ * 5. COM �յ��κ��ֽڶ�ԭʼ��ӡ�����㿴Ӳ�����κͻذ���
+ */
+#define MAIN_1WIRE_TEST_EN              (0U)
+#define MAIN_1WIRE_TEST_CMD_NUM         (9U)
+#define MAIN_1WIRE_INFO_PERIOD_10MS     (100U)
+
+#if (MAIN_1WIRE_TEST_EN != 0U)
+static u8 s_1w_test_idx;
+static u8 s_flow_run;
+static u8 s_flow_charge;
+static u8 s_flow_timeout_printed;
+static u16 s_flow_info_tick_10ms;
+
+static void test_put_char(u8 dat)
+{
+    ch_uart_send_byte(dat);
+}
+
+static void test_put_str(char *str)
+{
+    while(*str != 0)
+    {
+        test_put_char((u8)(*str));
+        str++;
+    }
+}
+
+static void test_put_crlf(void)
+{
+    test_put_char(0x0DU);
+    test_put_char(0x0AU);
+}
+
+static void test_put_hex4(u8 dat)
+{
+    dat &= 0x0FU;
+    if(dat < 10U)
+    {
+        test_put_char((u8)('0' + dat));
+    }
+    else
+    {
+        test_put_char((u8)('A' + dat - 10U));
+    }
+}
+
+static void test_put_hex8(u8 dat)
+{
+    test_put_hex4((u8)(dat >> 4));
+    test_put_hex4(dat);
+}
+
+static void test_put_bytes(u8 *buf, u8 len)
+{
+    u8 i;
+
+    for(i = 0U; i < len; i++)
+    {
+        test_put_hex8(buf[i]);
+        test_put_char(' ');
+    }
+}
+
+static u8 test_sum(u8 *buf, u8 len)
+{
+    u8 i;
+    u8 sum;
+
+    sum = 0U;
+    for(i = 0U; i < len; i++)
+    {
+        sum = (u8)(sum + buf[i]);
+    }
+
+    return sum;
+}
+
+static u8 test_get_cmd_by_idx(u8 idx)
+{
+    switch(idx)
+    {
+    case 0U: return U1W_CMD_A0;
+    case 1U: return U1W_CMD_A1;
+    case 2U: return U1W_CMD_A4;
+    case 3U: return U1W_CMD_A6;
+    case 4U: return U1W_CMD_A7;
+    case 5U: return U1W_CMD_B1;
+    case 6U: return U1W_CMD_B3;
+    case 7U: return U1W_CMD_B4;
+    default: return U1W_CMD_B6;
+    }
+}
+
+static void test_print_current_cmd(void)
+{
+    test_put_str("[1W] ��ǰ���=");
+    test_put_char((u8)('0' + s_1w_test_idx));
+    test_put_str(" ����=0x");
+    test_put_hex8(test_get_cmd_by_idx(s_1w_test_idx));
+    test_put_crlf();
+}
+
+static void test_print_help(void)
+{
+    test_put_str("[1W] �ֶ���������");
+    test_put_crlf();
+    test_put_str("[1W] ������s���� r�ط� n��һ�� p��һ�� 0~8ѡ�� g���� xֹͣ ?����");
+    test_put_crlf();
+    test_put_str("[1W] ���0=A0 1=A1 2=A4 3=A6 4=A7 5=B1 6=B3 7=B4 8=B6");
+    test_put_crlf();
+    test_put_str("[1W] �޳�ʱ�����Զ���һ��");
+    test_put_crlf();
+    test_print_current_cmd();
+}
+
+static void test_make_frame(u8 cmd, u8 *buf, u8 *len)
+{
+    buf[0] = cmd;
+
+    if(cmd == U1W_CMD_A0)
+    {
+        buf[1] = 0x00U;
+        buf[2] = U1W_MASTER;
+        buf[3] = test_sum(buf, 3U);
+        *len = 4U;
+    }
+    else if(cmd == U1W_CMD_B6)
+    {
+        buf[1] = 0x01U;
+        buf[2] = 0x01U;
+        buf[3] = test_sum(buf, 3U);
+        *len = 4U;
+    }
+    else
+    {
+        buf[1] = cmd;
+        *len = 2U;
+    }
+}
+
+static void test_send_current_cmd(void)
+{
+    u8 cmd;
+    u8 tx_len;
+    u8 tx_buf[4];
+
+    cmd = test_get_cmd_by_idx(s_1w_test_idx);
+    test_make_frame(cmd, tx_buf, &tx_len);
+
+    test_put_str("[1W] �������=");
+    test_put_char((u8)('0' + s_1w_test_idx));
+    test_put_str(" ����=0x");
+    test_put_hex8(cmd);
+    test_put_str(" �ֽ�=");
+    test_put_bytes(tx_buf, tx_len);
+    test_put_crlf();
+
+    if(com_uart_send_buf(tx_buf, tx_len) != BSP_OK)
+    {
+        test_put_str("[1W] ����ʧ��");
+        test_put_crlf();
+    }
+
+    com_uart_set_rx_mode();
+}
+
+static void test_next_cmd(void)
+{
+    s_1w_test_idx++;
+    if(s_1w_test_idx >= MAIN_1WIRE_TEST_CMD_NUM)
+    {
+        s_1w_test_idx = 0U;
+    }
+
+    test_send_current_cmd();
+}
+
+static void test_prev_cmd(void)
+{
+    if(s_1w_test_idx == 0U)
+    {
+        s_1w_test_idx = MAIN_1WIRE_TEST_CMD_NUM - 1U;
+    }
+    else
+    {
+        s_1w_test_idx--;
+    }
+
+    test_send_current_cmd();
+}
+
+
+
+static void test_put_u16(u16 dat)
+{
+    char buf[5];
+    u8 i;
+    u8 started;
+
+    buf[0] = (char)('0' + (dat / 10000U));
+    dat %= 10000U;
+    buf[1] = (char)('0' + (dat / 1000U));
+    dat %= 1000U;
+    buf[2] = (char)('0' + (dat / 100U));
+    dat %= 100U;
+    buf[3] = (char)('0' + (dat / 10U));
+    buf[4] = (char)('0' + (dat % 10U));
+
+    started = 0U;
+    for(i = 0U; i < 5U; i++)
+    {
+        if((buf[i] != '0') || (started != 0U) || (i == 4U))
+        {
+            started = 1U;
+            test_put_char((u8)buf[i]);
+        }
+    }
+}
+
+static void test_put_s8(s8 dat)
+{
+    if(dat < 0)
+    {
+        test_put_char('-');
+        dat = (s8)(0 - dat);
+    }
+    test_put_u16((u16)((u8)dat));
+}
+
+static void test_put_01_unit(u16 raw)
+{
+    test_put_u16((u16)(raw / 10U));
+    test_put_char('.');
+    test_put_char((u8)('0' + (raw % 10U)));
+}
+
+static void test_print_flow_info(void)
+{
+    test_put_str("[���̲���] ��=");
+    test_put_u16(uart_1_wire.cell_series);
+    test_put_str("��");
+    test_put_u16(uart_1_wire.cell_parallel);
+    test_put_str("�� ����=");
+    test_put_01_unit(uart_1_wire.cell_cap_01ah);
+    test_put_str("Ah Ŀ���ѹmV=");
+    test_put_u16(uart_1_wire.target_voltage_mv);
+    test_put_str(" Ŀ�����mA=");
+    test_put_u16(uart_1_wire.target_current_ma);
+    test_put_crlf();
+
+    test_put_str("[���̲���] ����mV=");
+    test_put_u16(uart_1_wire.cell_max_mv);
+    test_put_str(" ����=");
+    test_put_u16(uart_1_wire.soc_percent);
+    test_put_str(" �¶�=");
+    test_put_s8(uart_1_wire.batt_temp_degc);
+    test_put_str(" MOS=");
+    test_put_s8(uart_1_wire.mos_temp_degc);
+    test_put_str(" ״̬=0x");
+    test_put_hex8(uart_1_wire.charge_status);
+    test_put_crlf();
+}
+
+static void test_flow_start(void)
+{
+    s_flow_run = 1U;
+    s_flow_charge = 0U;
+    s_flow_timeout_printed = 0U;
+    s_flow_info_tick_10ms = 0U;
+
+    com_fifo_clear();
+    uart_1_wire_reset_link();
+    uart_1_wire_set_stage(U1W_STAGE_HANDSHAKE);
+
+    test_put_str("[����] Э������");
+    test_put_crlf();
+}
+
+static void test_flow_stop(void)
+{
+    s_flow_run = 0U;
+    s_flow_charge = 0U;
+    s_flow_timeout_printed = 0U;
+    s_flow_info_tick_10ms = 0U;
+
+    uart_1_wire_set_stage(U1W_STAGE_STOP);
+    com_fifo_clear();
+    com_uart_set_rx_mode();
+
+    test_put_str("[����] Э��ֹͣ���ص��ֶ�ģʽ");
+    test_put_crlf();
+}
+
+static void test_flow_task(void)
+{
+    if(s_flow_run == 0U)
+    {
+        return;
+    }
+
+    uart_1_wire_poll_10ms();
+
+    if((u1w_info.comm_timeout != 0U) && (s_flow_timeout_printed == 0U))
+    {
+        s_flow_timeout_printed = 1U;
+        test_put_str("[����] ͨ�ų�ʱ ����=0x");
+        test_put_hex8(u1w_info.key_timeout_cmd);
+        test_put_crlf();
+    }
+
+    if((s_flow_charge == 0U) && (u1w_info.handshake_ok != 0U))
+    {
+        s_flow_charge = 1U;
+        test_put_str("[����] �������");
+        test_put_crlf();
+        test_print_flow_info();
+        uart_1_wire_set_stage(U1W_STAGE_CHARGE);
+        test_put_str("[����] �е������ѯ");
+        test_put_crlf();
+    }
+
+    s_flow_info_tick_10ms++;
+    if(s_flow_info_tick_10ms >= MAIN_1WIRE_INFO_PERIOD_10MS)
+    {
+        s_flow_info_tick_10ms = 0U;
+        test_print_flow_info();
+    }
+}
+
+
+static void test_debug_key_task(void)
+{
+    u8 dat;
+
+    while(s_ch_fifo.cnt != 0U)
+    {
+        dat = ch_fifo_pop();
+
+        if((dat == 'g') || (dat == 'G'))
+        {
+            test_flow_start();
+        }
+        else if((dat == 'x') || (dat == 'X'))
+        {
+            test_flow_stop();
+        }
+        else if(dat == '?')
+        {
+            test_print_help();
+        }
+        else if(s_flow_run != 0U)
+        {
+            /* ���̲��������У�ֻ��Ӧ g/x/?�������������ԡ� */
+        }
+        else if((dat >= '0') && (dat <= '8'))
+        {
+            s_1w_test_idx = (u8)(dat - '0');
+            test_send_current_cmd();
+        }
+        else if((dat == 's') || (dat == 'S') || (dat == 'r') || (dat == 'R'))
+        {
+            test_send_current_cmd();
+        }
+        else if((dat == 'n') || (dat == 'N'))
+        {
+            test_next_cmd();
+        }
+        else if((dat == 'p') || (dat == 'P'))
+        {
+            test_prev_cmd();
+        }
+        else
+        {
+            /* ���Իس������к�������Ч������ */
+        }
+    }
+}
+
+static void test_rx_task(void)
+{
+    u8 dat;
+
+    if(s_com_fifo.cnt == 0U)
+    {
+        return;
+    }
+
+    test_put_str("[1W] ����=");
+    while(s_com_fifo.cnt != 0U)
+    {
+        dat = com_fifo_pop();
+        test_put_hex8(dat);
+        test_put_char(' ');
+    }
+    test_put_crlf();
+}
+
+static void usr_1wire_test_func(void)
+{
+    s_1w_test_idx = 0U;
+
+    ch_fifo_clear();
+    com_fifo_clear();
+    com_uart_set_rx_mode();
+    next_10ms = timer_deadline_ms(TASK_10MS);
+
+    test_print_help();
+
+    for(;;)
+    {
+        if(timer_period_elapsed(&next_10ms, TASK_10MS) != 0)
+        {
+            wdt_feed();
+            test_debug_key_task();
+            if(s_flow_run != 0U)
+            {
+                test_flow_task();
+            }
+            else
+            {
+                test_rx_task();
+            }
+        }
+    }
+}
+#endif
+
 void main(void)
 {
     Sys_Init();
@@ -23,6 +460,9 @@ void main(void)
 
     for(;;)
     {
+#if (MAIN_1WIRE_TEST_EN != 0U)
+        usr_1wire_test_func();
+#else
         if(flg_cal_mode == 0)
         {
             usr_ch_func();
@@ -31,5 +471,6 @@ void main(void)
         {
             usr_cal_func();
         }
+#endif
     }
 }
diff --git a/YAT-60V3A-Uart-1-wire/App/pc_uart.c b/YAT-60V3A-Uart-1-wire/App/pc_uart.c
index 6989a3c..a196306 100644
--- a/YAT-60V3A-Uart-1-wire/App/pc_uart.c
+++ b/YAT-60V3A-Uart-1-wire/App/pc_uart.c
@@ -16,6 +16,7 @@
 #include "adc.h"
 #include "ch.h"
 #include "timer.h"
+#include "uart_1_wire.h"
 #include "stm32_crc.h"
 
 #define PC_UART_DATA_LEN                 (36U)
@@ -106,18 +107,34 @@ static void pc_uart_print_param(void)
 /**
   * @brief  ��ӡ��ǰ������״̬������ DEBUG �˹�������
   */
-static void pc_uart_print_batt(void)
+void pc_uart_print_batt(void)
 {
-    uart_printf("[BATT] vout=%u mV, curr=%u mA, ntc=%d(0.1C)\r\n",
+    uart_printf("[���] �����ѹ=%u mV, �������=%u mA, NTC=%d(0.1C)\r\n",
                 val.vout,
                 val.curr,
                 (s16)val.i_ntc);
-    uart_printf("[BATT] state=%bu, flag=%bu, tick=%u\r\n",
+    uart_printf("[���] ���״̬=%bu, ���ϱ�־=%bu, ʱ��=%u\r\n",
                 (u8)ch_state,
                 pc_uart_pack_flag(),
                 timer_get_tick_ms());
+    uart_printf("[BMS] �׶�=%bu, ����=%bu, ��ʱ=%bu, ��ʱ����=%bu\r\n",
+                u1w_info.stage,
+                u1w_info.handshake_ok,
+                u1w_info.comm_timeout,
+                u1w_info.key_timeout_cmd);
+    uart_printf("[BMS] ��ذ�=%bu��%bu��, ����=%u(0.1Ah), Ŀ��=%u mV/%u mA\r\n",
+                uart_1_wire.cell_series,
+                uart_1_wire.cell_parallel,
+                uart_1_wire.cell_cap_01ah,
+                u1w_info.target_voltage_mv,
+                u1w_info.target_current_ma);
+    uart_printf("[BMS] �������=%u mV, ����=%bu, ����¶�=%dC, MOS�¶�=%dC, ״̬=%bu\r\n",
+                u1w_info.cell_max_mv,
+                u1w_info.soc_percent,
+                (s16)u1w_info.batt_temp_degc,
+                (s16)u1w_info.mos_temp_degc,
+                u1w_info.charge_status);
 }
-
 /**
   * @brief  д�� u16/u32 С�����ݣ����� 54.6V ԭ����֡�ֽ���
   */
diff --git a/YAT-60V3A-Uart-1-wire/App/pc_uart.h b/YAT-60V3A-Uart-1-wire/App/pc_uart.h
index c432327..cc83364 100644
--- a/YAT-60V3A-Uart-1-wire/App/pc_uart.h
+++ b/YAT-60V3A-Uart-1-wire/App/pc_uart.h
@@ -10,5 +10,6 @@
 #include "common.h"
 
 u8 pc_uart_func(void);
+void pc_uart_print_batt(void);
 
 #endif
diff --git a/YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c b/YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
index 41017a6..c2c4aeb 100644
--- a/YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
+++ b/YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
@@ -105,6 +105,168 @@ static void u1w_dbg_put_bytes(u8 *buf, u8 len)
     }
 }
 
+#if (U1W_DEBUG_PARSE_EN != 0U)
+static void u1w_dbg_put_u16(u16 dat)
+{
+    char buf[5];
+    u8 i;
+    u8 started;
+
+    buf[0] = (char)('0' + (dat / 10000U));
+    dat %= 10000U;
+    buf[1] = (char)('0' + (dat / 1000U));
+    dat %= 1000U;
+    buf[2] = (char)('0' + (dat / 100U));
+    dat %= 100U;
+    buf[3] = (char)('0' + (dat / 10U));
+    buf[4] = (char)('0' + (dat % 10U));
+
+    started = 0U;
+    for(i = 0U; i < 5U; i++)
+    {
+        if((buf[i] != '0') || (started != 0U) || (i == 4U))
+        {
+            started = 1U;
+            u1w_dbg_put_char(buf[i]);
+        }
+    }
+}
+
+static void u1w_dbg_put_s8(s8 dat)
+{
+    if(dat < 0)
+    {
+        u1w_dbg_put_char('-');
+        dat = (s8)(0 - dat);
+    }
+    u1w_dbg_put_u16((u16)((u8)dat));
+}
+
+static void u1w_dbg_put_01a(u16 raw)
+{
+    u1w_dbg_put_u16((u16)(raw / 10U));
+    u1w_dbg_put_char('.');
+    u1w_dbg_put_char((char)('0' + (raw % 10U)));
+    u1w_dbg_put_char('A');
+}
+
+static u16 u1w_dbg_get_u16(u8 byte0, u8 byte1)
+{
+    return (u16)((u16)byte0 | ((u16)byte1 << 8));
+}
+
+static void u1w_dbg_put_parse(u8 *buf, u8 len)
+{
+    u8 cmd;
+    u8 xy;
+    u16 raw1;
+    u16 raw2;
+
+    if(len < 4U)
+    {
+        return;
+    }
+
+    cmd = buf[0];
+    switch(cmd)
+    {
+    case U1W_CMD_A0:
+        xy = buf[2];
+        u1w_dbg_put_str(" ID=0x");
+        u1w_dbg_put_hex8(buf[1]);
+        u1w_dbg_put_char(' ');
+        u1w_dbg_put_u16((u16)(((xy >> 4) & 0x0FU) + 5U));
+        u1w_dbg_put_char('S');
+        u1w_dbg_put_u16((u16)((xy & 0x0FU) + 1U));
+        u1w_dbg_put_char('P');
+        break;
+
+    case U1W_CMD_A1:
+        u1w_dbg_put_str(" CAP=");
+        u1w_dbg_put_u16((u16)(buf[2] / 10U));
+        u1w_dbg_put_char('.');
+        u1w_dbg_put_char((char)('0' + (buf[2] % 10U)));
+        u1w_dbg_put_str("Ah");
+        break;
+
+    case U1W_CMD_A4:
+        if(len >= 6U)
+        {
+            raw1 = u1w_dbg_get_u16(buf[1], buf[2]);
+            raw2 = u1w_dbg_get_u16(buf[3], buf[4]);
+            u1w_dbg_put_str(" PRE=");
+            u1w_dbg_put_u16((u16)(raw1 * 10U));
+            u1w_dbg_put_str("mV FULL=");
+            u1w_dbg_put_u16((u16)(raw2 * 10U));
+            u1w_dbg_put_str("mV");
+        }
+        break;
+
+    case U1W_CMD_A6:
+        if(len >= 6U)
+        {
+            raw1 = u1w_dbg_get_u16(buf[1], buf[2]);
+            raw2 = u1w_dbg_get_u16(buf[3], buf[4]);
+            u1w_dbg_put_str(" I_CELL=");
+            u1w_dbg_put_01a(raw1);
+            u1w_dbg_put_str(" RAW2=");
+            u1w_dbg_put_u16(raw2);
+        }
+        break;
+
+    case U1W_CMD_A7:
+        if(len >= 6U)
+        {
+            u1w_dbg_put_str(" T=");
+            u1w_dbg_put_s8((s8)buf[1]);
+            u1w_dbg_put_str("..");
+            u1w_dbg_put_s8((s8)buf[2]);
+            u1w_dbg_put_str("C DER=");
+            u1w_dbg_put_01a(buf[4]);
+        }
+        break;
+
+    case U1W_CMD_B1:
+        if(len >= 6U)
+        {
+            raw1 = u1w_dbg_get_u16(buf[1], buf[2]);
+            raw2 = u1w_dbg_get_u16(buf[3], buf[4]);
+            u1w_dbg_put_str(" MIN=");
+            u1w_dbg_put_u16((u16)(raw1 * 10U));
+            u1w_dbg_put_str("mV MAX=");
+            u1w_dbg_put_u16((u16)(raw2 * 10U));
+            u1w_dbg_put_str("mV");
+        }
+        break;
+
+    case U1W_CMD_B3:
+        u1w_dbg_put_str(" BAT=");
+        u1w_dbg_put_s8((s8)buf[1]);
+        u1w_dbg_put_str("C MOS_T=");
+        u1w_dbg_put_s8((s8)buf[2]);
+        u1w_dbg_put_char('C');
+        break;
+
+    case U1W_CMD_B4:
+        u1w_dbg_put_str(" SOC=");
+        u1w_dbg_put_u16(buf[1]);
+        u1w_dbg_put_str("% ST=0x");
+        u1w_dbg_put_hex8(buf[2]);
+        break;
+
+    case U1W_CMD_B6:
+        u1w_dbg_put_str(" TYPE=0x");
+        u1w_dbg_put_hex8(buf[1]);
+        u1w_dbg_put_str(" VAL=0x");
+        u1w_dbg_put_hex8(buf[2]);
+        break;
+
+    default:
+        break;
+    }
+}
+#endif
+
 /* 只缓存主机发送帧，当前发送帧最长按 4 字节处理。 */
 static void u1w_dbg_copy_tx(u8 *src, u8 len)
 {
@@ -167,6 +329,9 @@ void u1w_dbg_rx_ok(u8 *buf, u8 rx_len)
 
     u1w_dbg_put_str(" RX=");
     u1w_dbg_put_bytes(buf, rx_len);
+#if (U1W_DEBUG_PARSE_EN != 0U)
+    u1w_dbg_put_parse(buf, rx_len);
+#endif
     u1w_dbg_put_str(" OK\r\n");
 }
 
diff --git a/YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h b/YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
index 9ebbeb7..0d54815 100644
--- a/YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
+++ b/YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
@@ -16,7 +16,7 @@
 
 /* 总开关：0=关闭全部一线调试日志，1=打开。 */
 #ifndef U1W_DEBUG_EN
-#define U1W_DEBUG_EN                        (1U)
+#define U1W_DEBUG_EN                        (0U)
 #endif
 
 /* 解析日志开关：当前成功帧已合并打印，本宏保留给调试配置使用。 */
diff --git a/YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt b/YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt
index b586027..9777749 100644
--- a/YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt
+++ b/YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt
@@ -1,44 +1,66 @@
-[06-24 17:43:39]Log start
-[06-24 17:43:39]Serial init start!
-[06-24 17:43:39]Download driver ok!
-[06-24 17:43:39]uart_txm8c1200_set_driver:0x0
-[06-24 17:43:39]user uart set driver ok!
-[06-24 17:43:39]TXLink ready ok!
-[06-24 17:43:41]TXLink init failed!
-[06-24 17:43:52]DllUv3Cap:0x2
-[06-24 17:43:52]DllUv3Cap:0x6d
-[06-24 17:43:52]DllUv3Cap:0x1
-[06-24 17:43:52]Serial init start!
-[06-24 17:43:53]uart_txm8c1200_set_driver:0x0
-[06-24 17:43:53]user uart set driver ok!
-[06-24 17:43:53]TXLink ready ok!
-[06-24 17:43:54]TXLink init failed!
-[06-24 17:43:54]uart_txm8c1200_set_driver:0x0
-[06-24 17:43:54]user uart set driver ok!
-[06-24 17:43:54]TXLink ready ok!
-[06-24 17:43:55]TXLink init failed!
-[06-24 17:43:55]uart_txm8c1200_set_driver:0x0
-[06-24 17:43:55]user uart set driver ok!
-[06-24 17:43:55]TXLink ready ok!
-[06-24 17:43:56]TXLink init failed!
-[06-24 17:43:57]uart_txm8c1200_set_driver:0x0
-[06-24 17:43:57]user uart set driver ok!
-[06-24 17:43:57]TXLink ready ok!
-[06-24 17:43:58]TXLink init failed!
-[06-24 17:43:58]uart_txm8c1200_set_driver:0x0
-[06-24 17:43:58]user uart set driver ok!
-[06-24 17:43:58]TXLink ready ok!
-[06-24 17:43:59]TXLink init failed!
-[06-24 17:43:59]uart_txm8c1200_set_driver:0x0
-[06-24 17:43:59]user uart set driver ok!
-[06-24 17:43:59]TXLink ready ok!
-[06-24 17:44:00]TXLink init failed!
-[06-24 17:44:00]Serial close ok!
-[06-24 17:55:37]DllUv3Cap:0x2
-[06-24 17:55:37]DllUv3Cap:0x64
-[06-27 15:05:40]DllUv3Cap:0x2
-[06-27 15:05:40]DllUv3Cap:0x64
-[06-27 15:56:23]DllUv3Cap:0x2
-[06-27 15:56:23]DllUv3Cap:0x64
-[06-27 17:11:50]DllUv3Cap:0x2
-[06-27 17:11:50]DllUv3Cap:0x64
+[07-03 15:14:52]Log start
+[07-03 15:14:52]Serial init start!
+[07-03 15:14:52]Download driver ok!
+[07-03 15:14:52]uart_txm8c1200_set_driver:0x0
+[07-03 15:14:52]user uart set driver ok!
+[07-03 15:14:52]TXLink ready ok!
+[07-03 15:14:53]TXLink init ok!
+[07-03 15:14:53]Serial close ok!
+[07-03 15:14:53]load_debug_init_api chip id:1260
+[07-03 15:14:53]sys_info_init! 
+[07-03 15:14:53]AG_Init: 0x30a
+[07-03 15:14:53]AG_Init: 0x30b
+[07-03 15:14:53]AG_Init: 0x30f
+[07-03 15:14:53]AG_Init: 0x310
+[07-03 15:14:53]AG_Init: 0x311
+[07-03 15:14:53]AG_Init: 0x312
+[07-03 15:14:53]AG_Init: 0x100
+[07-03 15:14:53]InitMem: 0x0
+[07-03 15:14:53]Serial init start!
+[07-03 15:14:53]chip_init_api
+[07-03 15:14:53]TXLink chip init ok!
+[07-03 15:14:53]AG_Init: 0x201
+[07-03 15:14:53]AG_Init: 0x202
+[07-03 15:14:53]AG_Init: 0x203
+[07-03 15:14:53]AG_Init: 0x204
+[07-03 15:14:53]AG_Init: 0x205
+[07-03 15:14:53]AG_Init: 0x206
+[07-03 15:14:53]AG_Init: 0x207
+[07-03 15:14:53]AG_Init: 0x313
+[07-03 15:14:53]Serial init start!
+[07-03 15:14:53]uart_txm8c1200_check_dev_id:
+[07-03 15:14:53]uart_txm8c1200_check_crc_bin check failed 2:
+[07-03 15:14:53]uart_txm8c1200_check_dev_id ok:
+[07-03 15:14:53]EraseFlash: 0x0
+[07-03 15:14:53]TXLink chip erase ok!
+[07-03 15:14:53]AG_Init: 0x314
+[07-03 15:14:53]FlashLoad: 0x0
+[07-03 15:14:53]InitProgress: Loading...
+[07-03 15:14:53]WriteToFlash: 0xff000000
+[07-03 15:14:53]WriteToFlash: 0xff000800
+[07-03 15:14:54]WriteToFlash: 0xff001000
+[07-03 15:14:55]WriteToFlash: 0xff001800
+[07-03 15:14:55]WriteToFlash: 0xff002000
+[07-03 15:14:56]WriteToFlash: 0xff002800
+[07-03 15:14:56]WriteToFlash: 0xff003000
+[07-03 15:14:57]WriteToFlash: 0xff003800
+[07-03 15:14:57]InitProgress: Verifying...
+[07-03 15:14:57]VerifyFlash nAdr: 0xff000000
+[07-03 15:14:57]VerifyFlash nAdr: 0xff000800
+[07-03 15:14:57]VerifyFlash nAdr: 0xff001000
+[07-03 15:14:57]VerifyFlash nAdr: 0xff001800
+[07-03 15:14:57]VerifyFlash nAdr: 0xff002000
+[07-03 15:14:57]VerifyFlash nAdr: 0xff002800
+[07-03 15:14:57]VerifyFlash nAdr: 0xff003000
+[07-03 15:14:58]VerifyFlash nAdr: 0xff003800
+[07-03 15:14:58]chip_reset_api
+[07-03 15:14:58]Serial close ok!
+[07-03 15:14:58]AG_Init: 0x40c
+[07-03 15:14:58]TXLink ready failed!
+[07-03 15:14:58]TXLink ready failed!
+[07-03 15:14:58]TXLink ready failed!
+[07-03 15:14:58]CloseAllDlg: 0x0
+[07-03 15:14:58]StopTarget: 0x0
+[07-03 15:14:58]FreeCache: 0x0
+[07-03 15:14:58]StopTarget: 0x0
+[07-03 15:14:58]FreeCache: 0x0
diff --git a/YAT-60V3A-Uart-1-wire/Project/Keil/config.ini b/YAT-60V3A-Uart-1-wire/Project/Keil/config.ini
index 9b3ceab..edc02c8 100644
--- a/YAT-60V3A-Uart-1-wire/Project/Keil/config.ini
+++ b/YAT-60V3A-Uart-1-wire/Project/Keil/config.ini
@@ -15,7 +15,7 @@ hw=V1.10
 dll=V1.38
 fw=L:V1.31  D:V1.15
 max=2 MHz
-chip_id= 
+chip_id=TX8C1260
 [LOG_SETTINGS_ITEM]
 class=1:Basic
 clear=1
@@ -24,7 +24,7 @@ save_chip_bin_en=NO
 chip_sel=TX8C126x
 auto_en=1
 [UART_COMM_SETTINGS_ITEM]
-port=COM5
+port=COM11
 next_baud=1000000
 cur_baud=1000000
 usb_en=0
diff --git "a/YAT-60V3A-Uart-1-wire/\346\234\254\350\275\256\345\274\202\346\255\245\345\215\217\350\256\256\344\270\216\347\212\266\346\200\201\346\234\272\350\247\204\345\210\231\350\257\264\346\230\216.md" "b/YAT-60V3A-Uart-1-wire/\346\234\254\350\275\256\345\274\202\346\255\245\345\215\217\350\256\256\344\270\216\347\212\266\346\200\201\346\234\272\350\247\204\345\210\231\350\257\264\346\230\216.md"
deleted file mode 100644
index 79c50ef..0000000
--- "a/YAT-60V3A-Uart-1-wire/\346\234\254\350\275\256\345\274\202\346\255\245\345\215\217\350\256\256\344\270\216\347\212\266\346\200\201\346\234\272\350\247\204\345\210\231\350\257\264\346\230\216.md"
+++ /dev/null
@@ -1,45 +0,0 @@
-﻿# 本轮异步协议与充电状态机规则说明
-
-## 1. 协议收发
-
-- 收发异步，不再使用“发一帧、等这一帧回复、再发下一帧”的同步等待模式。
-- 接收侧从缓存中 for 扫描 CMD：CMD 已知、长度够、校验和正确，才认为是合法帧。
-- 校验失败时不清空整包，从下一字节继续找可能的 CMD。
-- 发送侧按阶段每 100ms 发一帧，不等待对应回复。
-
-## 2. 发送阶段
-
-- 握手阶段：A0 / A1 / A4 / A6 / A7 / B1 / B3 / B4，全部收齐后才允许进入充电流程。
-- 充电阶段：B1 / B3 / B4 / B6，B6 固定发送“要求打开充电 MOS”。
-- 满电阶段：发送 B6 03 SOC，持续 3 分钟，之后通信层主动拉低 COM。
-- BMS 温度异常阶段：不拉低 COM，继续轮询 B3 / B4，等待温度恢复。
-- 普通异常阶段：主动拉低 COM，告知 BMS 主机断开。
-
-## 3. 超时
-
-- 2 秒没有收到任何合法帧，判定通信超时。
-- 3 秒关键帧没有刷新，判定通信超时。
-- 握手关键帧：A0 / A1 / A4 / A6 / A7 / B1 / B3 / B4。
-- 充电关键帧：B1 / B3 / B4。
-- 温度等待关键帧：B3 / B4。
-- 主机侧不再把 COM 被拉低 30ms 当作常规异常；30ms 拉低是主机主动结束通信给 BMS 识别的动作。
-
-## 4. 满电与异常的电压检测
-
-- 充电相关状态下，电池电压检测分压常开。
-- 满电、普通异常、温度异常锁止时，电池电压检测分压默认关闭，避免持续对电池放电。
-- 这些状态下采用间歇检测：每 1 秒打开 100ms，打开 50ms 后才认为 ADC 电压有效。
-
-## 5. 电池拔除判断
-
-- CH_IDLE 只判断插入，不判断拔出。
-- 除 CH_IDLE 外，所有状态都要判断电池拔出。
-- 继电器未闭合或输出关闭时，用有效电压窗口判断 val.vout < vRESET。
-- 继电器闭合的充电阶段，不能用电压判断拔出；改用“小电流 + 1 秒无合法通信帧”。
-- 拔出确认后统一处理：关输出、释放通信、清上一块电池数据、回 CH_IDLE。
-
-## 6. 状态机边界
-
-- ch.c 只负责充电流程、状态切换、拔电池判断和硬件输出。
-- uart_1_wire.c 只负责协议收发、帧扫描、校验、解析、通信超时和 COM 主动拉低动作。
-- BMS 普通异常不主动替 BMS 关 MOS；温度异常例外，充电器关闭输出并继续通信等待恢复。
```

## Existing AI Bridge Context

--- .ai-bridge/current-plan.md ---
  1 | # 只补本轮干净 implementation-diff.patch
  2 | 
  3 | Updated: 2026-06-28T04:17:43.204Z
  4 | Workspace: D:\Users\CODE
  5 | Target agent: Codex (codex)
  6 | 
  7 | ## Plan
  8 | 
  9 | # 本轮任务：只补干净的 implementation-diff.patch，不再改源码
 10 | 
 11 | ## 0. 当前状态
 12 | 
 13 | ChatGPT Pro 已经检查过本轮源码实现：
 14 | 
 15 | 1. `YAT-60V3A-Uart-1-wire/App/uart_1_wire.c` 当前逻辑基本正确；
 16 | 2. `u1w_key_index()` 已删除；
 17 | 3. `u1w_parse_frame()` 中 A0/A1/A4/A6/A7/B1/B3/B4 已在各自 case 内直接设置 `key`；
 18 | 4. `key_age_10ms` 在 switch 之后统一刷新；
 19 | 5. `handshake_mask` 只在 `s_u1w.stage == U1W_STAGE_HANDSHAKE` 时更新；
 20 | 6. `u1w_rx_task()` 的校验流程没有被修改，仍然只有校验正确才调用 `u1w_parse_frame()`；
 21 | 7. Keil 编译结果已确认：0 Error(s), 0 Warning(s)，Program Size 为 `data=163.4 xdata=315 const=146 code=15752`；
 22 | 8. `code + const + 128 = 15752 + 146 + 128 = 16026`，小于 16384，剩余 358 字节。
 23 | 
 24 | 所以当前不要再改源码。
 25 | 
 26 | ## 1. 本轮只允许做什么
 27 | 
 28 | 本轮只允许补交付文件：
 29 | 
 30 | ```text
 31 | .ai-bridge/implementation-diff.patch
 32 | .ai-bridge/codex-status.md
 33 | ```
 34 | 
 35 | 其中重点是重新生成 `.ai-bridge/implementation-diff.patch`。
 36 | 
 37 | ## 2. 本轮禁止做什么
 38 | 
 39 | 严禁修改以下源码或配置：
 40 | 
 41 | ```text
 42 | YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
 43 | YAT-60V3A-Uart-1-wire/App/ch.c
 44 | YAT-60V3A-Uart-1-wire/App/pc_uart.c
 45 | YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
 46 | YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
 47 | YAT-60V3A-Uart-1-wire/App/usr_cfg.h
 48 | YAT-60V3A-Uart-1-wire/memory.md
 49 | YAT-60V3A-Uart-1-wire/memory.md.bak_encoding_20260628_105912
 50 | ```
 51 | 
 52 | 严禁再做：
 53 | 
 54 | 1. 重新改 `u1w_parse_frame()`；
 55 | 2. 重新改握手逻辑；
 56 | 3. 删除或修改 `u1w_dbg_parse_ok()`；
 57 | 4. 改 `ch.c`；
 58 | 5. 改 `pc_uart.c`；
 59 | 6. 改默认调试开关；
 60 | 7. 改编码；
 61 | 8. 运行会自动改源码的格式化工具；
 62 | 9. 混入旧任务的 `ch.c` 大 diff。
 63 | 
 64 | ## 3. 为什么要补 patch
 65 | 
 66 | ChatGPT Pro 发现当前 `.ai-bridge/implementation-diff.patch` 不是本轮干净 diff，里面混入了旧任务的 `ch.c` 大 diff。
 67 | 
 68 | 这会影响后续审查和交接，所以要重新生成一个只对应本轮的干净 patch。
 69 | 
 70 | ## 4. implementation-diff.patch 应包含什么
 71 | 
 72 | 新的 `.ai-bridge/implementation-diff.patch` 只应包含本轮相关内容：
 73 | 
 74 | 必须包含：
 75 | 
 76 | ```text
 77 | YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
 78 | ```
 79 | 
 80 | 可以包含，如果你确实更新了状态：
 81 | 
 82 | ```text
 83 | .ai-bridge/codex-status.md
 84 | ```
 85 | 
 86 | 可以包含，如果 current-plan 本身被更新后需要记录：
 87 | 
 88 | ```text
 89 | .ai-bridge/current-plan.md
 90 | ```
 91 | 
 92 | 但不要把历史旧任务混进来。
 93 | 
 94 | 尤其不要包含：
 95 | 
 96 | ```text
 97 | YAT-60V3A-Uart-1-wire/App/ch.c
 98 | YAT-60V3A-Uart-1-wire/App/pc_uart.c
 99 | YAT-60V3A-Uart-1-wire/App/usr_cfg.h
100 | YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
101 | YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
102 | YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt
103 | YAT-60V3A-Uart-1-wire/Project/Keil/*.log
104 | YAT-60V3A-Uart-1-wire/memory.md
105 | ```
106 | 
107 | ## 5. 推荐生成方式
108 | 
109 | 不要直接用全仓库 `git diff` 覆盖 patch，因为工作区里有很多历史未提交内容。
110 | 
111 | 推荐只针对本轮文件生成：
112 | 
113 | ```text
114 | git diff -- YAT-60V3A-Uart-1-wire/App/uart_1_wire.c .ai-bridge/codex-status.md > .ai-bridge/implementation-diff.patch
115 | ```
116 | 
117 | 如果你的工具不允许重定向，就用等价方式写入，但内容必须只来自这两个路径的 diff。
118 | 
119 | 生成后必须检查 patch 内容，确认没有出现：
120 | 
121 | ```text
122 | App/ch.c
123 | App/pc_uart.c
124 | App/usr_cfg.h
125 | uart_1_wire_debug.c
126 | uart_1_wire_debug.h
127 | Project/Keil
128 | memory.md
129 | ```
130 | 
131 | 如果出现这些路径，说明 patch 不干净，必须重新生成。
132 | 
133 | ## 6. 对本轮源码的复核要求，只读，不写
134 | 
135 | 生成 patch 后，只读确认下面几项：
136 | 
137 | 1. 搜索 `u1w_key_index`，应无结果；
138 | 2. 读取 `u1w_parse_frame()` 片段，确认 `key = U1W_KEY_MAX;` 在 switch 前；
139 | 3. 确认 A0/A1/A4/A6/A7/B1/B3/B4 的 case 内各自设置 `key`；
140 | 4. 确认 B6 不设置 `key`；
141 | 5. 确认 switch 后统一执行：
142 | 
143 | ```c
144 | if(key < U1W_KEY_MAX)
145 | {
146 |     s_u1w.key_age_10ms[key] = 0U;
147 |     if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
148 |     {
149 |         uart_1_wire.handshake_mask |= (u8)(1U << key);
150 |     }
151 | }
152 | ```
153 | 
154 | 这里只读检查，不允许改源码。
155 | 
156 | ## 7. 本轮报告格式
157 | 
158 | 完成后只报告下面内容，不要贴大量命令流水账：
159 | 
160 | ```text
161 | 1. 是否只更新了 .ai-bridge/implementation-diff.patch 和必要状态文件；
162 | 2. patch 中包含哪些文件；
163 | 3. patch 中是否排除了 ch.c、pc_uart.c、memory.md、Keil 日志等无关内容；
164 | 4. 是否确认 u1w_key_index 已无残留；
165 | 5. 是否确认 uart_1_wire.c 逻辑仍保持本轮结果；
166 | 6. 是否没有再改源码；
167 | 7. 如更新 codex-status.md，说明新增了哪一小段。
168 | ```
169 | 
170 | ## 8. 额外通用规则
171 | 
172 | ChatGPT Pro 已经在 `AGENTS.md` 追加第 26 节“小范围修改的高效执行规则”。
173 | 
174 | 本轮你应先读取 `AGENTS.md` 第 26 节，再执行本计划。
175 | 
176 | 重点记住：
177 | 
178 | 1. 小范围修改必须先限定函数范围，不能全文件替换；
179 | 2. CP936/GBK 文件不能用默认 UTF-8 方式硬写；
180 | 3. 计划外 BUG 只记录，不顺手改；
181 | 4. `implementation-diff.patch` 必须对应本轮任务，不能混入旧任务 diff。
182 | 
183 | ## Implementation contract
184 | 
185 | - Work from this plan in small, reviewable steps.
186 | - Keep edits scoped to the requested task and existing project conventions.
187 | - Run focused verification before handing work back.
188 | - Update .ai-bridge/agent-status.md with files touched, checks run, results, blockers, and review notes.
189 | - Save the final review diff to .ai-bridge/implementation-diff.patch when practical.
190 | - Append notable execution events to .ai-bridge/execution-log.jsonl when the implementation agent supports logging.
191 | 

--- .ai-bridge/agent-status.md ---
1 | # Agent Status
2 | 
3 | No implementation agent status written yet.
4 | 

--- .ai-bridge/implementation-diff.patch ---
  1 | diff --git a/YAT-60V3A-Uart-1-wire/App/uart_1_wire.c b/YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
  2 | index 058d0d9..acca033 100644
  3 | --- a/YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
  4 | +++ b/YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
  5 | @@ -202,22 +202,6 @@ static u8 u1w_reply_len(u8 cmd)
  6 |      }
  7 |  }
  8 |  
  9 | -static u8 u1w_key_index(u8 cmd)
 10 | -{
 11 | -    switch(cmd)
 12 | -    {
 13 | -    case U1W_CMD_A0: return U1W_KEY_A0;
 14 | -    case U1W_CMD_A1: return U1W_KEY_A1;
 15 | -    case U1W_CMD_A4: return U1W_KEY_A4;
 16 | -    case U1W_CMD_A6: return U1W_KEY_A6;
 17 | -    case U1W_CMD_A7: return U1W_KEY_A7;
 18 | -    case U1W_CMD_B1: return U1W_KEY_B1;
 19 | -    case U1W_CMD_B3: return U1W_KEY_B3;
 20 | -    case U1W_CMD_B4: return U1W_KEY_B4;
 21 | -    default:         return U1W_KEY_MAX;
 22 | -    }
 23 | -}
 24 | -
 25 |  static void u1w_refresh_target_current(void)
 26 |  {
 27 |      u16 target_ma;
 28 | @@ -254,16 +238,12 @@ static void u1w_parse_frame(u8 *frame)
 29 |      uart_1_wire.offline_count_10ms = 0U;
 30 |      s_u1w.any_rx_age_10ms = 0U;
 31 |  
 32 | -    key = u1w_key_index(cmd);
 33 | -    if(key < U1W_KEY_MAX)
 34 | -    {
 35 | -        s_u1w.key_age_10ms[key] = 0U;
 36 | -        uart_1_wire.handshake_mask |= (u8)(1U << key);
 37 | -    }
 38 | +    key = U1W_KEY_MAX;
 39 |  
 40 |      switch(cmd)
 41 |      {
 42 |      case U1W_CMD_A0:
 43 | +        key = U1W_KEY_A0;
 44 |          /* A0 ZZ XY CHK��ZZ=ʶ���룬X=�������룬Y=�������롣 */
 45 |          uart_1_wire.pack_id = frame[1];
 46 |          xy = frame[2];
 47 | @@ -281,11 +261,13 @@ static void u1w_parse_frame(u8 *frame)
 48 |          break;
 49 |  
 50 |      case U1W_CMD_A1:
 51 | +        key = U1W_KEY_A1;
 52 |          /* A1 XX YY CHK��YY=���� 0.1Ah�� */
 53 |          uart_1_wire.cell_cap_01ah = frame[2];
 54 |          break;
 55 |  
 56 |      case U1W_CMD_A4:
 57 | +        key = U1W_KEY_A4;
 58 |          /* A4 xxxx yyyy CHK��xxxx=Ԥ���ֹ��yyyy=���䣬��λ0.01V�� */
 59 |          uart_1_wire.cell_pre_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[1], frame[2]));
 60 |          pack_mv = (u32)u1w_001v_to_mv(u1w_get_u16_bus(frame[3], frame[4])) *
 61 | @@ -298,6 +280,7 @@ static void u1w_parse_frame(u8 *frame)
 62 |          break;
 63 |  
 64 |      case U1W_CMD_A6:
 65 | +        key = U1W_KEY_A6;
 66 |          /* A6 xxxx yyyy CHK��xxxx=����о��������0.1A�� */
 67 |          uart_1_wire.max_charge_current_ma =
 68 |              U1W_LIMIT_CURRENT(u1w_cell_01a_to_pack_ma(u1w_get_u16_bus(frame[1], frame[2])));
 69 | @@ -305,6 +288,7 @@ static void u1w_parse_frame(u8 *frame)
 70 |          break;
 71 |  
 72 |      case U1W_CMD_A7:
 73 | +        key = U1W_KEY_A7;
 74 |          /* A7 XX YY 00 ZZ CHK��XX/YY=����������ZZ=����о��������0.1A�� */
 75 |          uart_1_wire.derate_low_degc = (s8)frame[1];
 76 |          uart_1_wire.derate_high_degc = (s8)frame[2];
 77 | @@ -313,12 +297,14 @@ static void u1w_parse_frame(u8 *frame)
 78 |          break;
 79 |  
 80 |      case U1W_CMD_B1:
 81 | +        key = U1W_KEY_B1;
 82 |          /* B1 xxxx yyyy CHK��xxxx=��͵��ڣ�yyyy=��ߵ��ڣ���λ0.01V�� */
 83 |          uart_1_wire.cell_min_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[1], frame[2]));
 84 |          uart_1_wire.cell_max_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[3], frame[4]));
 85 |          break;
 86 |  
 87 |      case U1W_CMD_B3:
 88 | +        key = U1W_KEY_B3;
 89 |          /* B3 XX YY CHK��XX=����¶ȣ�YY=��� MOS �¶ȡ� */
 90 |          uart_1_wire.batt_temp_degc = (s8)frame[1];
 91 |          uart_1_wire.mos_temp_degc = (s8)frame[2];
 92 | @@ -326,6 +312,7 @@ static void u1w_parse_frame(u8 *frame)
 93 |          break;
 94 |  
 95 |      case U1W_CMD_B4:
 96 | +        key = U1W_KEY_B4;
 97 |          /* B4 XX YY CHK��XX=SOC��YY=״̬λ��״̬λֻ��¼����ֱ�ӿ���B6�� */
 98 |          uart_1_wire.soc_percent = frame[1];
 99 |          if(uart_1_wire.soc_percent > 100U)
100 | @@ -343,6 +330,15 @@ static void u1w_parse_frame(u8 *frame)
101 |          break;
102 |      }
103 |  
104 | +    if(key < U1W_KEY_MAX)
105 | +    {
106 | +        s_u1w.key_age_10ms[key] = 0U;
107 | +        if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
108 | +        {
109 | +            uart_1_wire.handshake_mask |= (u8)(1U << key);
110 | +        }
111 | +    }
112 | +
113 |      if((uart_1_wire.handshake_mask & U1W_HANDSHAKE_MASK) == U1W_HANDSHAKE_MASK)
114 |      {
115 |          uart_1_wire.link_state = U1W_LINK_ONLINE;
116 | 

--- .ai-bridge/codex-status.md ---
  1 | # Codex Status
  2 | 
  3 | 更新时间: 2026-06-27
  4 | 
  5 | ## 当前状态
  6 | - 已先提交当前工作区，提交: 2574a0a checkpoint before plan execution
  7 | - 当前分支: master
  8 | - 工作区: 提交后干净
  9 | - 已读取: AGENTS.md, USER.md, .ai-bridge/current-plan.md
 10 | - 已读取参考文档: 一线通信代码实现思路.md, 一线通信异步轮询协议重构总结.md
 11 | 
 12 | ## CodeGraph
 13 | - 检测到 .codegraph
 14 | - codegraph status 失败: PowerShell 执行策略阻止脚本；cmd /c codegraph status 报 unable to open database file
 15 | - 处理方式: 暂按计划使用 rg 和源码人工复核，修改前输出影响范围判断
 16 | 
 17 | ## 当前计划步骤
 18 | 1. 修正充电中拔电池判定: 1s 无通信且电流小于 iGED 后，先关输出，等待约 20ms，再用 vout < vRESET 确认拔电池，否则进入 BMS_ERR。
 19 | 2. 检查并补齐 CH_IDLE 低压电池插入稳定确认，避免残压下降或插入瞬态误判。
 20 | 3. 复核 B4 状态分类、FULL_DISPLAY、复充、LED 1Hz 是否符合 current-plan。
 21 | 4. 每步修改后运行 Keil 编译/链接验证，并记录 code size。
 22 | 
 23 | ## 风险与约束
 24 | - 严格遵守 ENCODING_GUARD + GIT_GUARD。
 25 | - 不改串口收发流程为同步等待。
 26 | - 不默认打开协议日志。
 27 | - 不引入大宏、大日志、大字符串，优先小代码。
 28 | ## Step 1 结果: 充电中拔电池判定
 29 | - 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
 30 | - 修改点: `val.curr < iGED` 且 `no_rx_10ms >= 100` 后先关输出，约 20ms 后检查 `val.vout < vRESET`。
 31 | - 正常拔出: 复位一线链路并回到 CH_IDLE。
 32 | - 非正常拔出: 进入 BMS_ERR。
 33 | - 编码: ch.c 保持 CP936, 无 BOM, LF。
 34 | - 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
 35 | - Program Size: data=155.4 xdata=315 const=146 code=14713。
 36 | - 备注: Git 提示该文件受 autocrlf 影响，下次 Git 触碰可能转换 CRLF；本次写回仍保持 LF。
 37 | ## Step 2 结果: CH_IDLE 低压插入稳定确认
 38 | - 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
 39 | - 修改点: 待机时 `vRESET < val.vout < vSTART` 不立即报错，需连续约 2s 且相邻 10ms 变化不超过 100mV。
 40 | - 正常插入: `val.vout >= vSTART` 仍按原 50 次确认后进 BMS_HANDSHAKE。
 41 | - 残压放空: `val.vout <= vRESET` 清低压候选，保持 CH_IDLE。
 42 | - 低压确认: 进入 CH_UVP，输出保持关闭。
 43 | - 编码: ch.c 保持 CP936, 无 BOM, LF。
 44 | - 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
 45 | - Program Size: data=157.4 xdata=315 const=146 code=14893。
 46 | ## Step 3 结果: B4 分类、满电显示、LED、协议禁项复核
 47 | - 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
 48 | - B4 bit0: 进入 CH_FULL，等效满电处理。
 49 | - B4 bit1/2/3: 进入 BMS_TEMP_ERR，输出关闭，继续轮询 B3/B4，不主动拉低 COM。
 50 | - B4 bit4/5/6/7: 进入 BMS_ERR，输出关闭，通信阶段为 PULL_LOW。
 51 | - BMS_TEMP_ERR 中若后续出现 bit4~7，也会转 BMS_ERR。
 52 | - FULL_DISPLAY: 保持协议层发送 B6 03 SOC，3 分钟后拉低 COM；未改 uart_1_wire.c 收发流程。
 53 | - 复充: FULL_DISPLAY 阶段不轮询 B1，当前仍用整包 vCH60 近似判断，并已在代码注释说明。
 54 | - LED: Red_Flash/Ged_Flash/RGed_Flash(50) 在 10ms 调度下 500ms 翻转一次，完整周期约 1Hz；低压/普通异常为红灯约 1Hz。
 55 | - 协议日志: U1W_DEBUG_EN=0, U1W_DEBUG_PARSE_EN=0，默认关闭。
 56 | - 禁项复核: 未新增 A2/A3/A5/B0/B2/B5/B7/B8；未恢复 wait_cmd/wait_len/wait_start_ms/retry_count/busy/can_send 同步等待模型。
 57 | - 编码: ch.c 保持 CP936, 无 BOM, LF。
 58 | - 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
 59 | - Program Size: data=157.4 xdata=315 const=146 code=14940。
 60 | ## Final Check
 61 | - 最终 Keil Build: 0 Error(s), 0 Warning(s)。
 62 | - 最终 Program Size: data=157.4 xdata=315 const=146 code=14940。
 63 | - 16K code 上限 0x3F80/16256 字节，当前剩余约 1316 字节。
 64 | - ch.c 编码: CP936/GBK, 无 BOM, LF, 未发现 emoji。
 65 | - codex-status.md 编码: UTF-8 无 BOM, LF。
 66 | - implementation-diff.patch 已生成。
 67 | - Keil 构建更新了 Project/Keil/TXLinkLog.txt，未擅自回退。
 68 | ## Remove Return Fix
 69 | - 计划来源: 重新读取 .ai-bridge/current-plan.md，目标为修复充电中拔除等待期间重新开输出问题。
 70 | - 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
 71 | - CodeGraph: 已 sync，索引最新；目标函数 ch_battery_removed_check_10ms 只有 usr_ch_func 一个上游调用者。
 72 | - 修复点: 疑似拔除启动后立即 return 1，让主循环 continue，避免本轮继续执行 CH_REPAIR/CH_Pre1/CH_CCCV 重新开输出。
 73 | - 修复点: 20ms 等待期间每轮 ch_output_all_off() 后 return 1，后续状态逻辑不会运行。
 74 | - 修复点: 20ms 后 vout >= vRESET 进入 BMS_ERR 后 return 1，不再继续执行后续充电逻辑。
 75 | - 检查: 未发现 iGED / 2 作为充电中拔除阈值。
 76 | - 编码: ch.c 保持 CP936/GBK, 无 BOM, LF, 未发现 emoji。
 77 | - 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
 78 | - Program Size: data=157.4 xdata=315 const=146 code=14957，距 16K 上限约 1299 字节。
 79 | ## Current Plan Run: Remove Wait + C51 Chinese FD Check
 80 | - 计划来源: 已重新读取 .ai-bridge/current-plan.md，本次以该计划为准。
 81 | - 任务1结果: 已确认 ch_battery_removed_check_10ms 修复疑似拔除后 return 0 导致后续充电状态重新开输出的问题。
 82 | - 疑似拔除启动: `val.curr < iGED` 且 `no_rx_10ms >= 100U` 后，立即 ch_output_all_off(), s_remove_cnt=1U, return 1。
 83 | - 20ms 等待: s_remove_cnt != 0U 分支每轮先 ch_output_all_off()，未到 20ms return 1。
 84 | - 20ms 后: `val.vout < vRESET` 回 CH_IDLE；`val.vout >= vRESET` 进入 BMS_ERR；两条路径都 return 1，不继续执行后续充电逻辑。
 85 | - 检查: 未发现 `iGED / 2` 作为充电中拔除阈值。
 86 | - 任务2结果: 已检查 uart_printf/printf/pc_uart_print_current_line/u1w_dbg_put_str 等运行时中文打印字符串中的 C51 0xFD/\xFD 问题。
 87 | - 已补 \xFD: YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c 三处 `" 数据="` 改为 `" 数据\xFD="`。
 88 | - 已确认保留中文，未改英文/ASCII，未修改普通中文注释。
 89 | - 复扫结果: 运行时打印里 GBK 第二字节为 FD 的中文位置均已有 \xFD；pc_uart.c 原有 `过\xFD流恢复`、`放大倍数\xFD` 保持不动。
 90 | - 默认协议日志: U1W_DEBUG_EN=0, U1W_DEBUG_PARSE_EN=0，未打开。
 91 | - CodeGraph: 已 sync；目标函数唯一上游为 usr_ch_func，影响范围集中在 ch.c 状态机接管逻辑。
 92 | - Keil Build: 0 Error(s), 0 Warning(s)。
 93 | - Program Size: data=157.4 xdata=315 const=146 code=14957；按 16K 上限 16256 计算，剩余约 1299 字节。
 94 | - 编码: ch.c 与 uart_1_wire_debug.c 均保持 CP936/GBK、无 BOM、LF，未发现 emoji。
 95 | ## 2026-06-28 本轮进度: uart_1_wire 握手标志修复
 96 | - 工作目录: D:\Users\CODE。
 97 | - 已按要求先读取 YAT-60V3A-Uart-1-wire/memory.md，再读取 .ai-bridge/current-plan.md。
 98 | - 已只回退 App/uart_1_wire.c 中 ChatGPT 刚才新增的 u1w_mark_valid_frame(u8 cmd) 及相关调用结构；未回退其它源码文件。
 99 | - 最终 App/uart_1_wire.c 只保留最小补丁: handshake_mask 只在 s_u1w.stage == U1W_STAGE_HANDSHAKE 时更新。
100 | - 校验失败帧不会调用 u1w_parse_frame()，因此不会更新 handshake_mask；长度不够、未知 CMD 也不会更新 handshake_mask。
101 | - 非 HANDSHAKE 阶段收到合法帧只刷新数据和 key_age，不再增加 handshake_mask。
102 | - App/uart_1_wire.c 保持 CP936/GBK、无 BOM、CRLF；未新增中文注释，未批量格式化，未批量转码。
103 | - 按用户最新规则，后续默认 U1W_DEBUG_EN=1U、U1W_DEBUG_PARSE_EN=1U，用于查看最大 CODE 占用；U1W_DEBUG_CMD_FILTER=0U；需要改 0 由用户自己改。
104 | - 未改 pc_uart；未改 DBG_BAUD=115200UL；未改 Flash/EEPROM 布局。
105 | - Keil Build 命令: D:\Keil_v5\UV4\UV4.exe -b Project.uvproj -j0。
106 | - Keil Build 结果: 0 Error(s), 0 Warning(s)。
107 | - Program Size: data=163.4 xdata=315 const=146 code=15784。
108 | - 按 code + const + 128 计算: 15784 + 146 + 128 = 16058，小于 16384，剩余约 326 字节。
109 | - 已将“后续默认调试打开，需要改 0 由用户自己改”的规则写入 YAT-60V3A-Uart-1-wire/memory.md。
110 | - memory.md 为未跟踪文件，写入前已备份: YAT-60V3A-Uart-1-wire/memory.md.bak_encoding_20260628_105912。
111 | - 真机风险: 仍需实测坏校验帧不会推进握手完成、HANDSHAKE 完成后正常切阶段、打开一线日志时串口时序是否影响现场通信。
112 | 

--- .ai-bridge/decisions.md ---
1 | # Decisions
2 | 
3 | 

--- .ai-bridge/open-questions.md ---
1 | # Open Questions
2 | 
3 | 

--- .ai-bridge/execution-log.jsonl ---
 1 | {"ts":"2026-06-27T08:16:39.626Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"补齐 B4 故障保护","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 2 | {"ts":"2026-06-27T08:47:06.213Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"按异步轮询思路补齐 B4 故障保护","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 3 | {"ts":"2026-06-27T09:06:57.574Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"按PDF修正一线通信与充电保护计划","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 4 | {"ts":"2026-06-27T09:25:10.245Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"补充低压插入与拔除判断说明","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 5 | {"ts":"2026-06-27T09:26:49.487Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"补充插入稳定后再判断低压","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 6 | {"ts":"2026-06-27T09:30:11.529Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修正充电中拔除判断流程","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 7 | {"ts":"2026-06-27T09:32:43.865Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"确认源码中IEND对应iGED","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 8 | {"ts":"2026-06-27T10:25:51.406Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"按PDF修正充电保护与拔除判断","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
 9 | {"ts":"2026-06-27T10:30:16.822Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"追加16K容量约束","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
10 | {"ts":"2026-06-27T10:33:25.582Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"追加资料优先级规则","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
11 | {"ts":"2026-06-27T14:22:17.262Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修复充电中拔除等待期间重新开输出问题","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
12 | {"ts":"2026-06-27T14:34:21.317Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"补充检查C51打印中文乱码","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
13 | {"ts":"2026-06-27T14:38:13.246Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修正打印检查要求：保留中文","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
14 | {"ts":"2026-06-27T14:44:46.655Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修复拔除等待与C51中文打印0xFD检查","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
15 | {"ts":"2026-06-27T15:34:54.953Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"优化一线协议调试短日志","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
16 | {"ts":"2026-06-27T15:38:49.833Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"优化一线协议调试可读短日志","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
17 | {"ts":"2026-06-27T16:04:36.715Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"修复一线调试日志全开超16K","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
18 | {"ts":"2026-06-28T01:09:00.096Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志交接","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
19 | {"ts":"2026-06-28T01:09:11.566Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志交接追加","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
20 | {"ts":"2026-06-28T01:09:18.989Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志交接追加","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
21 | {"ts":"2026-06-28T01:09:30.864Z","event":"handoff_to_agent","agent":"codex","agent_name":"Codex","title":"YAT一线调试日志最终交接","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
22 | {"ts":"2026-06-28T02:37:10.558Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"修复一线握手标志健壮性","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
23 | {"ts":"2026-06-28T02:39:57.763Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Revert ChatGPT uart_1_wire.c change then fix safely","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
24 | {"ts":"2026-06-28T03:38:13.038Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Simplify 1-wire handshake code only","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
25 | {"ts":"2026-06-28T03:38:48.503Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Simplify 1-wire handshake code only","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
26 | {"ts":"2026-06-28T03:49:49.086Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"Codex restart alignment then simplify 1-wire handshake","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
27 | {"ts":"2026-06-28T04:17:43.213Z","event":"handoff_to_codex","agent":"codex","agent_name":"Codex","title":"只补本轮干净 implementation-diff.patch","plan_path":".ai-bridge/current-plan.md","status_path":".ai-bridge/agent-status.md","diff_path":".ai-bridge/implementation-diff.patch"}
28 |

## Selected Files

Changed files detected: .ai-bridge/codex-status.md, .ai-bridge/current-plan.md, .ai-bridge/execution-log.jsonl, .ai-bridge/implementation-diff.patch, .ai-bridge/session-log.jsonl, CHECK_CODEXPRO_WORKSPACE_ASCII_STAY.bat, YAT-60V3A-Uart-1-wire/App/ch.c, YAT-60V3A-Uart-1-wire/App/main.c, YAT-60V3A-Uart-1-wire/App/pc_uart.c, YAT-60V3A-Uart-1-wire/App/pc_uart.h, YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c, YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h, YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt, YAT-60V3A-Uart-1-wire/Project/Keil/config.ini, YAT-60V3A-Uart-1-wire//346/234/254/350/275/256/345/274/202/346/255/245/345/215/217/350/256/256/344/270/216/347/212/266/346/200/201/346/234/272/350/247/204/345/210/231/350/257/264/346/230/216.md, YAT-21V4A-6A/, YAT-60V3A-Uart-1-wire/60V 3A /351/200/232/344/277/241/345/215/217/350/256/256.pdf, YAT-60V3A-Uart-1-wire/63V 3A /345/205/205/347/224/265/345/231/250/350/247/204/346/240/274.pdf, YAT-60V3A-Uart-1-wire/Project/Keil/build_codex.log, YAT-60V3A-Uart-1-wire/Project/Keil/codex_build_debug_on.log, YAT-60V3A-Uart-1-wire/Project/Keil/codex_build_debug_on2.log, YAT-60V3A-Uart-1-wire/Project/Keil/codex_build_default.log, YAT-60V3A-Uart-1-wire/Project/Keil/codex_build_tmp.log, /344/270/255/346/226/207/347/274/226/347/240/201/345/256/211/345/205/250/345/244/204/347/220/206Skill.md
Auto-include important root files: yes
Auto-include changed files: no
Explicit selected paths: YAT-60V3A-Uart-1-wire/App/ch.c, YAT-60V3A-Uart-1-wire/App/main.c, YAT-60V3A-Uart-1-wire/App/pc_uart.c, YAT-60V3A-Uart-1-wire/App/pc_uart.h, YAT-60V3A-Uart-1-wire/App/uart_1_wire.c, YAT-60V3A-Uart-1-wire/App/uart_1_wire.h, YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c, YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
Extra globs: none
Files included below: AGENTS.md, YAT-60V3A-Uart-1-wire/App/ch.c, YAT-60V3A-Uart-1-wire/App/main.c, YAT-60V3A-Uart-1-wire/App/pc_uart.c, YAT-60V3A-Uart-1-wire/App/pc_uart.h, YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c, YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h, YAT-60V3A-Uart-1-wire/App/uart_1_wire.c, YAT-60V3A-Uart-1-wire/App/uart_1_wire.h

## File Contents

### AGENTS.md

Bytes: 30237
SHA-256: 6b2e2b4cba4496eb5c7d8f17f713a4f8daebf81b0e3a1c5eb841c573ccce1d61
Lines: 1-1205 of 1205

```markdown
   1 | # AGENTS.md - AI 工具协作与编码保护规则
   2 | 
   3 | > 本文件放在项目根目录，用于约束 Codex / Cursor / Claude Code / Copilot / 通义灵码等 AI 工具修改本工程代码时的行为。  
   4 | > 本规则适用于 Keil、C51、MDK、IAR、Code::Blocks、Source Insight、VS Code、GCC、Makefile、CMake、Python 上位机、手机 APP、网页后台等工程。  
   5 | > 开始处理本工程前，必须先读取并遵守项目根目录的 `AGENTS.md` 和 `USER.md`。
   6 | 
   7 | ---
   8 | 
   9 | # 0. 核心目标
  10 | 
  11 | 本文件的目标不是限制 AI 发挥，而是保证 AI 修改工程时：
  12 | 
  13 | 1. 不改坏编码；
  14 | 2. 不破坏老工程风格；
  15 | 3. 不做无关重构；
  16 | 4. 不引入不可控依赖；
  17 | 5. 修改可审查、可回退、可交付。
  18 | 
  19 | 一句话原则：
  20 | 
  21 | ```text
  22 | 能小改，不大改；能说明，不猜测；能验证，不冒险。
  23 | ```
  24 | 
  25 | ---
  26 | 
  27 | # 1. 默认工作模式
  28 | 
  29 | 除非用户明确说：
  30 | 
  31 | ```text
  32 | 允许写入
  33 | ```
  34 | 
  35 | 否则 AI 默认只能执行：
  36 | 
  37 | 1. 只读查看文件；
  38 | 2. 分析问题；
  39 | 3. 给出修改方案；
  40 | 4. 给出人工合入补丁；
  41 | 5. 给出需要替换、插入、删除的代码片段。
  42 | 
  43 | 默认禁止：
  44 | 
  45 | 1. 写文件；
  46 | 2. apply patch；
  47 | 3. 自动格式化；
  48 | 4. 整文件重写；
  49 | 5. 批量替换；
  50 | 6. 自动转码；
  51 | 7. 自动修复乱码。
  52 | 
  53 | ---
  54 | 
  55 | # 2. Git 工程的写入与备份策略
  56 | 
  57 | 用户明确补充：
  58 | 
  59 | ```text
  60 | 有 Git 管理的工程，不需要每次写入前再额外生成 .bak 备份文件。
  61 | ```
  62 | 
  63 | 因此本工程采用以下规则。
  64 | 
  65 | ## 2.1 如果当前目录是 Git 仓库
  66 | 
  67 | AI 在写文件前必须先检查：
  68 | 
  69 | ```bash
  70 | git status --short
  71 | git branch --show-current
  72 | ```
  73 | 
  74 | 如果满足以下条件：
  75 | 
  76 | 1. 当前目录是 Git 仓库；
  77 | 2. 目标文件已经被 Git 跟踪；
  78 | 3. 工作区干净，或者本次修改范围与用户已有改动不冲突；
  79 | 4. 用户已经明确说“允许写入”；
  80 | 
  81 | 则允许直接修改文件，不强制创建 `.bak_encoding_YYYYMMDD_HHMMSS` 备份。
  82 | 
  83 | 写入后必须输出：
  84 | 
  85 | ```text
  86 | GIT_GUARD_REPORT
  87 | 
  88 | - 是否 Git 仓库：
  89 | - 当前分支：
  90 | - 修改文件：
  91 | - 写入前工作区状态：
  92 | - 写入后工作区状态：
  93 | - 建议用户查看：git diff
  94 | - 回退方式：git checkout -- <file> 或 git restore <file>
  95 | ```
  96 | 
  97 | ## 2.2 如果 Git 工作区不干净
  98 | 
  99 | 如果目标文件本来就有用户未提交改动，AI 必须谨慎。
 100 | 
 101 | 允许行为：
 102 | 
 103 | 1. 只改本任务直接相关的最小范围；
 104 | 2. 修改前说明检测到未提交改动；
 105 | 3. 修改后提醒用户用 `git diff` 核对；
 106 | 4. 不覆盖用户已有改动。
 107 | 
 108 | 如果无法保证不覆盖用户已有改动，必须停止：
 109 | 
 110 | ```text
 111 | GIT_GUARD_STOP: 目标文件已有未提交改动，无法保证不覆盖用户修改，未写入文件。
 112 | ```
 113 | 
 114 | ## 2.3 如果不是 Git 仓库
 115 | 
 116 | 如果当前目录不是 Git 仓库，且用户允许写入，则写入编码敏感文件前必须创建备份。
 117 | 
 118 | 备份文件命名格式：
 119 | 
 120 | ```text
 121 | 原文件名.bak_encoding_YYYYMMDD_HHMMSS
 122 | ```
 123 | 
 124 | 示例：
 125 | 
 126 | ```text
 127 | main.c.bak_encoding_20260511_213000
 128 | uart.c.bak_encoding_20260511_213000
 129 | protocol.h.bak_encoding_20260511_213000
 130 | ```
 131 | 
 132 | ## 2.4 如果目标文件未被 Git 跟踪
 133 | 
 134 | 即使当前目录是 Git 仓库，如果目标文件是未跟踪文件，也不能把 Git 当成备份。
 135 | 
 136 | 处理方式：
 137 | 
 138 | 1. 优先提醒用户该文件未被 Git 跟踪；
 139 | 2. 如果要直接写入，应先备份；
 140 | 3. 或建议用户先执行 `git add <file>` 后再继续。
 141 | 
 142 | ---
 143 | 
 144 | # 3. 编码保护最高原则
 145 | 
 146 | 编码保护优先级高于功能修改。
 147 | 
 148 | 如果编码保护和功能修改冲突，必须放弃功能修改，先保护编码。
 149 | 
 150 | 如果无法确认编码安全，唯一允许的行为是停止并输出人工合入补丁：
 151 | 
 152 | ```text
 153 | ENCODING_GUARD_STOP: 无法保证编码不变，未修改文件。
 154 | ```
 155 | 
 156 | ---
 157 | 
 158 | # 4. 编码敏感文件类型
 159 | 
 160 | 以下文件默认视为编码敏感文件：
 161 | 
 162 | ```text
 163 | .c
 164 | .h
 165 | .hpp
 166 | .cpp
 167 | .cc
 168 | .s
 169 | .S
 170 | .asm
 171 | .inc
 172 | .a51
 173 | .lst
 174 | .map
 175 | .txt
 176 | .md
 177 | .ini
 178 | .cfg
 179 | .bat
 180 | .cmd
 181 | .mak
 182 | makefile
 183 | Makefile
 184 | CMakeLists.txt
 185 | .uvproj
 186 | .uvprojx
 187 | .uvopt
 188 | .uvoptx
 189 | .ewp
 190 | .eww
 191 | .project
 192 | .py
 193 | .spec
 194 | .js
 195 | .ts
 196 | .tsx
 197 | .jsx
 198 | .json
 199 | .yaml
 200 | .yml
 201 | ```
 202 | 
 203 | 只要文件中可能含有中文注释、中文字符串、中文日志，就必须按编码敏感文件处理。
 204 | 
 205 | ---
 206 | 
 207 | # 5. 修改前只读检查
 208 | 
 209 | 在修改任何编码敏感文件前，必须先做只读检查。
 210 | 
 211 | 只读检查阶段禁止写文件，禁止 apply patch，禁止格式化，禁止保存。
 212 | 
 213 | 必须输出：
 214 | 
 215 | ```text
 216 | ENCODING_GUARD_PRECHECK
 217 | 
 218 | - 文件路径：
 219 | - 文件大小：
 220 | - 是否存在 BOM：
 221 | - 推测编码：
 222 | - 行尾格式：CRLF / LF / mixed / unknown
 223 | - 是否混合行尾：
 224 | - 是否包含无法按 UTF-8 解码的字节：
 225 | - 是否包含中文：
 226 | - 是否包含 emoji：
 227 | - 是否包含非 GBK/CP936 可表示字符：
 228 | - 是否存在疑似乱码文本：
 229 | - 是否 Git 跟踪：
 230 | - 是否需要额外备份：
 231 | - 是否允许安全写入：是 / 否
 232 | ```
 233 | 
 234 | 如果文件疑似 GBK / CP936 / ANSI，必须输出：
 235 | 
 236 | ```text
 237 | ENCODING_GUARD: 该文件疑似 GBK/CP936/ANSI，禁止使用普通 UTF-8 写文件方式。
 238 | ```
 239 | 
 240 | 如果无法判断编码，必须停止：
 241 | 
 242 | ```text
 243 | ENCODING_GUARD_STOP: 无法确认原始编码，未修改文件。
 244 | ```
 245 | 
 246 | ---
 247 | 
 248 | # 6. 编码判断要求
 249 | 
 250 | 不能只看文件扩展名判断编码。
 251 | 
 252 | 必须综合判断：
 253 | 
 254 | 1. 是否有 BOM；
 255 | 2. 是否能被 UTF-8 严格解码；
 256 | 3. 是否能被 GBK / CP936 解码；
 257 | 4. 文件中中文是否可读；
 258 | 5. 是否存在典型乱码；
 259 | 6. 文件历史和工程类型；
 260 | 7. 当前编辑器和编译器环境。
 261 | 
 262 | 常见判断规则：
 263 | 
 264 | ```text
 265 | 有 UTF-8 BOM：
 266 |     可以认为是 UTF-8 BOM，但仍需确认工程是否允许。
 267 | 
 268 | 无 BOM，UTF-8 严格解码成功：
 269 |     只能说明“可能是 UTF-8”，不能直接整文件重写。
 270 | 
 271 | 无 BOM，UTF-8 严格解码失败，但 GBK/CP936 可解码：
 272 |     高概率是 GBK/CP936/ANSI。
 273 | 
 274 | GBK 和 UTF-8 都能解码：
 275 |     必须结合中文显示效果判断，不能擅自转换。
 276 | 
 277 | 文件中没有中文：
 278 |     仍需保持原字节风格、原 BOM 状态和原行尾格式。
 279 | ```
 280 | 
 281 | ---
 282 | 
 283 | # 7. 绝对禁止项
 284 | 
 285 | 以下行为禁止执行：
 286 | 
 287 | 1. 禁止擅自把源码文件转换成 UTF-8。
 288 | 2. 禁止擅自把源码文件转换成 UTF-8 BOM。
 289 | 3. 禁止擅自把源码文件转换成 UTF-16。
 290 | 4. 禁止整文件重写。
 291 | 5. 禁止无关格式化。
 292 | 6. 禁止批量格式化整个文件或整个目录。
 293 | 7. 禁止跨大段正则替换。
 294 | 8. 禁止自动整理 include。
 295 | 9. 禁止自动重排函数。
 296 | 10. 禁止自动删除空行。
 297 | 11. 禁止自动转换 Tab 和空格。
 298 | 12. 禁止修改行尾格式。
 299 | 13. 禁止修改文件末尾是否带换行的状态。
 300 | 14. 禁止新增 emoji。
 301 | 15. 禁止新增特殊图标字符。
 302 | 16. 禁止新增原编码不可表示的字符。
 303 | 17. 禁止为了“修复乱码”而自动转码。
 304 | 18. 禁止顺手优化与任务无关的代码。
 305 | 19. 禁止顺手润色与任务无关的文案。
 306 | 20. 禁止无依据改动协议、阈值、保护逻辑。
 307 | 
 308 | 说明：
 309 | 
 310 | ```text
 311 | 如果工程已经由 Git 管理，并且满足第 2 节条件，则“不生成额外 .bak 备份”不算违反本规则。
 312 | ```
 313 | 
 314 | ---
 315 | 
 316 | # 8. 唯一允许的安全写入流程
 317 | 
 318 | 如果用户明确允许写入，并且编码已确认，必须按下面流程执行：
 319 | 
 320 | 1. 用二进制方式读取原始文件字节。
 321 | 2. 记录原始文件哈希。
 322 | 3. 检测并记录 BOM。
 323 | 4. 检测并记录行尾格式。
 324 | 5. 按原始编码解码。
 325 | 6. 只修改和任务直接相关的最小行范围。
 326 | 7. 新增内容必须能被原编码编码。
 327 | 8. 按同一原始编码重新编码。
 328 | 9. 保持原始 BOM 状态。
 329 | 10. 保持原始行尾格式。
 330 | 11. 写入临时文件。
 331 | 12. 用原编码重新读取临时文件验证。
 332 | 13. 对比差异，确认只有预期修改。
 333 | 14. 替换原文件。
 334 | 15. 再次读取验证。
 335 | 16. 输出编码保护报告。
 336 | 17. 如果是 Git 仓库，输出 Git 差异查看建议。
 337 | 
 338 | ---
 339 | 
 340 | # 9. GBK / CP936 / ANSI 文件写入要求
 341 | 
 342 | 如果目标文件是 GBK / CP936 / ANSI：
 343 | 
 344 | 1. 必须使用 `gbk` 或 `cp936` 解码。
 345 | 2. 必须使用同一编码写回。
 346 | 3. 禁止使用默认编码。
 347 | 4. 禁止使用 UTF-8 写回。
 348 | 5. 禁止新增 emoji。
 349 | 6. 禁止新增 GBK 不能表示的字符。
 350 | 7. 新增中文注释必须是普通简体中文。
 351 | 8. 新增日志建议使用 ASCII 标签加普通中文。
 352 | 9. 标点尽量使用 ASCII 标点。
 353 | 10. 不确定字符能否表示时，必须先停止。
 354 | 
 355 | 推荐：
 356 | 
 357 | ```c
 358 | usr_printf("[UART] 接收超时\r\n");
 359 | usr_printf("[BMS] 等待握手成功\r\n");
 360 | usr_printf("[CHG] 进入恒流恒压阶段\r\n");
 361 | ```
 362 | 
 363 | 不推荐：
 364 | 
 365 | ```c
 366 | usr_printf("【UART】接收超时\r\n");
 367 | usr_printf("→ 等待握手成功\r\n");
 368 | usr_printf("● 进入恒流恒压阶段\r\n");
 369 | ```
 370 | 
 371 | 禁止：
 372 | 
 373 | ```c
 374 | usr_printf("✅ UART接收成功\r\n");
 375 | usr_printf("🎬 进入充电流程\r\n");
 376 | usr_printf("⚠️ 电池温度异常\r\n");
 377 | ```
 378 | 
 379 | ---
 380 | 
 381 | # 10. UTF-8 文件写入要求
 382 | 
 383 | 如果目标文件确认是 UTF-8：
 384 | 
 385 | 1. 也不能整文件重写。
 386 | 2. 也不能无关格式化。
 387 | 3. 也不能新增 emoji，除非用户明确允许。
 388 | 4. 也必须保持原行尾。
 389 | 5. 也必须保持 BOM 状态。
 390 | 6. 如果工程是嵌入式老工程，即使文件是 UTF-8，也建议日志中只使用 ASCII 标签和普通中文。
 391 | 
 392 | ---
 393 | 
 394 | # 11. 文案、注释、日志保护规则
 395 | 
 396 | 禁止自作主张“优化”文案。
 397 | 
 398 | 除非用户明确要求修改文字表达，否则不得修改：
 399 | 
 400 | 1. 中文注释；
 401 | 2. 英文注释；
 402 | 3. 调试日志；
 403 | 4. printf / usr_printf / LOG 输出文本；
 404 | 5. 错误提示文本；
 405 | 6. 状态说明文本；
 406 | 7. 文档说明文字；
 407 | 8. README / MD 文档内容；
 408 | 9. 用户原有命名习惯；
 409 | 10. 用户原有描述风格。
 410 | 
 411 | 禁止因为以下理由擅自改文案：
 412 | 
 413 | 1. 看起来更通顺；
 414 | 2. 看起来更专业；
 415 | 3. 看起来更简洁；
 416 | 4. 看起来更统一；
 417 | 5. 看起来更符合 AI 的表达习惯；
 418 | 6. 顺手整理；
 419 | 7. 顺手润色；
 420 | 8. 顺手规范化。
 421 | 
 422 | 允许修改文案的情况：
 423 | 
 424 | 1. 用户明确要求修改该处文案；
 425 | 2. 原文案导致编译错误；
 426 | 3. 原文案导致编码风险；
 427 | 4. 原文案与本次代码逻辑修改强相关，不改会造成明显误导。
 428 | 
 429 | 源码、注释、日志、字符串中禁止新增：
 430 | 
 431 | ```text
 432 | emoji
 433 | 特殊图标
 434 | 不可见控制字符
 435 | 不确定编码兼容性的符号
 436 | 非工程必要的 Unicode 符号
 437 | ```
 438 | 
 439 | 推荐日志标签只使用 ASCII 字符：
 440 | 
 441 | ```text
 442 | [INIT]
 443 | [OK]
 444 | [ERR]
 445 | [WARN]
 446 | [INFO]
 447 | [ADC]
 448 | [UART]
 449 | [CAN]
 450 | [BMS]
 451 | [CHG]
 452 | [PRE]
 453 | [CCCV]
 454 | [FULL]
 455 | [TEST]
 456 | ```
 457 | 
 458 | ---
 459 | 
 460 | # 12. 最小修改原则
 461 | 
 462 | 修改代码时必须遵守：
 463 | 
 464 | 1. 只改本任务直接相关代码。
 465 | 2. 修改范围越小越好。
 466 | 3. 能改 1 行，不改 3 行。
 467 | 4. 能改 1 个函数，不改整个文件。
 468 | 5. 不改无关变量名。
 469 | 6. 不改无关注释。
 470 | 7. 不调整无关缩进。
 471 | 8. 不改变代码风格。
 472 | 9. 不引入新的编码风险。
 473 | 10. 不做顺手优化。
 474 | 11. 不擅自修改协议字段、大小端、阈值、保护策略。
 475 | 12. 不擅自改变原有状态机跳转条件。
 476 | 
 477 | 禁止因为“看起来更整洁”而扩大修改范围。
 478 | 
 479 | ---
 480 | 
 481 | # 13. 嵌入式 C 工程规则
 482 | 
 483 | 处理 MCU / C51 / STM32 / PY32 / PIC / AVR / GD32 / NXP 等工程时，默认遵守：
 484 | 
 485 | 1. 优先兼容 C89/C90。
 486 | 2. 变量声明尽量放在代码块开头。
 487 | 3. 默认不使用 `malloc/free`。
 488 | 4. 默认不使用 `float/double`。
 489 | 5. 默认不引入大型库。
 490 | 6. 默认不使用递归。
 491 | 7. 中断中只做必要动作。
 492 | 8. 通信接收优先中断 + FIFO。
 493 | 9. 主循环或周期任务处理复杂逻辑。
 494 | 10. 状态机跳转必须有明确条件。
 495 | 11. 协议解析必须明确大小端、长度、校验、超时。
 496 | 12. ADC / 滤波 / 控制算法优先整数化。
 497 | 
 498 | ---
 499 | 
 500 | # 14. 电脑端串口调试工具规则
 501 | 
 502 | 处理 Python / PySide6 / PyQt / Tkinter / C# / Electron 等上位机工具时，默认遵守：
 503 | 
 504 | 1. 串口接收不能卡 UI。
 505 | 2. 长任务放线程或异步处理。
 506 | 3. UI 操作必须有明确反馈。
 507 | 4. 日志、导出、缓存大小要清楚。
 508 | 5. CSV 导出字段要稳定。
 509 | 6. 配置项应可保存和恢复。
 510 | 7. 打包前说明依赖和 PyInstaller 注意事项。
 511 | 8. 修改 UI 时不要只追求好看，必须适合测试人员操作。
 512 | 9. 对串口协议要保留原始帧显示和解析结果。
 513 | 10. 异常断开、端口占用、权限不足要有提示。
 514 | 
 515 | ---
 516 | 
 517 | # 15. 手机端设备 APP 规则
 518 | 
 519 | 处理 Android / iOS / React Native / Flutter / Expo 等手机端应用时，默认遵守：
 520 | 
 521 | 1. 这是设备配套 APP，不是普通互联网 APP。
 522 | 2. 优先保证连接稳定、提示清楚、流程可靠。
 523 | 3. 蓝牙 / 串口 / 网络通信必须考虑断连、重连、超时。
 524 | 4. 每次连接成功后，如项目需要，应同步设备时间。
 525 | 5. 参数配置必须有确认、保存、失败提示。
 526 | 6. 测试结果、历史记录、导出流程要清楚。
 527 | 7. UI 适合现场测试人员，不要为了炫酷牺牲稳定性。
 528 | 8. 协议字段不要凭空编造，必须以项目文档或源码为准。
 529 | 
 530 | ---
 531 | 
 532 | # 16. 文档与交付规则
 533 | 
 534 | 用户经常需要把内容交给老板、客户或同事。
 535 | 
 536 | 输出文档时：
 537 | 
 538 | 1. 优先使用 Markdown。
 539 | 2. 结构清晰，标题分级明确。
 540 | 3. 先结论，再原因，再证据，再方案。
 541 | 4. 对老板版本要少讲废话，突出结论和解决方案。
 542 | 5. 对客户版本要有事实依据，避免情绪化。
 543 | 6. 对交接文档要写清楚路径、账号、配置、当前进度、风险点、后续目标。
 544 | 7. 不确定的信息必须标注“需核对”。
 545 | 
 546 | ---
 547 | 
 548 | # 17. 修改后必须复查
 549 | 
 550 | 修改后必须输出：
 551 | 
 552 | ```text
 553 | ENCODING_GUARD_REPORT
 554 | 
 555 | - 文件：
 556 | - 原编码：
 557 | - 写回编码：
 558 | - 原 BOM：
 559 | - 写回 BOM：
 560 | - 原行尾：
 561 | - 写回行尾：
 562 | - 是否混合行尾：
 563 | - 是否新增非 ASCII 字符：
 564 | - 是否新增非 GBK 字符：
 565 | - 是否包含 emoji：
 566 | - 是否发现乱码：
 567 | - 是否需要额外备份：
 568 | - 备份文件：
 569 | - 修改行范围：
 570 | - 是否只做最小修改：
 571 | ```
 572 | 
 573 | 如果是 Git 仓库，还必须输出：
 574 | 
 575 | ```text
 576 | GIT_GUARD_REPORT
 577 | 
 578 | - 当前分支：
 579 | - 修改文件：
 580 | - 写入前状态：
 581 | - 写入后状态：
 582 | - 建议检查命令：git diff
 583 | - 回退命令：git restore <file>
 584 | ```
 585 | 
 586 | 并贴出关键修改前后片段。
 587 | 
 588 | ---
 589 | 
 590 | # 18. 发现乱码后的处理
 591 | 
 592 | 如果修改后发现乱码风险，必须立即：
 593 | 
 594 | 1. 停止继续修改；
 595 | 2. 如果有备份，从备份恢复；
 596 | 3. 如果是 Git 仓库，提示使用 Git 回退；
 597 | 4. 不得尝试自动修复；
 598 | 5. 不得继续猜测编码；
 599 | 6. 输出回滚报告。
 600 | 
 601 | 回滚报告格式：
 602 | 
 603 | ```text
 604 | ENCODING_GUARD_ROLLBACK
 605 | 
 606 | - 文件：
 607 | - 原因：
 608 | - 是否已恢复：
 609 | - 恢复方式：
 610 | - 保留的修改：
 611 | - 当前状态：
 612 | ```
 613 | 
 614 | 如果无法回滚，必须明确报告：
 615 | 
 616 | ```text
 617 | ENCODING_GUARD_CRITICAL: 检测到乱码风险，但无法自动回滚，请立即人工检查备份或 Git 差异。
 618 | ```
 619 | 
 620 | ---
 621 | 
 622 | # 19. 工具能力不足时的唯一行为
 623 | 
 624 | 如果当前 AI 工具、编辑器、脚本环境无法做到以下任一项：
 625 | 
 626 | 1. 检测原始编码；
 627 | 2. 识别 BOM；
 628 | 3. 保持原编码写回；
 629 | 4. 保持原行尾；
 630 | 5. 只做最小行级修改；
 631 | 6. 写回后用原编码验证；
 632 | 7. 检查 Git 状态或创建备份；
 633 | 8. 差异复查；
 634 | 
 635 | 则禁止直接修改文件。
 636 | 
 637 | 必须输出：
 638 | 
 639 | ```text
 640 | ENCODING_GUARD_STOP: 当前工具无法保证编码不变，因此未修改文件。下面仅提供人工合入补丁。
 641 | ```
 642 | 
 643 | ---
 644 | 
 645 | # 20. 人工合入补丁格式
 646 | 
 647 | 如果不能安全写入文件，必须输出方便人工定位的补丁。
 648 | 
 649 | 核心原则：
 650 | 
 651 | ```text
 652 | 行号只能作为辅助定位，必须同时提供文件路径、函数名/结构体名/模块位置、原始行号范围和原代码片段，避免文件改动后行号漂移导致误合入。
 653 | ```
 654 | 
 655 | 模板：
 656 | 
 657 | ```text
 658 | 文件：xxx.c
 659 | 
 660 | 修改 1：
 661 | - 函数/位置：
 662 | - 原始行号：
 663 | - 修改类型：替换 / 插入 / 删除
 664 | - 定位关键字：
 665 | 
 666 | 定位原代码：
 667 | ```
 668 | 
 669 | ```c
 670 | // Lxxx
 671 | 原代码片段
 672 | ```
 673 | 
 674 | ```text
 675 | 替换为 / 插入内容 / 删除内容：
 676 | ```
 677 | 
 678 | ```c
 679 | // Lxxx
 680 | 新代码片段
 681 | ```
 682 | 
 683 | ```text
 684 | 说明：
 685 | - 修改原因：
 686 | - 修改范围：
 687 | - 是否包含中文：
 688 | - 是否包含非 ASCII：
 689 | - 是否包含 emoji：
 690 | - 是否需要按原编码保存：
 691 | - 是否需要保持原行尾：
 692 | - 是否需要人工复查：
 693 | ```
 694 | 
 695 | 强制要求：
 696 | 
 697 | 1. 有文件路径。
 698 | 2. 有函数名或模块位置。
 699 | 3. 有修改前原始行号范围。
 700 | 4. 有原代码定位片段。
 701 | 5. 有新代码片段。
 702 | 6. 有修改原因。
 703 | 7. 有编码风险说明。
 704 | 8. 有是否包含中文 / 非 ASCII / emoji 的说明。
 705 | 9. 有是否需要按原编码保存的说明。
 706 | 10. 有是否保持原行尾的说明。
 707 | 
 708 | ---
 709 | 
 710 | 
 711 | # 21. 推荐源码日志规范
 712 | 
 713 | 为了兼容老工程，建议统一采用 ASCII 标签 + 普通中文：
 714 | 
 715 | ```c
 716 | usr_printf("[INIT] 系统初始化完成\r\n");
 717 | usr_printf("[UART] 接收超时\r\n");
 718 | usr_printf("[ADC] 采样异常\r\n");
 719 | usr_printf("[BMS] 通信失败\r\n");
 720 | usr_printf("[CHG] 进入预充阶段\r\n");
 721 | usr_printf("[ERR] 输出过压\r\n");
 722 | ```
 723 | 
 724 | 不建议使用：
 725 | 
 726 | ```c
 727 | usr_printf("【初始化】系统初始化完成\r\n");
 728 | usr_printf("-----> 进入预充阶段\r\n");
 729 | usr_printf("※ 通信失败\r\n");
 730 | usr_printf("● 输出过压\r\n");
 731 | ```
 732 | 
 733 | 禁止使用：
 734 | 
 735 | ```c
 736 | usr_printf("🎬 系统初始化完成\r\n");
 737 | usr_printf("✅ 通信成功\r\n");
 738 | usr_printf("❌ 通信失败\r\n");
 739 | usr_printf("⚠️ 输出过压\r\n");
 740 | ```
 741 | 
 742 | ---
 743 | 
 744 | # 22. 建议的工程约定
 745 | 
 746 | 如果是 Windows 老嵌入式工程，建议在项目说明中明确：
 747 | 
 748 | ```text
 749 | 源码编码：保持原文件编码，不统一转码。
 750 | 老文件：多数为 GBK/CP936/ANSI。
 751 | 新文件：由项目负责人决定编码。
 752 | 行尾：保持原文件行尾。
 753 | 中文注释：允许，但必须保持原编码可表示。
 754 | 中文日志：允许，但禁止 emoji 和特殊图标。
 755 | AI 修改：默认只输出补丁，不直接写文件。
 756 | Git 工程：允许写入后以 git diff 审查，不强制额外 .bak。
 757 | 非 Git 工程：写入前必须备份。
 758 | ```
 759 | 
 760 | 如果项目决定统一 UTF-8，必须一次性规划迁移，不允许 AI 工具边改边混合转码。
 761 | 
 762 | ---
 763 | 
 764 | # 23. 最硬的一句话
 765 | 
 766 | 当非常担心 AI 工具改坏编码时，直接加这句：
 767 | 
 768 | ```text
 769 | 除非你能证明“读取编码、写回编码、BOM、行尾、修改范围、Git 状态或备份策略”全部可控，否则禁止修改文件，只能给我人工合入补丁。
 770 | ```
 771 | 
 772 | 更硬一点：
 773 | 
 774 | ```text
 775 | 如果你调用了任何可能整文件重写、自动保存、默认 UTF-8 写入、自动格式化、覆盖用户未提交改动的工具，本次任务视为失败。你必须停止并报告，不得继续。
 776 | ```
 777 | 
 778 | ---
 779 | 
 780 | # 24. 总结
 781 | 
 782 | 编码敏感工程的核心原则：
 783 | 
 784 | ```text
 785 | 先识别，再修改；
 786 | 有 Git，看 diff；
 787 | 无 Git，先备份；
 788 | 先保护编码，再修改功能；
 789 | 能小改，不大改；
 790 | 能给补丁，不乱写；
 791 | 不能证明安全，就停止。
 792 | ```
 793 | 
 794 | 不要相信“默认保存”。  
 795 | 不要相信“自动修复”。  
 796 | 不要相信“看起来没问题”。  
 797 | 必须以原始字节、原始编码、原始行尾、Git 差异、最小修改为准。
 798 | 
 799 | ---
 800 | 
 801 | # 25. CodeGraph / 代码图谱使用规则
 802 | 
 803 | 如果项目已经安装或启用了 CodeGraph，AI 工具在分析和修改代码前应优先使用 CodeGraph 做代码关系分析。
 804 | 
 805 | CodeGraph 的定位：
 806 | 
 807 | ```text
 808 | CodeGraph 不是替代 Codex / Cursor / Claude Code。
 809 | CodeGraph 是给 AI 工具提供代码地图，用于查函数、变量、调用链、影响范围和上下文。
 810 | ```
 811 | 
 812 | ## 25.1 使用前状态检查规则
 813 | 
 814 | AI 工具每次使用 CodeGraph 前，必须先在项目根目录检查 CodeGraph 当前状态。
 815 | 
 816 | 推荐顺序：
 817 | 
 818 | ```bash
 819 | codegraph status
 820 | ```
 821 | 
 822 | 如果项目还没有初始化 CodeGraph，应初始化：
 823 | 
 824 | ```bash
 825 | codegraph init -i
 826 | ```
 827 | 
 828 | 初始化完成后，必须再次查看状态：
 829 | 
 830 | ```bash
 831 | codegraph status
 832 | ```
 833 | 
 834 | 如果 CodeGraph 状态不是最新，或者提示索引过期、文件有变化、需要同步，应先同步：
 835 | 
 836 | ```bash
 837 | codegraph sync
 838 | ```
 839 | 
 840 | 同步完成后，必须再次查看状态：
 841 | 
 842 | ```bash
 843 | codegraph status
 844 | ```
 845 | 
 846 | 确认 CodeGraph 可用并且状态正常后，才继续查询函数、变量、调用链和影响范围。
 847 | 
 848 | 说明：
 849 | 
 850 | ```text
 851 | CodeGraph 索引是辅助分析文件，不是源码。
 852 | 是否提交 .codegraph 到 Git，由项目负责人决定。
 853 | 即使 CodeGraph 已同步，最终结论仍必须以源码和 git diff 为准。
 854 | ```
 855 | 
 856 | 
 857 | ## 25.2 修改代码前必须优先做 CodeGraph 分析
 858 | 
 859 | 涉及源码分析、BUG 定位、功能修改、重构建议时，AI 应优先使用 CodeGraph 查询：
 860 | 
 861 | 1. 目标函数在哪里定义；
 862 | 2. 谁调用了目标函数；
 863 | 3. 目标函数内部调用了谁；
 864 | 4. 相关全局变量在哪里定义；
 865 | 5. 相关全局变量在哪里读写；
 866 | 6. 目标模块与其他模块的依赖关系；
 867 | 7. 修改可能影响哪些文件和函数；
 868 | 8. 是否涉及中断、定时器、通信、状态机、ADC、PWM、显示、存储等关键路径。
 869 | 
 870 | 禁止在没有确认调用链和影响范围前直接修改关键代码。
 871 | 
 872 | ## 25.3 推荐的 CodeGraph 分析顺序
 873 | 
 874 | AI 工具应按以下顺序处理复杂代码问题：
 875 | 
 876 | ```text
 877 | 1. 用 CodeGraph 搜索目标符号；
 878 | 2. 查看目标函数定义位置；
 879 | 3. 查看 callers，确认上游调用关系；
 880 | 4. 查看 callees，确认下游依赖；
 881 | 5. 查看相关全局变量的读写位置；
 882 | 6. 查看相关文件和模块关系；
 883 | 7. 输出影响范围分析；
 884 | 8. 再读取具体源码片段；
 885 | 9. 给出最小修改方案；
 886 | 10. 用户允许写入后，才按 ENCODING_GUARD + GIT_GUARD 修改。
 887 | ```
 888 | 
 889 | ## 25.4 必须输出的 CodeGraph 分析报告
 890 | 
 891 | 在修改重要代码前，应先输出：
 892 | 
 893 | ```text
 894 | CODEGRAPH_ANALYSIS_REPORT
 895 | 
 896 | - 是否检测到 .codegraph：
 897 | - 目标函数 / 变量：
 898 | - 定义位置：
 899 | - 上游调用者：
 900 | - 下游被调用函数：
 901 | - 相关全局变量：
 902 | - 读写位置：
 903 | - 涉及文件：
 904 | - 可能影响的模块：
 905 | - 是否涉及 ISR / 定时器：
 906 | - 是否涉及通信协议：
 907 | - 是否涉及状态机：
 908 | - 是否涉及硬件控制：
 909 | - 风险点：
 910 | - 建议修改范围：
 911 | ```
 912 | 
 913 | 如果 CodeGraph 查询结果不完整，必须明确说明：
 914 | 
 915 | ```text
 916 | CODEGRAPH_WARNING: CodeGraph 结果不完整，下面结论需要结合源码人工复核。
 917 | ```
 918 | 
 919 | ## 25.5 嵌入式项目中的重点检查项
 920 | 
 921 | 处理 MCU / C51 / STM32 / PY32 / PIC / AVR / GD32 / NXP 等工程时，CodeGraph 分析应重点检查：
 922 | 
 923 | 1. 中断入口函数；
 924 | 2. 定时器周期任务；
 925 | 3. 主循环任务调度；
 926 | 4. 状态机入口和状态切换；
 927 | 5. UART / CAN / I2C / SPI 接收和发送流程；
 928 | 6. FIFO 写入和读取位置；
 929 | 7. ADC 采样、滤波和工程单位换算；
 930 | 8. PWM / GPIO / 继电器 / MOS 控制函数；
 931 | 9. 保护逻辑，例如过压、欠压、过流、过温、通信超时；
 932 | 10. 全局变量是否在 ISR 和主循环之间共享。
 933 | 
 934 | ## 25.6 不允许把 CodeGraph 当成最终依据
 935 | 
 936 | CodeGraph 只能作为辅助索引，不能替代源码复核。
 937 | 
 938 | 以下情况必须人工复核源码：
 939 | 
 940 | 1. 宏展开复杂；
 941 | 2. 条件编译复杂；
 942 | 3. Keil / C51 特殊关键字较多；
 943 | 4. 函数指针、回调、表驱动较多；
 944 | 5. 中断入口由启动文件或编译器关键字绑定；
 945 | 6. 代码通过宏拼接函数名或变量名；
 946 | 7. 老工程存在重复文件、备份文件、不同版本文件。
 947 | 
 948 | 如果 CodeGraph 与源码不一致，以源码为准。
 949 | 
 950 | ## 25.7 禁止行为
 951 | 
 952 | 使用 CodeGraph 时仍然禁止：
 953 | 
 954 | 1. 未经用户允许初始化项目；
 955 | 2. 未经用户允许写入 `.codegraph`；
 956 | 3. 只凭调用图直接改代码；
 957 | 4. 忽略 ENCODING_GUARD；
 958 | 5. 忽略 GIT_GUARD；
 959 | 6. 为了让 CodeGraph 解析成功而擅自修改源码；
 960 | 7. 为了索引方便而批量重命名文件或函数；
 961 | 8. 为了索引方便而改 include、宏定义、工程结构；
 962 | 9. 把 CodeGraph 的猜测结果当成协议或硬件事实。
 963 | 
 964 | 
 965 | ## 25.9 一句话原则
 966 | 
 967 | ```text
 968 | 先用 CodeGraph 看清战场，再用 Codex 修改代码；CodeGraph 负责找关系，Codex 负责分析和改代码，最终仍以源码和 git diff 为准。
 969 | ```
 970 | 
 971 | ---
 972 | 
 973 | # 26. 小范围修改的高效执行规则
 974 | 
 975 | 本章节适用于所有“只改一个函数、只删一个 helper、只补一个判断、只改一个宏”的小任务。
 976 | 
 977 | ## 26.1 先限定范围，再动代码
 978 | 
 979 | 小范围任务禁止直接全文件搜索替换。
 980 | 
 981 | 必须先确认：
 982 | 
 983 | 1. 目标文件；
 984 | 2. 目标函数；
 985 | 3. 目标函数起止边界；
 986 | 4. 同名 case、同名宏、同名变量是否也出现在其他函数里。
 987 | 
 988 | 例如同一个 `case U1W_CMD_A0:` 可能同时出现在：
 989 | 
 990 | 1. `u1w_reply_len()`；
 991 | 2. `u1w_parse_frame()`；
 992 | 3. 调试打印函数；
 993 | 4. 其他协议辅助函数。
 994 | 
 995 | 如果任务只要求改 `u1w_parse_frame()`，就只能在这个函数范围内改，不能全文件按 `case U1W_CMD_A0:` 插入或替换。
 996 | 
 997 | ## 26.2 禁止低效命令堆叠
 998 | 
 999 | 小改动不允许为了显得“做了很多”而反复跑无关命令。
1000 | 
1001 | 推荐最小流程：
1002 | 
1003 | ```text
1004 | 1. git status / show_changes，看当前工作区和目标文件 diff；
1005 | 2. 读取目标函数片段；
1006 | 3. 做编码预检；
1007 | 4. 只改目标函数或目标小块；
1008 | 5. 复读目标函数片段；
1009 | 6. 搜索旧符号确认没有残留；
1010 | 7. 运行必要编译；
1011 | 8. 输出简短报告。
1012 | ```
1013 | 
1014 | 如果中间发现命令方式不适合，例如 UTF-8 apply_patch 不能处理 CP936 文件，应立即切换到保编码方式，不要反复尝试可能破坏编码的方法。
1015 | 
1016 | ## 26.3 CP936 / GBK 老工程写入规则
1017 | 
1018 | 如果预检发现 `.c/.h/.txt/.md` 等文件是 CP936 / GBK / ANSI：
1019 | 
1020 | 1. 不要用默认 UTF-8 方式直接写；
1021 | 2. 不要整文件转码；
1022 | 3. 不要为了修改一小块而重写整文件；
1023 | 4. 应使用保留原编码、原 BOM、原行尾的精确替换或按行修改；
1024 | 5. 写入后必须复查编码、BOM、行尾。
1025 | 
1026 | 如果无法确认保编码写入，必须停止并给人工补丁。
1027 | 
1028 | ## 26.4 计划外问题处理规则
1029 | 
1030 | 执行 current-plan.md 时，如果发现计划外 BUG、可疑问题、顺手优化点：
1031 | 
1032 | 1. 不允许自己修改；
1033 | 2. 只允许记录到 `.ai-bridge/agent-status.md` 的 `Found but not changed`；
1034 | 3. 停下来等 ChatGPT Pro / 用户确认；
1035 | 4. 等 ChatGPT Pro 给出具体最小改法后才能继续。
1036 | 
1037 | 禁止“顺手修一下”。
1038 | 
1039 | ## 26.5 交付 diff 规则
1040 | 
1041 | `implementation-diff.patch` 必须对应本轮任务。
1042 | 
1043 | 如果本轮只改了一个文件，patch 就只应包含这个文件和必要的状态文件。禁止把旧任务的大 diff、历史 `ch.c` diff、无关日志文件混入本轮 patch。
1044 | 
1045 | 生成 patch 前必须确认：
1046 | 
1047 | ```text
1048 | 1. 本轮实际改了哪些文件；
1049 | 2. 哪些是历史未提交改动；
1050 | 3. 哪些不属于本轮；
1051 | 4. implementation-diff.patch 是否只包含本轮相关内容。
1052 | ```
1053 | 
1054 | ## 26.6 报告规则
1055 | 
1056 | 最终报告只写结论，不贴流水账。
1057 | 
1058 | 必须包含：
1059 | 
1060 | 1. 改了哪些文件；
1061 | 2. 没改哪些禁止文件；
1062 | 3. 核心逻辑是否满足计划；
1063 | 4. 编译结果；
1064 | 5. Program Size；
1065 | 6. 回退方式；
1066 | 7. 计划外问题是否有记录。
1067 | 
1068 | 不要把每一条命令过程都复制给用户，除非用户要求排查过程。
1069 | 
1070 | ---
1071 | 
1072 | # 27. GBK/CP936 中文源码读取与保编码修改技能
1073 | 
1074 | 本技能适用于 Keil C51 老工程中的 `.c/.h/.txt/.md/.ini/.uvproj` 等文本文件，特别是含中文注释、中文串口日志、GBK/ANSI 编码的文件。
1075 | 
1076 | ## 27.1 先判断编码，不要默认 UTF-8
1077 | 
1078 | 读取中文乱码时，不要立刻认为源码乱码。
1079 | 
1080 | 必须先做编码判断：
1081 | 
1082 | ```text
1083 | 1. 读取原始 bytes；
1084 | 2. 依次尝试 utf-8-sig、utf-8、gbk、cp936；
1085 | 3. 哪个能完整 decode，就记录为原编码；
1086 | 4. 如果 UTF-8 读取显示乱码，但 GBK/CP936 正常，说明文件本身没坏，是读取方式错了。
1087 | ```
1088 | 
1089 | 推荐检查命令：
1090 | 
1091 | ```text
1092 | PYTHONIOENCODING=utf-8 python -c "from pathlib import Path
1093 | for f in ['App/usr_cfg.h','App/ch.c','App/uart_1_wire.c']:
1094 |     b=Path(f).read_bytes()
1095 |     ok=[]
1096 |     for enc in ['utf-8-sig','utf-8','gbk','cp936']:
1097 |         try:
1098 |             b.decode(enc)
1099 |             ok.append(enc)
1100 |         except Exception:
1101 |             pass
1102 |     print(f, ok, 'bytes', len(b))"
1103 | ```
1104 | 
1105 | ## 27.2 GBK 文件读取中文时，要同时指定输出编码
1106 | 
1107 | 在 Windows / MSYS / Codex 工具中，即使 Python 已经用 GBK 正确 decode，如果控制台输出编码不对，显示仍可能乱码。
1108 | 
1109 | 读取 GBK 中文源码时，推荐使用：
1110 | 
1111 | ```text
1112 | PYTHONIOENCODING=utf-8 python -c "from pathlib import Path
1113 | p=Path('App/ch.c')
1114 | lines=p.read_bytes().decode('gbk', errors='replace').splitlines()
1115 | for i in range(1, 80):
1116 |     print(f'{i:4d} | {lines[i-1]}')"
1117 | ```
1118 | 
1119 | 关键点：
1120 | 
1121 | ```text
1122 | 1. 文件 decode 用 gbk/cp936；
1123 | 2. 控制台输出用 PYTHONIOENCODING=utf-8；
1124 | 3. 不要用默认 read 工具的 UTF-8 结果判断中文是否损坏。
1125 | ```
1126 | 
1127 | ## 27.3 修改 GBK 文件必须保留三件事
1128 | 
1129 | 修改前必须记录：
1130 | 
1131 | ```text
1132 | 1. 原编码：GBK/CP936/ANSI/UTF-8/UTF-8-SIG；
1133 | 2. 原 BOM：有 / 无；
1134 | 3. 原行尾：CRLF / LF。
1135 | ```
1136 | 
1137 | 修改后必须保证：
1138 | 
1139 | ```text
1140 | 1. 写回相同编码；
1141 | 2. 写回相同行尾；
1142 | 3. 不新增或删除 BOM；
1143 | 4. 不整文件转码；
1144 | 5. 不批量格式化。
1145 | ```
1146 | 
1147 | 对于 GBK 文件，推荐做法是：
1148 | 
1149 | ```text
1150 | b = path.read_bytes()
1151 | text = b.decode('gbk')
1152 | text = text.replace(old, new, 1)
1153 | path.write_bytes(text.encode('gbk'))
1154 | ```
1155 | 
1156 | 禁止做法：
1157 | 
1158 | ```text
1159 | path.write_text(text, encoding='utf-8')
1160 | ```
1161 | 
1162 | ## 27.4 修改后必须复查中文和编码
1163 | 
1164 | 修改后至少检查：
1165 | 
1166 | ```text
1167 | 1. 用 GBK/CP936 能否重新 decode；
1168 | 2. 文件是否仍然无 BOM / 原 BOM；
1169 | 3. 行尾是否仍为 CRLF；
1170 | 4. 中文注释和中文日志是否能按 GBK 正常显示；
1171 | 5. git diff 是否只包含计划内小范围修改。
1172 | ```
1173 | 
1174 | ## 27.5 处理 Keil C51 中文字符串里的 0xFD 风险
1175 | 
1176 | 部分 GBK 汉字本身含 `0xFD` 字节，例如：
1177 | 
1178 | ```text
1179 | 待 = B4 FD
1180 | 过 = B9 FD
1181 | 数 = CA FD
1182 | ```
1183 | 
1184 | 如果运行时串口日志中遇到这类字，可能被历史代码用 `\xFD` 规避，但这会导致日志里多出异常字节。
1185 | 
1186 | 优先方案不是硬塞 `\xFD`，而是换词避开这些字：
1187 | 
1188 | ```text
1189 | 等待BMS握手 -> 等BMS握手
1190 | 过压保护   -> 高压保护
1191 | 过温保护   -> 高温保护
1192 | 过流保护   -> 电流保护
1193 | 过流恢复   -> 电流恢复
1194 | 放大倍数   -> GAIN
1195 | ```
1196 | 
1197 | 中文注释可以保留这些字；运行时字符串尽量避开含 `0xFD` 的汉字。
1198 | 
1199 | ## 27.6 一句话原则
1200 | 
1201 | ```text
1202 | 先用 bytes 判断编码，再用正确编码读中文；改 GBK 文件必须 GBK 写回，不能默认 UTF-8。
1203 | ```
1204 | 
1205 | 
```

### YAT-60V3A-Uart-1-wire/App/ch.c

Bytes: 36036
SHA-256: 9422e77e1f9ffc28fb5751bdad7dd014e57c5713665d8ca7fa29df0f0cb57786
Lines: 1-1251 of 1251

```text
   1 | /**
   2 |   ******************************************************************************
   3 |   * @file    ch.c
   4 |   * @brief   ��������̡�
   5 |   *
   6 |   * ˵����
   7 |   * 1. ���ļ��� 60V3A/63V ��Ŀ����ʵ�֣�������״̬��ת��������ơ�
   8 |   * 2. P30 һ��ͨ��Э���� uart_1_wire.c/.h ��ʵ�֣����ļ�ֻ��ȡĿ���ѹ��������BMS״̬��
   9 |   * 3. ÿ��״̬�仯�����ӡ������־������ DEBUG �����ֳ��ж���������
  10 |   * 4. ��ѹ�������ͼ�ʱ��ֵͳһ���� usr_cfg.h������������ɢ��Ӳ���롣
  11 |   *
  12 |   * ͨ�Ŵ����߽磺
  13 |   * 1. BMS_HANDSHAKE �ȴ� A0/A1/A4/A6/A7/B1/B3/B4 ȫ�����롣
  14 |   * 2. ������״ֻ̬���ͨ�ų�ʱ��������Э��֡ϸ�ڡ�
  15 |   * 3. B6 ��Э����ڳ��׶ι̶�Ҫ��򿪳�� MOS��BMS״̬�ɱ��ļ����ദ����
  16 |   * 4. ����/��ͨ�쳣��Э����������� COM��BMS�¶��쳣����ͨ�ż��ָ���
  17 |   * 5. �ε��ȫ�ִ������̵����պ�ʱ�á�С����+��ͨ�š�������ر�ʱ�õ�ѹ��⡣
  18 |   ******************************************************************************
  19 |   */
  20 | #include "ch.h"
  21 | #include "adc.h"
  22 | #include "gpio.h"
  23 | #include "pwm.h"
  24 | #include "timer.h"
  25 | #include "bsp_init.h"
  26 | #include "cal.h"
  27 | #include "usr_cfg.h"
  28 | #include "pc_uart.h"
  29 | #include "uart_1_wire.h"
  30 | 
  31 | CH_STATUS_Types idata ch_state;
  32 | CH_STATUS_Types idata last_state;
  33 | 
  34 | static u8 idata s_cut[4];          /* ״̬��ȷ�ϼ�������ֹ�ٽ�㶶���� */
  35 | 
  36 | static u16 idata s_cccv_curr_limit_ma;        /* CCCV ʵ��������������λ mA�� */
  37 | static u8  idata s_cccv_derate_cnt;           /* CCCV ������������� */
  38 | static u8  idata s_remove_cnt;                /* �ε��ȷ�ϼ����� */
  39 | static u16 idata s_idle_low_last_mv;          /* ������ѹ��ѡ�ϴε�ѹ�� */
  40 | static u16 idata s_vout_probe_period_10ms;    /* ����/�쳣ʱ��ѹ����������� */
  41 | static u8  idata s_vout_probe_on_10ms;        /* ����/�쳣ʱ��ѹ��⿪�������� */
  42 | static bit s_vout_sample_valid;               /* ������ val.vout �Ƿ����������жϡ� */
  43 | 
  44 | #define BMS_HANDSHAKE_TIMEOUT_S      (20U)
  45 | #define CH_VOUT_PROBE_PERIOD_10MS    (100U)   /* ����/�쳣ʱÿ1���һ�η�ѹ��� */
  46 | #define CH_VOUT_PROBE_ON_10MS        (10U)    /* ÿ�δ�100ms */
  47 | #define CH_VOUT_PROBE_VALID_10MS     (5U)     /* ��50ms����ΪADC��Ч */
  48 | #define CH_BMS_TEMP_MASK             (U1W_B4_LOW_TEMP | U1W_B4_HIGH_TEMP | U1W_B4_MOS_HOT)
  49 | #define CH_BMS_ERR_MASK              (U1W_B4_OCP | U1W_B4_SHORT | U1W_B4_TIMEOUT | U1W_B4_FAIL)
  50 | 
  51 | /*
  52 |  * ״̬������
  53 |  * 1. �±������ CH_STATUS_Types ö��˳��һ�¡�
  54 |  * 2. ֻ���ڴ�����־��ʾ��������״̬���жϡ�
  55 |  * 3. ֱ�������ȡ�״ֵ̬+�ַ������Ľṹ����ʡ code��Ҳ��һ��ѭ���Ƚϡ�
  56 |  */
  57 | static char * code s_ch_state_name[] =
  58 | {
  59 |     "����",
  60 |     "���",
  61 |     "��BMS����",
  62 |     "Ԥ��",
  63 |     "������ѹ",
  64 |     "����",
  65 |     "��ѹ����",
  66 |     "Ԥ�䳬ʱ",
  67 |     "���±���",
  68 |     "��������",
  69 |     "NTC�쳣",
  70 |     "Ӳ���쳣",
  71 |     "Ƿѹ����",
  72 |     "CCCV��ʱ",
  73 |     "BMS�¶��쳣",
  74 |     "BMS�쳣",
  75 |     "����ѹ�޸�",
  76 |     "�ϻ�",
  77 | };
  78 | 
  79 | /**
  80 |   * @brief  ��״̬���Ա���ȡ����״̬����
  81 |   */
  82 | static char *ch_state_name(CH_STATUS_Types state)
  83 | {
  84 |     if((u8)state < ARRAY_SIZE(s_ch_state_name))
  85 |     {
  86 |         return s_ch_state_name[(u8)state];
  87 |     }
  88 | 
  89 |     return "δ֪";
  90 | }
  91 | 
  92 | /**
  93 |   * @brief  ͳһ״̬�л���ڡ�
  94 |   * @param  next_state: Ŀ��״̬��
  95 |   * @param  reason    : �����л�ԭ�򣬿ɴ���ָ�롣
  96 |   */
  97 | static void ch_set_state(CH_STATUS_Types next_state, char *reason)
  98 | {
  99 |     if(ch_state == next_state)
 100 |     {
 101 |         return;
 102 |     }
 103 | 
 104 |     /*
 105 |      * �ͻ����ֳ�������������־�ж����̡�
 106 |      * ���ﱣ��������Ϣ����ͳһΪһ�� printf��
 107 |      * 1. ���ٶ�� printf ���ú��ظ���ʽ�ַ��������� C51 ���������
 108 |      * 2. NTC ������Χ�� int �ڣ��� s16 ��ӡ���������� long printf ��ʽ��
 109 |      */
 110 |     uart_printf("״̬:%s -> %s :%s V=%umV I=%umA NTC=%d\n",
 111 |                 ch_state_name(ch_state),
 112 |                 ch_state_name(next_state),
 113 |                 (reason != 0) ? reason : "��",
 114 |                 val.vout,
 115 |                 val.curr,
 116 |                 (s16)val.i_ntc);
 117 | 
 118 |     ch_state = next_state;
 119 | }
 120 | 
 121 | /**
 122 |   * @brief  ������ռ��顣
 123 |   * @return 1 ��ʾ�����ڴ��ڳ����Ӳ������̬��
 124 |   */
 125 | static bit ch_check_protect_state(void)
 126 | {
 127 |     if(ch_flag.ch_ntcErr != 0)
 128 |     {
 129 |         ch_set_state(NTC_ERR, "NTC����·");
 130 |         return 1;
 131 |     }
 132 |     else if(ch_flag.ch_hotErr != 0)
 133 |     {
 134 |         ch_set_state(CH_OTP, "CH����");
 135 |         return 1;
 136 |     }
 137 |     else if(ch_flag.ch_ovp != 0)
 138 |     {
 139 |         ch_set_state(CH_OVP, "CH��ѹ");
 140 |         return 1;
 141 |     }
 142 |     else if(ch_flag.ch_ocp != 0)
 143 |     {
 144 |         ch_set_state(CH_OCP, "CH OCP");
 145 |         return 1;
 146 |     }
 147 | 
 148 |     return 0;
 149 | }
 150 | 
 151 | /**
 152 |   * @brief  ��ȡ CCCV �׶�ʵ��ʹ�õ�Ŀ���������λ��mA��
 153 |   * @param  target_current_ma  Э��ͱ����޷����Ŀ�������
 154 |   *
 155 |   * @note   ����������
 156 |   *         �� B1 �ϱ�����ߵ��ڵ�ѹ cell_max_mv >= 4.195V ʱ��
 157 |   *         ��Ϊ����Ѿ��ӽ����䣬��Ҫ�������ͳ�������
 158 |   *
 159 |   * @note   �����ٶȣ�
 160 |   *         ÿ 0.2 ����ཱུ�� 0.1A�������� 10ms �����������������ٽ�����
 161 |   *
 162 |   * @note   �������ޣ�
 163 |   *         ���ֻ���� iGED�����ή�� 0��
 164 |   *         iGED ������/ת�Ƶ������������ڸ�ֵ���岻��
 165 |   *
 166 |   * @note   �ָ����ԣ�
 167 |   *         �����ߵ��ڵ�ѹ���� 4.195V��������ֹֻͣ����������
 168 |   *         ��������������������ֵ��������������ɵ���������
 169 |   *
 170 |   * @retval ���� CCCV ʵ��ʹ�õ�Ŀ���������λ mA��
 171 |   */
 172 | static u16 ch_get_cccv_work_current_ma(u16 target_current_ma)
 173 | {
 174 |     /*
 175 |      * ���ν��� CCCV ʱ��s_cccv_curr_limit_ma Ϊ 0��
 176 |      * ��ʹ��Э��/�����޷����Ŀ�������
 177 |      *
 178 |      * ���Э��Ŀ�������С����ͬ�����͵�ǰ����ֵ��
 179 |      * ��ֹ��̬����ֵ�����µ�Э��Ŀ��ֵ��
 180 |      */
 181 |     if((s_cccv_curr_limit_ma == 0U) || (s_cccv_curr_limit_ma > target_current_ma))
 182 |     {
 183 |         s_cccv_curr_limit_ma = target_current_ma;
 184 |         s_cccv_derate_cnt = 0U;
 185 |     }
 186 | 
 187 |     /*
 188 |      * B1 �ϱ���ߵ��ڵ�ѹ�ﵽ 4.195V ��ʼ����������
 189 |      */
 190 |     if(uart_1_wire.cell_max_mv >= (4195U))  /* 4.195V */
 191 |     {
 192 |         /*
 193 |          * 10ms ����һ�Σ��ۼ� 20 ��Լ���� 0.2 �롣
 194 |          * δ��ʱ��ǰ��������
 195 |          */
 196 |         if(s_cccv_derate_cnt < 20)
 197 |         {
 198 |             s_cccv_derate_cnt++;
 199 |         }
 200 |         else
 201 |         {
 202 |             s_cccv_derate_cnt = 0U;
 203 | 
 204 |             /*
 205 |              * ÿ�ν��� 100mA��������Ƶ� iGED��
 206 |              */
 207 |             if(s_cccv_curr_limit_ma > (u16)(iGED + 500))
 208 |             {
 209 |                 s_cccv_curr_limit_ma -= 500;
 210 |             }
 211 |             else
 212 |             {
 213 |                 s_cccv_curr_limit_ma = iGED+100;
 214 |             }
 215 |         }
 216 |     }
 217 |     else
 218 |     {
 219 |         /*
 220 |          * ���� 4.195V ʱ��������������
 221 |          * Ҳ�����������������ѹ����ֵ��������ʱ�������ر仯��
 222 |          */
 223 |         s_cccv_derate_cnt = 0U;
 224 |     }
 225 | 
 226 |     return s_cccv_curr_limit_ma;
 227 | }
 228 | 
 229 | 
 230 | /**
 231 |   * @brief  ��ȡԤ�������ѹ����λ��mV��
 232 |   *
 233 |   * @note   ���ã�
 234 |   *         �����ж�Ԥ��׶�ʲôʱ�������ʲôʱ����Խ��� CCCV �׶Ρ�
 235 |   *
 236 |   * @note   ������Դ��
 237 |   *         1. ����ʹ�� A4 Э���·��ĵ���Ԥ���ֹ��ѹ cell_pre_mv��
 238 |   *         2. ���� A0 Э���·��Ĵ��� cell_series�����������Ԥ�������ѹ��
 239 |   *         3. ���Э�������쳣�������ʹ�ñ���Ĭ��Ԥ�������ѹ vPRE_37V5��
 240 |   *
 241 |   * @note   ����ԭ��
 242 |   *         1. �����쳣ʱ��ʹ�ñ���Ĭ�ϴ��� BAT_SERIES��
 243 |   *         2. ����Ԥ���ֹ��ѹ�쳣ʱ��ֱ�ӻ��� vPRE_37V51��
 244 |   *         3. ������������ѹ���� SET_vMAX ����ʱ��ֱ�ӻ��� vPRE_37V5��
 245 |   *         4. ������������ѹ���ܵ��ڱ������Ԥ����ֵ vPRE_30V��
 246 |   *
 247 |   * @retval ����Ԥ�������ѹ����λ mV��
 248 |   */
 249 | static u16 ch_get_pre_end_voltage_mv(void)
 250 | {
 251 |     u8 i;
 252 |     u8 series;          /* ��ش��������� A0 Э�� */
 253 |     u16 cell_pre_mv;    /* ����Ԥ���ֹ��ѹ����λ mV������ A4 Э�� */
 254 |     u16 pack_mv;        /* ����������Ԥ�������ѹ����λ mV */
 255 | 
 256 |     /*
 257 |      * ��ȡ��ش�����
 258 |      *
 259 |      * ��������£��������� A0 Э�顣
 260 |      * Ϊ��ֹЭ���쳣������������Ч��ΧΪ 5~20 ����
 261 |      *
 262 |      * ��� A0 ���صĴ���Ϊ 0����С�����
 263 |      * ��ʹ�ñ���Ĭ�ϴ��� BAT_SERIES��
 264 |      */
 265 |     series = uart_1_wire.cell_series;
 266 |     if((series < 5U) || (series > 20U))
 267 |     {
 268 |         series = BAT_SERIES;
 269 |     }
 270 | 
 271 |     /*
 272 |      * ��ȡ A4 Э������ĵ���Ԥ���ֹ��ѹ��
 273 |      *
 274 |      * cell_pre_mv ��λ��mV��
 275 |      *
 276 |      * ���磺
 277 |      *  2500 ��ʾ 2.500V/��
 278 |      *  3000 ��ʾ 3.000V/��
 279 |      *
 280 |      * ������Χ�����ڣ�
 281 |      *  CELL_REPAIR_MV 2V ~ CELL_FULL_MV 4.2V
 282 |      *
 283 |      * ��������޸���ѹ��˵��ֵ̫�ͣ�
 284 |      * ������ڵ��������ѹ��˵��ֵ̫�ߣ�
 285 |      * �����������ΪЭ�������쳣�����˵�����Ĭ�� vPRE_37V5��
 286 |      */
 287 |     cell_pre_mv = uart_1_wire.cell_pre_mv;
 288 |     if((cell_pre_mv < CELL_REPAIR_MV) || (cell_pre_mv > CELL_FULL_MV))
 289 |     {
 290 |         return vPRE_37V5;
 291 |     }
 292 | 
 293 |     /*
 294 |      * ���ݵ���Ԥ���ֹ��ѹ�ʹ�������������Ԥ�������ѹ��
 295 |      *
 296 |      * ��ֱ��ʹ�ã�
 297 |      *  pack_mv = cell_pre_mv * series;
 298 |      *
 299 |      * ��Ϊ�˱���˷�����쳣����Ҳ�������ۼӹ������ж��Ƿ񳬹� SET_vMAX��
 300 |      */
 301 |     pack_mv = 0U;
 302 |     for(i = 0U; i < series; i++)
 303 |     {
 304 |         /*
 305 |          * ��������ۼӻᳬ�� SET_vMAX��
 306 |          * ˵�� A4 ��ѹֵ�� A0 ���������쳣���ա�
 307 |          *
 308 |          * ��ʱ��ʹ��Э�����ֵ��ֱ�ӻ��˱���Ĭ�� vPRE_37V5��
 309 |          */
 310 |         if(pack_mv > (u16)(SET_vMAX - cell_pre_mv))
 311 |         {
 312 |             return vPRE_37V5;
 313 |         }
 314 | 
 315 |         pack_mv += cell_pre_mv;
 316 |     }
 317 | 
 318 |     /*
 319 |      * ����������Ԥ�������ѹ�����ܵ��ڱ������Ԥ����ֵ vPRE_30V��
 320 |      *
 321 |      * ������� vPRE��˵��Э�������Ԥ�������ƫ�ͣ�
 322 |      * ���ܵ��¹����˳�Ԥ��׶Σ���˻��˵�Ĭ�� vPRE_37V5��
 323 |      */
 324 |     if(pack_mv < vPRE_30V)
 325 |     {
 326 |         return vPRE_37V5;
 327 |     }
 328 | 
 329 |     return pack_mv;
 330 | }
 331 | 
 332 | /**
 333 |   * @brief  ��ȡ CCCV �׶�����ʱ�䣬��λ�����ӡ�
 334 |   *
 335 |   * @param  target_current_ma
 336 |   *         ��ǰ׼��ʹ�õ�Ŀ�����������λ mA��
 337 |   *
 338 |   * @note   �������ݣ�
 339 |   *         1. A0 Э���еõ���ز��� cell_parallel��
 340 |   *         2. A1 Э���еõ��������� cell_cap_01ah����λ 0.1Ah��
 341 |   *         3. ������ذ����� / ������ ����������ʱ�䡣
 342 |   *         4. ���������� 30 ����������
 343 |   *
 344 |   * @note   Э��δ��������ذ��ۺ�������������ֻ����
 345 |   *         �������� �� ����
 346 |   *         �������ذ�������
 347 |   *
 348 |   * @note   Ϊ��ֹ�쳣Э�����ݵ���ʱ�����
 349 |   *         1. ��������Ϊ 1~16���쳣ʱ�� 1 ��������
 350 |   *         2. ���������������Ϊ 80.0Ah��
 351 |   *         3. ��ذ��������������Ϊ 80.0Ah��
 352 |   *
 353 |   * @retval CCCV ����ʱ�䣬��λ�����ӡ�
 354 |   */
 355 | static u16 ch_get_cccv_timeout_min(u16 target_current_ma)
 356 | {
 357 |     u8 i;
 358 |     u8 parallel;          /* ��ز��������� A0 Э�� */
 359 |     u16 cell_cap_01ah;    /* ������������λ 0.1Ah������ A1 Э�� */
 360 |     u16 pack_cap_01ah;    /* �����������ĵ�ذ���������λ 0.1Ah */
 361 |     u16 current_100ma;    /* ����������Ϊ 100mA ��λ������ʹ�ø��� */
 362 |     u16 timeout_min;      /* ����õ��� CCCV ��ʱʱ�䣬��λ���� */
 363 | 
 364 |     /*
 365 |      * Ŀ�����̫С�����߻�û�л�ȡ����о����ʱ��
 366 |      * �����ж�̬���㣬ֱ��ʹ��Ĭ�� CCCV ʱ�䡣
 367 |      */
 368 |     if((target_current_ma < 100U) || (uart_1_wire.cell_cap_01ah == 0U))
 369 |     {
 370 |         return TIM_CCCV;
 371 |     }
 372 | 
 373 |     /*
 374 |      * ��ȡ A0 Э������Ĳ�����
 375 |      * ������Χ����Ϊ 1~16��
 376 |      * ���Э�������쳣������ 0 �򳬹� 16���� 1 ��������
 377 |      */
 378 |     parallel = uart_1_wire.cell_parallel;
 379 |     if((parallel == 0U) || (parallel > 16U))
 380 |     {
 381 |         parallel = 1U;
 382 |     }
 383 | 
 384 |     /*
 385 |      * ��ȡ A1 Э������ĵ���������
 386 |      * ��λ��0.1Ah��
 387 |      *
 388 |      * ���磺
 389 |      *  50  = 5.0Ah
 390 |      *  200 = 20.0Ah
 391 |      *  800 = 80.0Ah
 392 |      *
 393 |      * �����������Ϊ 800���� 80.0Ah��
 394 |      * ��ֹ�쳣Э��ֵ���º���ʱ��������
 395 |      */
 396 |     cell_cap_01ah = uart_1_wire.cell_cap_01ah;
 397 |     if(cell_cap_01ah > 800U)
 398 |     {
 399 |         cell_cap_01ah = 800U;
 400 |     }
 401 | 
 402 |     /*
 403 |      * ���ݲ����ۼӵõ���ذ�������
 404 |      *
 405 |      * ��ֱ��ʹ�ã�
 406 |      *  pack_cap_01ah = cell_cap_01ah * parallel;
 407 |      *
 408 |      * ��Ϊ�˱���˷��������Ҳ�������ۼӹ����������ޱ�����
 409 |      * ���� pack_cap_01ah Ҳ����Ϊ��� 800���� 80.0Ah��
 410 |      */
 411 |     pack_cap_01ah = 0U;
 412 |     for(i = 0U; i < parallel; i++)
 413 |     {
 414 |         /*
 415 |          * ��������ۼӻᳬ�� 800����ֱ��ǯλ�� 800��
 416 |          * �������Ա��������쳣�Ŵ󣬵��� CCCV ��ʱʱ���쳣�䳤��
 417 |          */
 418 |         if(pack_cap_01ah > (u16)(800U - cell_cap_01ah))
 419 |         {
 420 |             pack_cap_01ah = 800U;
 421 |             break;
 422 |         }
 423 | 
 424 |         pack_cap_01ah += cell_cap_01ah;
 425 |     }
 426 | 
 427 |     /*
 428 |      * ��Ŀ������� mA ����� 100mA ��λ��
 429 |      *
 430 |      * ���磺
 431 |      *  3000mA -> 30
 432 |      *  5000mA -> 50
 433 |      *
 434 |      * ��������������������㣺
 435 |      *  0.1Ah / 0.1A = 1 Сʱ
 436 |      */
 437 |     current_100ma = target_current_ma / 100U;
 438 |     if(current_100ma == 0U)
 439 |     {
 440 |         return TIM_CCCV;
 441 |     }
 442 | 
 443 |     /*
 444 |      * ��ʱʱ����㣺
 445 |      *
 446 |      * pack_cap_01ah ��λ�� 0.1Ah
 447 |      * current_100ma ��λ�� 0.1A
 448 |      *
 449 |      * pack_cap_01ah / current_100ma �õ�����Сʱ��
 450 |      *
 451 |      * ������ת����Ӧ���� 60��
 452 |      * ����ʹ�� 75���൱�������۳��ʱ������ϷŴ� 1.25 ����
 453 |      * ����ѹβ�Ρ������½���ͨ���������������������
 454 |      */
 455 |     timeout_min = (u16)((pack_cap_01ah * 75U) / current_100ma);
 456 | 
 457 |     /*
 458 |      * �ٶ������� 30 ���ӱ���������
 459 |      * ��ֹ���ֵ��β��ʱ��ϳ�ʱ��ǰ��ʱ��
 460 |      */
 461 |     timeout_min += 30U;
 462 | 
 463 |     /*
 464 |      * CCCV �ʱ�䲻�ܱ�Ԥ���ʱ�仹�̡�
 465 |      * ��������̫С��������ʹ�� TIM_PRE��
 466 |      */
 467 |     if(timeout_min < TIM_PRE)
 468 |     {
 469 |         timeout_min = TIM_PRE;
 470 |     }
 471 | 
 472 |     return timeout_min;
 473 | }
 474 | 
 475 | /**
 476 |   * @brief  �ر����г����������밲ȫ���״̬��
 477 |   */
 478 | static void ch_output_all_off(void)
 479 | {
 480 | 
 481 |     DCJK = 0;
 482 |     REPAIR_OUTPUT = 0;   // ��Ԥ��ر�
 483 |     VADJ = 0;
 484 |     FAN = 0;
 485 |     DUMMY_LOAD = 0;  // �رռٸ���
 486 |     set_Curr_Duty(PWMMAX/2);
 487 | }
 488 | 
 489 | /**
 490 |   * @brief  �ж�BMS�Ƿ����ڱ������¶����쳣��
 491 |   */
 492 | static bit ch_bms_temp_fault_active(void)
 493 | {
 494 |     if((u1w_info.charge_status & CH_BMS_TEMP_MASK) != 0U)
 495 |     {
 496 |         return 1;
 497 |     }
 498 | 
 499 |     return 0;
 500 | }
 501 | 
 502 | static bit ch_bms_status_check(void)
 503 | {
 504 |     if((u1w_info.charge_status & CH_BMS_ERR_MASK) != 0U)
 505 |     {
 506 |         ch_set_state(BMS_ERR, "BMS�쳣");
 507 |         return 1;
 508 |     }
 509 |     if(ch_bms_temp_fault_active() != 0)
 510 |     {
 511 |         ch_set_state(BMS_TEMP_ERR, "BMS�¶��쳣");
 512 |         return 1;
 513 |     }
 514 |     if((u1w_info.charge_status & U1W_B4_OV) != 0U)
 515 |     {
 516 |         ch_set_state(CH_FULL, "BMS����");
 517 |         return 1;
 518 |     }
 519 | 
 520 |     return 0;
 521 | }
 522 | 
 523 | /**
 524 |   * @brief  �жϵ�ǰ�Ƿ����ڼ̵����պϵ��������׶Ρ�
 525 |   * @note   ��Щ״̬�ε��ʱ�����õ�ѹ�жϣ���Ϊ val.vout �����ǳ�������������
 526 |   */
 527 | static bit ch_relay_charge_state(void)
 528 | {
 529 |     if((ch_state == CH_Pre1) || (ch_state == CH_CCCV))
 530 |     {
 531 |         return 1;
 532 |     }
 533 | 
 534 |     return 0;
 535 | }
 536 | 
 537 | /**
 538 |   * @brief  ����/�쳣��ֹ״̬�Ƿ���Ҫ��Ъ�򿪵�ѹ��⡣
 539 |   */
 540 | static bit ch_need_vout_probe(void)
 541 | {
 542 |     switch(ch_state)
 543 |     {
 544 |     case CH_FULL:
 545 |     case BMS_TEMP_ERR:
 546 |     case BMS_ERR:
 547 |     case CH_OTP:
 548 |     case CH_TimOut:
 549 |     case CCCV_TimOut:
 550 |     case CH_UVP:
 551 |     case CH_OVP:
 552 |     case CH_OCP:
 553 |     case NTC_ERR:
 554 |     case HW_ERR:
 555 |         return 1;
 556 | 
 557 |     default:
 558 |         return 0;
 559 |     }
 560 | }
 561 | 
 562 | static void ch_vout_probe_reset(void)
 563 | {
 564 |     s_vout_probe_period_10ms = CH_VOUT_PROBE_PERIOD_10MS;
 565 |     s_vout_probe_on_10ms = 0U;
 566 |     s_vout_sample_valid = 0;
 567 | }
 568 | 
 569 | /**
 570 |   * @brief  ÿ10ms׼��һ�ε�ص�ѹ����ʹ�ܡ�
 571 |   * @note   ����������״̬����������/�쳣״̬��Ъ�����������ѹ���ڷŵ硣
 572 |   */
 573 | static void ch_prepare_vout_sample_10ms(void)
 574 | {
 575 |     s_vout_sample_valid = 0;
 576 | 
 577 |     if((ch_state == CH_IDLE) ||
 578 |        (ch_state == BMS_HANDSHAKE) ||
 579 |        (ch_state == CH_Check) ||
 580 |        (ch_state == CH_REPAIR) ||
 581 |        (ch_state == CH_Pre1) ||
 582 |        (ch_state == CH_CCCV) ||
 583 |        (ch_state == CH_AGING))
 584 |     {
 585 |         BATT_DIVIDER_EN = 1;
 586 |         s_vout_sample_valid = 1;
 587 |         s_vout_probe_period_10ms = 0U;
 588 |         s_vout_probe_on_10ms = 0U;
 589 |         return;
 590 |     }
 591 | 
 592 |     if(ch_need_vout_probe() == 0)
 593 |     {
 594 |         BATT_DIVIDER_EN = 0;
 595 |         return;
 596 |     }
 597 | 
 598 |     if(s_vout_probe_on_10ms != 0U)
 599 |     {
 600 |         BATT_DIVIDER_EN = 1;
 601 | 
 602 |         if(s_vout_probe_on_10ms >= CH_VOUT_PROBE_VALID_10MS)
 603 |         {
 604 |             s_vout_sample_valid = 1;
 605 |         }
 606 | 
 607 |         s_vout_probe_on_10ms++;
 608 |         if(s_vout_probe_on_10ms > CH_VOUT_PROBE_ON_10MS)
 609 |         {
 610 |             s_vout_probe_on_10ms = 0U;
 611 |             s_vout_probe_period_10ms = 0U;
 612 |             BATT_DIVIDER_EN = 0;
 613 |             s_vout_sample_valid = 0;
 614 |         }
 615 |         return;
 616 |     }
 617 | 
 618 |     BATT_DIVIDER_EN = 0;
 619 |     if(s_vout_probe_period_10ms < CH_VOUT_PROBE_PERIOD_10MS)
 620 |     {
 621 |         s_vout_probe_period_10ms++;
 622 |     }
 623 |     else
 624 |     {
 625 |         s_vout_probe_on_10ms = 1U;
 626 |         BATT_DIVIDER_EN = 1;
 627 |     }
 628 | }
 629 | 
 630 | /**
 631 |   * @brief  �������⣬ȫ���жϵ���Ƿ�γ���
 632 |   * @return 1 ��ʾ�������ɰγ����ӹܡ�
 633 |   */
 634 | static bit ch_battery_removed_check_10ms(void)
 635 | {
 636 |     u8 need_cnt;
 637 | 
 638 |     if(ch_state == CH_IDLE)
 639 |     {
 640 |         s_remove_cnt = 0U;
 641 |         return 0;
 642 |     }
 643 | 
 644 |     if(ch_relay_charge_state() != 0)
 645 |     {
 646 |         /*
 647 |          * �̵����պ�ʱ��val.vout �����ǳ�������������������Ϊ�ε�����ݡ�
 648 |          * 1���޺Ϸ�ͨ��֡�ҵ���С������������ȹ������Լ20ms���ٿ���ض˵�ѹ��
 649 |          */
 650 |         if(s_remove_cnt != 0U)
 651 |         {
 652 |             ch_output_all_off();
 653 |             if(++s_remove_cnt >= 3U)
 654 |             {
 655 |                 if(val.vout < vRESET)
 656 |                 {
 657 |                     BATT_DIVIDER_EN = 0;
 658 |                     uart_1_wire_reset_link();
 659 |                     ch_vout_probe_reset();
 660 |                     ch_set_state(CH_IDLE, "�ε��");
 661 |                     return 1;
 662 |                 }
 663 |                 s_remove_cnt = 0U;
 664 |                 ch_set_state(BMS_ERR, "�γ��쳣");
 665 |                 return 1;
 666 |             }
 667 |             return 1;
 668 |         }
 669 | 
 670 |         if((val.curr < iGED) && (u1w_info.no_rx_10ms >= 100U))
 671 |         {
 672 |             ch_output_all_off();
 673 |             s_remove_cnt = 1U;
 674 |             return 1;
 675 |         }
 676 |         return 0;
 677 |     }
 678 | 
 679 |     if((s_vout_sample_valid != 0) && (val.vout < vRESET))
 680 |     {
 681 |         if(ch_need_vout_probe() != 0)
 682 |         {
 683 |             need_cnt = 2U;      /* ��Ъ��ⴰ���£�����2����Ч����ȷ�ϰγ��� */
 684 |         }
 685 |         else
 686 |         {
 687 |             need_cnt = 50U;     /* ��������£�Լ500msȷ�ϰγ��� */
 688 |         }
 689 | 
 690 |         if(++s_remove_cnt >= need_cnt)
 691 |         {
 692 |             ch_output_all_off();
 693 |             BATT_DIVIDER_EN = 0;
 694 |             uart_1_wire_reset_link();
 695 |             ch_vout_probe_reset();
 696 |             ch_set_state(CH_IDLE, "�ε��");
 697 |             return 1;
 698 |         }
 699 |     }
 700 |     else if(s_vout_sample_valid != 0)
 701 |     {
 702 |         s_remove_cnt = 0U;
 703 |     }
 704 | 
 705 |     return 0;
 706 | }
 707 | 
 708 | 
 709 | 
 710 | 
 711 | void usr_ch_func(void)
 712 | {
 713 |     u16 target_voltage_mv;
 714 |     u16 target_current_ma;
 715 |     u16 pre_end_voltage_mv;
 716 |     u16 cccv_timeout_min;
 717 |     u8 u1w_stage;
 718 | 
 719 |     ch_state = CH_IDLE;
 720 |     last_state = CH_IDLE;
 721 | 
 722 |     next_10ms = timer_deadline_ms(TASK_10MS);
 723 |     uart_1_wire_reset_link();
 724 |     uart_printf("������\n");
 725 | 
 726 |     while(flg_cal_mode == 0)
 727 |     {
 728 |         if(timer_period_elapsed(&next_10ms, TASK_10MS) != 0)
 729 |         {
 730 |             wdt_feed();
 731 | 
 732 |             /* DEBUG ���յ� *RST ������У׼���̣����� 54.6V ��Ϊ�� */
 733 |             if(pc_uart_func() == 1U)
 734 |             {
 735 |                 flg_cal_mode = 1;
 736 |                 break;
 737 |             }
 738 | 
 739 |             /*
 740 |              * һ��ͨ�Ž׶��ɳ������ֱ�Ӿ�����
 741 |              * - �������ͷ� COM����ͨ�ţ�
 742 |              * - BMS_HANDSHAKE����ѯ����֡��
 743 |              * - ������磺��ѯ B1/B3/B4/B6��
 744 |              * - ���磺��Э�鷢�� B6 03 SOC��3 ���Ӻ��������� COM��
 745 |              * - BMS�¶��쳣��������ѯ B3/B4 �ȴ��ָ���
 746 |              * - ��ͨ�쳣���������� COM����֪ BMS �����Ͽ���
 747 |              */
 748 |             switch(ch_state)
 749 |             {
 750 |             case BMS_HANDSHAKE:
 751 |                 u1w_stage = U1W_STAGE_HANDSHAKE;
 752 |                 break;
 753 | 
 754 |             case CH_Check:
 755 |             case CH_REPAIR:
 756 |             case CH_Pre1:
 757 |             case CH_CCCV:
 758 |                 u1w_stage = U1W_STAGE_CHARGE;
 759 |                 break;
 760 | 
 761 |             case CH_FULL:
 762 |                 u1w_stage = U1W_STAGE_FULL_DISPLAY;
 763 |                 break;
 764 | 
 765 |             case BMS_TEMP_ERR:
 766 |                 u1w_stage = U1W_STAGE_TEMP_WAIT;
 767 |                 break;
 768 | 
 769 |             case BMS_ERR:
 770 |             case CH_OTP:
 771 |             case CH_TimOut:
 772 |             case CCCV_TimOut:
 773 |             case CH_UVP:
 774 |             case CH_OVP:
 775 |             case CH_OCP:
 776 |             case NTC_ERR:
 777 |             case HW_ERR:
 778 |                 u1w_stage = U1W_STAGE_PULL_LOW;
 779 |                 break;
 780 | 
 781 |             default:
 782 |                 u1w_stage = U1W_STAGE_STOP;
 783 |                 break;
 784 |             }
 785 |             uart_1_wire_set_stage(u1w_stage);
 786 |             uart_1_wire_poll_10ms();
 787 | 
 788 |             /* �Ȱ���ǰ״̬׼����ѹ����ʹ�ܣ��ٸ���ADC�������� */
 789 |             ch_prepare_vout_sample_10ms();
 790 | 
 791 |             /* ÿ 10ms ���� ADC �������ͱ�����־�� */
 792 |             adc_sample_all();
 793 |             ch_err_ck();
 794 | 
 795 |             /* ����������������ȼ�����BMS������ */
 796 |             if(ch_check_protect_state() != 0)
 797 |             {
 798 |                 ch_output_all_off();
 799 |             }
 800 | 
 801 |             if(ch_battery_removed_check_10ms() != 0)
 802 |             {
 803 |                 continue;
 804 |             }
 805 | 
 806 |             target_voltage_mv = u1w_info.target_voltage_mv;
 807 |             if(target_voltage_mv > SET_vMAX)
 808 |             {
 809 |                 target_voltage_mv = SET_vMAX;
 810 |             }
 811 | 
 812 |             target_current_ma = u1w_info.target_current_ma;
 813 |             if(target_current_ma > iMAX)
 814 |             {
 815 |                 target_current_ma = iMAX;
 816 |             }
 817 |             if(last_state != ch_state)
 818 |             {
 819 |                 last_state = ch_state;
 820 |                 s_cut[0] = 0;
 821 |                 s_cut[1] = 0;
 822 |                 s_cut[2] = 0;
 823 |                 s_cut[3] = 0;
 824 |                 s_remove_cnt = 0U;
 825 |                 ch_vout_probe_reset();
 826 |                 Tim.ms = 0;
 827 |                 Tim.s = 0;
 828 |                 Tim.min = 0;
 829 | 
 830 |                 if(ch_state == CH_CCCV)
 831 |                 {
 832 |                     s_cccv_curr_limit_ma = 0U;
 833 |                     s_cccv_derate_cnt = 0U;
 834 |                 }
 835 |             }
 836 | 
 837 | //            ch_state = CH_AGING;
 838 | 
 839 |             switch(ch_state)
 840 |             {
 841 |             default:
 842 |             case CH_IDLE:
 843 |                 /*
 844 |                  * ����/������
 845 |                  * - �̵�����VADJ��PWM������ȫ���رգ�
 846 |                  * - ��ѹ���������ֵ�����BMS���֣�
 847 |                  * - 1V~15V ���ȶ�ȷ�ϣ������ѹ�����˲̬���е�ѹ��
 848 |                  */
 849 |                 ch_output_all_off();
 850 |                 RLED = 1;
 851 |                 GLED = 0;
 852 |                 BATT_DIVIDER_EN = 1;          /* �򿪵�ط�ѹ����֤ AIN3 ������Ч�� */
 853 |                 if(val.vout >= vSTART)
 854 |                 {
 855 |                     s_cut[1] = 0U;
 856 |                     s_idle_low_last_mv = 0U;
 857 |                     if(++s_cut[0] >= 50U)
 858 |                     {
 859 |                         uart_1_wire_reset_link();
 860 |                         ch_set_state(BMS_HANDSHAKE, "����");
 861 |                     }
 862 |                 }
 863 |                 else if(val.vout > vRESET)
 864 |                 {
 865 |                     s_cut[0] = 0U;
 866 |                     if(s_cut[1] == 0U)
 867 |                     {
 868 |                         s_idle_low_last_mv = val.vout;
 869 |                         s_cut[1] = 1U;
 870 |                     }
 871 |                     else
 872 |                     {
 873 |                         if(((val.vout > s_idle_low_last_mv) &&
 874 |                             ((u16)(val.vout - s_idle_low_last_mv) > 100U)) ||
 875 |                            ((s_idle_low_last_mv > val.vout) &&
 876 |                             ((u16)(s_idle_low_last_mv - val.vout) > 100U)))
 877 |                         {
 878 |                             s_cut[1] = 1U;
 879 |                         }
 880 |                         else if(s_cut[1] < 200U)
 881 |                         {
 882 |                             s_cut[1]++;
 883 |                         }
 884 | 
 885 |                         s_idle_low_last_mv = val.vout;
 886 |                         if(s_cut[1] >= 200U)
 887 |                         {
 888 |                             ch_set_state(CH_UVP, "��ص�ѹ");
 889 |                         }
 890 |                     }
 891 |                 }
 892 |                 else
 893 |                 {
 894 |                     s_cut[0] = 0U;
 895 |                     s_cut[1] = 0U;
 896 |                     s_idle_low_last_mv = 0U;
 897 |                 }
 898 |                 break;
 899 | 
 900 |             case BMS_HANDSHAKE:
 901 |                 /*
 902 |                  * �ȴ����ֳɹ���
 903 |                  * - ������ֹرգ�
 904 |                  * - �ȴ�Э������ A0/A1/A4/A6/A7/B1/B3/B4��
 905 |                  * - ͨ�ų�ʱ���� BMS_ERR��
 906 |                  * - ���ֳɹ���Ž��� CH_Check���� CH_Check �����޸�/Ԥ��/CCCV��
 907 |                  */
 908 |                 ch_output_all_off();
 909 |                 RLED = 1;
 910 |                 GLED = 0;
 911 | 
 912 |                 TimCut();
 913 |                 if(Tim.s >= BMS_HANDSHAKE_TIMEOUT_S)
 914 |                 {
 915 |                     ch_set_state(BMS_ERR, "���ֳ�ʱ");
 916 |                 }
 917 |                 else if(val.vout < vSTART)
 918 |                 {
 919 |                     if(++s_cut[0] >= 50U)
 920 |                     {
 921 |                         ch_set_state(CH_UVP, "���ֵ�ѹ");
 922 |                     }
 923 |                 }
 924 |                 else
 925 |                 {
 926 |                     s_cut[0] = 0U;
 927 | 
 928 |                     if(u1w_info.comm_timeout != 0U)
 929 |                     {
 930 |                         ch_set_state(BMS_ERR, "BMSͨ�ų�ʱ");
 931 |                     }
 932 |                     else if(u1w_info.handshake_ok != 0U)
 933 |                     {
 934 |                         pc_uart_print_batt();
 935 |                         ch_set_state(CH_Check, "���ֳɹ�");
 936 |                     }
 937 |                     else
 938 |                     {
 939 |                         /* �����ȴ� A0/A1/A4/A6/A7/B1/B3/B4 ���롣 */
 940 |                     }
 941 |                 }
 942 |                 break;
 943 | 
 944 |             case CH_Check:
 945 |                 /*
 946 |                  * ���״̬��
 947 |                  * - ��״̬Ĭ���Ѿ����BMS���֣�
 948 |                  * - ������ֹرգ�
 949 |                  * - ��ͨ�ű���λ����δ���ߣ����˻�BMS_HANDSHAKE�ȴ���
 950 |                  * - ͨ���������ٸ��ݵ�ѹ�����޸���Ԥ���CCCV��
 951 |                  */
 952 |                 ch_output_all_off();
 953 |                 RLED = 1;
 954 |                 GLED = 0;
 955 | 
 956 |                 if(val.vout < vSTART)
 957 |                 {
 958 |                     if(++s_cut[0] >= 50U)
 959 |                     {
 960 |                         ch_set_state(CH_UVP, "��ص�ѹ");
 961 |                     }
 962 |                 }
 963 |                 else
 964 |                 {
 965 |                     /*
 966 |                      * ��ѹ�ѻָ��������ֵ���ϣ����Ƿѹȷ�ϼ�����
 967 |                      * ����ǰ���ٽ粨���ۼƵ�s_cut[0]�����������ٴβ���ʱ���Ƿѹ��
 968 |                      */
 969 |                     s_cut[0] = 0U;
 970 |                     pre_end_voltage_mv = ch_get_pre_end_voltage_mv();
 971 | 
 972 |                     if(u1w_info.comm_timeout != 0U)
 973 |                     {
 974 |                         ch_set_state(BMS_ERR, "BMSͨ�ų�ʱ");
 975 |                     }
 976 |                     else if(ch_bms_status_check() != 0)
 977 |                     {
 978 |                         /* ״̬���л��� */
 979 |                     }
 980 |                     else if(u1w_info.handshake_ok == 0U)
 981 |                     {
 982 |                         uart_1_wire_reset_link();
 983 |                         ch_set_state(BMS_HANDSHAKE, "������");
 984 |                     }
 985 |                     else if(val.vout < vPRE_30V)
 986 |                     {
 987 |                         ch_set_state(CH_REPAIR, "ͨ��OK���޸�");
 988 |                     }
 989 |                     else if(val.vout < pre_end_voltage_mv)
 990 |                     {
 991 |                         ch_set_state(CH_Pre1, "ͨ��OK��Ԥ��");
 992 |                     }
 993 |                     else
 994 |                     {
 995 |                         ch_set_state(CH_CCCV, "ͨ��OK��CCCV");
 996 |                     }
 997 |                 }
 998 |                 break;
 999 | 
1000 |             case CH_REPAIR:
1001 |                 /*
1002 |                  * ����ѹ�޸���
1003 |                  * - �̵����Ͽ���ֻ���޸������
1004 |                  * - ����ʹ�� iREPAIR������С�����޸���
1005 |                  * - �� vPRE_30V ��ת��Ԥ�䣬��ʱ����Ԥ�䳬ʱ�쳣��
1006 |                  */
1007 |                 if(u1w_info.comm_timeout != 0U)
1008 |                 {
1009 |                     ch_set_state(BMS_ERR, "BMSͨ�ų�ʱ");
1010 |                     break;
1011 |                 }
1012 |                 if(ch_bms_status_check() != 0)
1013 |                 {
1014 |                     break;
1015 |                 }
1016 |                 DCJK = 0;
1017 |                 VADJ = 0;
1018 |                 if(DCJK == 0)
1019 |                 {
1020 |                     REPAIR_OUTPUT = 1;
1021 |                 }
1022 |                 FAN = 1;
1023 |                 Ged_Flash(50);
1024 |                 TimCut();
1025 |                 set_Curr_Duty(SET_CURR(iREPAIR));
1026 | 
1027 |                 if(Tim.min >= TIM_PRE)
1028 |                 {
1029 |                     ch_set_state(CH_TimOut, "�޸���ʱ");
1030 |                 }
1031 |                 else if(val.vout >= vPRE_30V)
1032 |                 {
1033 |                     if(++s_cut[0] >= 50U)
1034 |                     {
1035 |                         ch_set_state(CH_Pre1, "�޸����");
1036 |                     }
1037 |                 }
1038 |                 else
1039 |                 {
1040 |                     s_cut[0] = 0;
1041 |                 }
1042 |                 break;
1043 | 
1044 |             case CH_Pre1:
1045 |                 /*
1046 |                  * Ԥ�䣺
1047 |                  * - ����Ԥ��ʱ�������ͨ�ų�ʱ��
1048 |                  * - B4 ״̬λ�����硢�¶��쳣����ͨ�쳣���ദ����
1049 |                  */
1050 |                 if(u1w_info.comm_timeout != 0U)
1051 |                 {
1052 |                     ch_set_state(BMS_ERR, "BMSͨ�ų�ʱ");
1053 |                     break;
1054 |                 }
1055 |                 if(ch_bms_status_check() != 0)
1056 |                 {
1057 |                     break;
1058 |                 }
1059 | 
1060 |                 /*
1061 |                  * �첽ͨ�Ű�ֻ�����ﴦ��ͨ�ų�ʱ��
1062 |                  * B4 ״̬λ�����硢�¶��쳣����ͨ�쳣���ദ����
1063 |                  */
1064 |                 DCJK = 1;
1065 |                 REPAIR_OUTPUT = 0;
1066 |                 VADJ = 1;
1067 |                 FAN = 1;
1068 |                 Ged_Flash(50);
1069 |                 TimCut();
1070 |                 set_Curr_Duty(SET_CURR(iPRE));
1071 |                 pre_end_voltage_mv = ch_get_pre_end_voltage_mv();
1072 | 
1073 |                 if(Tim.min >= TIM_PRE)
1074 |                 {
1075 |                     ch_set_state(CH_TimOut, "Ԥ�䳬ʱ");
1076 |                 }
1077 |                 else if(val.vout >= pre_end_voltage_mv)
1078 |                 {
1079 |                     if(++s_cut[0] >= 50U)
1080 |                     {
1081 |                         ch_set_state(CH_CCCV, "Ԥ�����");
1082 |                     }
1083 |                 }
1084 |                 else
1085 |                 {
1086 |                     s_cut[0] = 0;
1087 |                 }
1088 |                 break;
1089 | 
1090 |             case CH_CCCV:
1091 |                 /*
1092 |                  * ������ѹ��
1093 |                  * - ��Э��Ŀ���ѹ/��������PWM��
1094 |                  * - �����ж�ʹ��Э��Ŀ���ѹ�������ǹ̶�SET_vMAX��
1095 |                  * - ����г������ͨ�ų�ʱ��B4״̬��
1096 |                  */
1097 |                 if(u1w_info.comm_timeout != 0U)
1098 |                 {
1099 |                     ch_set_state(BMS_ERR, "BMSͨ�ų�ʱ");
1100 |                     break;
1101 |                 }
1102 |                 if(ch_bms_status_check() != 0)
1103 |                 {
1104 |                     break;
1105 |                 }
1106 | 
1107 |                 /*
1108 |                  * �첽ͨ�Ű�ֻ�����ﴦ��ͨ�ų�ʱ��
1109 |                  * B4 ״̬λ�����硢�¶��쳣����ͨ�쳣���ദ����
1110 |                  */
1111 |                 DCJK = 1;
1112 |                 REPAIR_OUTPUT = 0;
1113 |                 VADJ = 1;
1114 |                 FAN = 1;
1115 |                 RLED = 0;
1116 |                 Ged_Flash(50);
1117 |                 TimCut();
1118 | 
1119 |                 /*
1120 |                  * ��ߵ��ڵ�ѹ�ﵽ 4.195V ��CCCV ����ÿ 0.2 ����ཱུ�� 0.1A��
1121 |                  * ������ֵֻ�ڱ��� CCCV �׶��ڱ��֣����½��� CCCV ������³�ʼ����
1122 |                  */
1123 |                 target_current_ma = ch_get_cccv_work_current_ma(target_current_ma);
1124 |                 set_Curr_Duty(SET_CURR(target_current_ma));
1125 |                 cccv_timeout_min = ch_get_cccv_timeout_min(target_current_ma);
1126 | 
1127 |                 if(Tim.min >= cccv_timeout_min)
1128 |                 {
1129 |                     ch_set_state(CCCV_TimOut, "CCCV��ʱ");
1130 |                 }
1131 |                 else if((val.vout >= target_voltage_mv) && (val.curr <= iGED))
1132 |                 {
1133 |                     if(++s_cut[0] >= 100U)
1134 |                     {
1135 |                         ch_set_state(CH_FULL, "ת����");
1136 |                     }
1137 |                 }
1138 |                 else
1139 |                 {
1140 |                     s_cut[0] = 0;
1141 |                 }
1142 |                 break;
1143 | 
1144 |             case CH_FULL:
1145 |                 /*
1146 |                  * ���磺
1147 |                  * - ֹͣ����������̵ƣ�
1148 |                  * - ��Э�鷢�� B6 03 SOC��3���Ӻ���ͨ�Ų���������COM��
1149 |                  * - FULL_DISPLAY ������ѯB1���س��������� vCH60 �����жϡ�
1150 |                  */
1151 |                 ch_output_all_off();
1152 |                 RLED = 0;
1153 |                 GLED = 1;
1154 |                 if((s_vout_sample_valid != 0) && (val.vout < vCH60))
1155 |                 {
1156 |                     if(++s_cut[0] >= 2U)
1157 |                     {
1158 |                         uart_1_wire_reset_link();
1159 |                         ch_set_state(BMS_HANDSHAKE, "���������������");
1160 |                     }
1161 |                 }
1162 |                 else if(s_vout_sample_valid != 0)
1163 |                 {
1164 |                     s_cut[0] = 0;
1165 |                 }
1166 |                 break;
1167 | 
1168 |             case BMS_TEMP_ERR:
1169 |                 /*
1170 |                  * BMS�¶��쳣��
1171 |                  * - ����رգ�
1172 |                  * - ����������COM��
1173 |                  * - ������ѯB3/B4����BMS�¶�״̬�ָ����������֡�
1174 |                  */
1175 |                 ch_output_all_off();
1176 |                 RGed_Flash(50);
1177 |                 if(u1w_info.comm_timeout != 0U)
1178 |                 {
1179 |                     ch_set_state(BMS_ERR, "�¶�ͨ�ų�ʱ");
1180 |                 }
1181 |                 else if((u1w_info.charge_status & CH_BMS_ERR_MASK) != 0U)
1182 |                 {
1183 |                     ch_set_state(BMS_ERR, "BMS�쳣");
1184 |                 }
1185 |                 else if(ch_bms_temp_fault_active() == 0)
1186 |                 {
1187 |                     uart_1_wire_reset_link();
1188 |                     ch_set_state(BMS_HANDSHAKE, "BMS�¶Ȼָ�");
1189 |                 }
1190 |                 break;
1191 | 
1192 |             case BMS_ERR:
1193 |                 /*
1194 |                  * BMSͨ���쳣��
1195 |                  * - ����رգ�
1196 |                  * - ��������COM����֪BMS�����Ͽ���
1197 |                  * - �ε����ȫ�ְγ����ͳһ������
1198 |                  */
1199 |                 ch_output_all_off();
1200 |                 Red_Flash(50);
1201 |                 break;
1202 | 
1203 |             case CH_OTP:
1204 |                 /*
1205 |                  * ���±������¶Ȼָ���ؼ��״̬��
1206 |                  */
1207 |                 ch_output_all_off();
1208 |                 RGed_Flash(50);
1209 |                 if(ch_flag.ch_hotErr == 0)
1210 |                 {
1211 |                     uart_1_wire_reset_link();
1212 |                     ch_set_state(BMS_HANDSHAKE, "OTP�ָ���������");
1213 |                 }
1214 |                 break;
1215 | 
1216 |             case CH_TimOut:
1217 |             case CCCV_TimOut:
1218 |             case CH_UVP:
1219 |             case CH_OVP:
1220 |             case CH_OCP:
1221 |                 /*
1222 |                  * ��ѹ/����/��ʱ�ౣ��������رգ��ȵ�ذγ���ָ����ء�
1223 |                  */
1224 |                 ch_output_all_off();
1225 |                 Red_Flash(50);
1226 |                 break;
1227 | 
1228 |             case NTC_ERR:
1229 |             case HW_ERR:
1230 |                 /*
1231 |                  * NTC/Ӳ���쳣������رգ��ȴ���ѹ�ͷŻ��˹�������
1232 |                  */
1233 |                 ch_output_all_off();
1234 |                 RGed_Flash(50);
1235 |                 break;
1236 | 
1237 |             case CH_AGING:
1238 |                 /*
1239 |                  * �ϻ�ģʽԤ����
1240 |                  */
1241 |                 DCJK = 1;
1242 |                 VADJ = 1;
1243 |                 FAN = 1;
1244 |                 set_Curr_Duty(SET_CURR(iMAX));
1245 |                 Ged_Flash(50);
1246 |                 break;
1247 |             }
1248 |         }
1249 |     }
1250 | }
1251 | 
```

### YAT-60V3A-Uart-1-wire/App/main.c

Bytes: 10275
SHA-256: 2ccdb1158f2c3b57fc747d7f5be3b4d012adcaf31c12f946ae5070a7c2250ef2
Lines: 1-477 of 477

```text
  1 | /**
  2 |   ******************************************************************************
  3 |   * @file    main.c
  4 |   * @brief   60V3A �����Ӧ����ڣ����� 54.6V ��Ŀ��ѭ�����
  5 |   ******************************************************************************
  6 |   */
  7 | #include "bsp_init.h"
  8 | #include "ch.h"
  9 | #include "cal.h"
 10 | #include "uart_1_wire.h"
 11 | #include "common.h"
 12 | #include "timer.h"
 13 | #include "fifo.h"
 14 | #include "uart.h"
 15 | 
 16 | bit flg_cal_mode = 0;     /* 0: �������  1: DEBUG У׼���� */
 17 | data u16 next_10ms;
 18 | 
 19 | /*
 20 |  * һ��ͨ���ֶ�����������ڡ�
 21 |  * ��ǰ����Ӳ��ͨ��δ��ͨ�׶Σ�
 22 |  * 1. ���ܳ�����̣�
 23 |  * 2. ����ͨ�ų�ʱ��
 24 |  * 3. ���Զ�����һ�����
 25 |  * 4. ͨ�� DEBUG ���ڰ����ֶ�����ÿһ��һ�����
 26 |  * 5. COM �յ��κ��ֽڶ�ԭʼ��ӡ�����㿴Ӳ�����κͻذ���
 27 |  */
 28 | #define MAIN_1WIRE_TEST_EN              (0U)
 29 | #define MAIN_1WIRE_TEST_CMD_NUM         (9U)
 30 | #define MAIN_1WIRE_INFO_PERIOD_10MS     (100U)
 31 | 
 32 | #if (MAIN_1WIRE_TEST_EN != 0U)
 33 | static u8 s_1w_test_idx;
 34 | static u8 s_flow_run;
 35 | static u8 s_flow_charge;
 36 | static u8 s_flow_timeout_printed;
 37 | static u16 s_flow_info_tick_10ms;
 38 | 
 39 | static void test_put_char(u8 dat)
 40 | {
 41 |     ch_uart_send_byte(dat);
 42 | }
 43 | 
 44 | static void test_put_str(char *str)
 45 | {
 46 |     while(*str != 0)
 47 |     {
 48 |         test_put_char((u8)(*str));
 49 |         str++;
 50 |     }
 51 | }
 52 | 
 53 | static void test_put_crlf(void)
 54 | {
 55 |     test_put_char(0x0DU);
 56 |     test_put_char(0x0AU);
 57 | }
 58 | 
 59 | static void test_put_hex4(u8 dat)
 60 | {
 61 |     dat &= 0x0FU;
 62 |     if(dat < 10U)
 63 |     {
 64 |         test_put_char((u8)('0' + dat));
 65 |     }
 66 |     else
 67 |     {
 68 |         test_put_char((u8)('A' + dat - 10U));
 69 |     }
 70 | }
 71 | 
 72 | static void test_put_hex8(u8 dat)
 73 | {
 74 |     test_put_hex4((u8)(dat >> 4));
 75 |     test_put_hex4(dat);
 76 | }
 77 | 
 78 | static void test_put_bytes(u8 *buf, u8 len)
 79 | {
 80 |     u8 i;
 81 | 
 82 |     for(i = 0U; i < len; i++)
 83 |     {
 84 |         test_put_hex8(buf[i]);
 85 |         test_put_char(' ');
 86 |     }
 87 | }
 88 | 
 89 | static u8 test_sum(u8 *buf, u8 len)
 90 | {
 91 |     u8 i;
 92 |     u8 sum;
 93 | 
 94 |     sum = 0U;
 95 |     for(i = 0U; i < len; i++)
 96 |     {
 97 |         sum = (u8)(sum + buf[i]);
 98 |     }
 99 | 
100 |     return sum;
101 | }
102 | 
103 | static u8 test_get_cmd_by_idx(u8 idx)
104 | {
105 |     switch(idx)
106 |     {
107 |     case 0U: return U1W_CMD_A0;
108 |     case 1U: return U1W_CMD_A1;
109 |     case 2U: return U1W_CMD_A4;
110 |     case 3U: return U1W_CMD_A6;
111 |     case 4U: return U1W_CMD_A7;
112 |     case 5U: return U1W_CMD_B1;
113 |     case 6U: return U1W_CMD_B3;
114 |     case 7U: return U1W_CMD_B4;
115 |     default: return U1W_CMD_B6;
116 |     }
117 | }
118 | 
119 | static void test_print_current_cmd(void)
120 | {
121 |     test_put_str("[1W] ��ǰ���=");
122 |     test_put_char((u8)('0' + s_1w_test_idx));
123 |     test_put_str(" ����=0x");
124 |     test_put_hex8(test_get_cmd_by_idx(s_1w_test_idx));
125 |     test_put_crlf();
126 | }
127 | 
128 | static void test_print_help(void)
129 | {
130 |     test_put_str("[1W] �ֶ���������");
131 |     test_put_crlf();
132 |     test_put_str("[1W] ������s���� r�ط� n��һ�� p��һ�� 0~8ѡ�� g���� xֹͣ ?����");
133 |     test_put_crlf();
134 |     test_put_str("[1W] ���0=A0 1=A1 2=A4 3=A6 4=A7 5=B1 6=B3 7=B4 8=B6");
135 |     test_put_crlf();
136 |     test_put_str("[1W] �޳�ʱ�����Զ���һ��");
137 |     test_put_crlf();
138 |     test_print_current_cmd();
139 | }
140 | 
141 | static void test_make_frame(u8 cmd, u8 *buf, u8 *len)
142 | {
143 |     buf[0] = cmd;
144 | 
145 |     if(cmd == U1W_CMD_A0)
146 |     {
147 |         buf[1] = 0x00U;
148 |         buf[2] = U1W_MASTER;
149 |         buf[3] = test_sum(buf, 3U);
150 |         *len = 4U;
151 |     }
152 |     else if(cmd == U1W_CMD_B6)
153 |     {
154 |         buf[1] = 0x01U;
155 |         buf[2] = 0x01U;
156 |         buf[3] = test_sum(buf, 3U);
157 |         *len = 4U;
158 |     }
159 |     else
160 |     {
161 |         buf[1] = cmd;
162 |         *len = 2U;
163 |     }
164 | }
165 | 
166 | static void test_send_current_cmd(void)
167 | {
168 |     u8 cmd;
169 |     u8 tx_len;
170 |     u8 tx_buf[4];
171 | 
172 |     cmd = test_get_cmd_by_idx(s_1w_test_idx);
173 |     test_make_frame(cmd, tx_buf, &tx_len);
174 | 
175 |     test_put_str("[1W] �������=");
176 |     test_put_char((u8)('0' + s_1w_test_idx));
177 |     test_put_str(" ����=0x");
178 |     test_put_hex8(cmd);
179 |     test_put_str(" �ֽ�=");
180 |     test_put_bytes(tx_buf, tx_len);
181 |     test_put_crlf();
182 | 
183 |     if(com_uart_send_buf(tx_buf, tx_len) != BSP_OK)
184 |     {
185 |         test_put_str("[1W] ����ʧ��");
186 |         test_put_crlf();
187 |     }
188 | 
189 |     com_uart_set_rx_mode();
190 | }
191 | 
192 | static void test_next_cmd(void)
193 | {
194 |     s_1w_test_idx++;
195 |     if(s_1w_test_idx >= MAIN_1WIRE_TEST_CMD_NUM)
196 |     {
197 |         s_1w_test_idx = 0U;
198 |     }
199 | 
200 |     test_send_current_cmd();
201 | }
202 | 
203 | static void test_prev_cmd(void)
204 | {
205 |     if(s_1w_test_idx == 0U)
206 |     {
207 |         s_1w_test_idx = MAIN_1WIRE_TEST_CMD_NUM - 1U;
208 |     }
209 |     else
210 |     {
211 |         s_1w_test_idx--;
212 |     }
213 | 
214 |     test_send_current_cmd();
215 | }
216 | 
217 | 
218 | 
219 | static void test_put_u16(u16 dat)
220 | {
221 |     char buf[5];
222 |     u8 i;
223 |     u8 started;
224 | 
225 |     buf[0] = (char)('0' + (dat / 10000U));
226 |     dat %= 10000U;
227 |     buf[1] = (char)('0' + (dat / 1000U));
228 |     dat %= 1000U;
229 |     buf[2] = (char)('0' + (dat / 100U));
230 |     dat %= 100U;
231 |     buf[3] = (char)('0' + (dat / 10U));
232 |     buf[4] = (char)('0' + (dat % 10U));
233 | 
234 |     started = 0U;
235 |     for(i = 0U; i < 5U; i++)
236 |     {
237 |         if((buf[i] != '0') || (started != 0U) || (i == 4U))
238 |         {
239 |             started = 1U;
240 |             test_put_char((u8)buf[i]);
241 |         }
242 |     }
243 | }
244 | 
245 | static void test_put_s8(s8 dat)
246 | {
247 |     if(dat < 0)
248 |     {
249 |         test_put_char('-');
250 |         dat = (s8)(0 - dat);
251 |     }
252 |     test_put_u16((u16)((u8)dat));
253 | }
254 | 
255 | static void test_put_01_unit(u16 raw)
256 | {
257 |     test_put_u16((u16)(raw / 10U));
258 |     test_put_char('.');
259 |     test_put_char((u8)('0' + (raw % 10U)));
260 | }
261 | 
262 | static void test_print_flow_info(void)
263 | {
264 |     test_put_str("[���̲���] ��=");
265 |     test_put_u16(uart_1_wire.cell_series);
266 |     test_put_str("��");
267 |     test_put_u16(uart_1_wire.cell_parallel);
268 |     test_put_str("�� ����=");
269 |     test_put_01_unit(uart_1_wire.cell_cap_01ah);
270 |     test_put_str("Ah Ŀ���ѹmV=");
271 |     test_put_u16(uart_1_wire.target_voltage_mv);
272 |     test_put_str(" Ŀ�����mA=");
273 |     test_put_u16(uart_1_wire.target_current_ma);
274 |     test_put_crlf();
275 | 
276 |     test_put_str("[���̲���] ����mV=");
277 |     test_put_u16(uart_1_wire.cell_max_mv);
278 |     test_put_str(" ����=");
279 |     test_put_u16(uart_1_wire.soc_percent);
280 |     test_put_str(" �¶�=");
281 |     test_put_s8(uart_1_wire.batt_temp_degc);
282 |     test_put_str(" MOS=");
283 |     test_put_s8(uart_1_wire.mos_temp_degc);
284 |     test_put_str(" ״̬=0x");
285 |     test_put_hex8(uart_1_wire.charge_status);
286 |     test_put_crlf();
287 | }
288 | 
289 | static void test_flow_start(void)
290 | {
291 |     s_flow_run = 1U;
292 |     s_flow_charge = 0U;
293 |     s_flow_timeout_printed = 0U;
294 |     s_flow_info_tick_10ms = 0U;
295 | 
296 |     com_fifo_clear();
297 |     uart_1_wire_reset_link();
298 |     uart_1_wire_set_stage(U1W_STAGE_HANDSHAKE);
299 | 
300 |     test_put_str("[����] Э������");
301 |     test_put_crlf();
302 | }
303 | 
304 | static void test_flow_stop(void)
305 | {
306 |     s_flow_run = 0U;
307 |     s_flow_charge = 0U;
308 |     s_flow_timeout_printed = 0U;
309 |     s_flow_info_tick_10ms = 0U;
310 | 
311 |     uart_1_wire_set_stage(U1W_STAGE_STOP);
312 |     com_fifo_clear();
313 |     com_uart_set_rx_mode();
314 | 
315 |     test_put_str("[����] Э��ֹͣ���ص��ֶ�ģʽ");
316 |     test_put_crlf();
317 | }
318 | 
319 | static void test_flow_task(void)
320 | {
321 |     if(s_flow_run == 0U)
322 |     {
323 |         return;
324 |     }
325 | 
326 |     uart_1_wire_poll_10ms();
327 | 
328 |     if((u1w_info.comm_timeout != 0U) && (s_flow_timeout_printed == 0U))
329 |     {
330 |         s_flow_timeout_printed = 1U;
331 |         test_put_str("[����] ͨ�ų�ʱ ����=0x");
332 |         test_put_hex8(u1w_info.key_timeout_cmd);
333 |         test_put_crlf();
334 |     }
335 | 
336 |     if((s_flow_charge == 0U) && (u1w_info.handshake_ok != 0U))
337 |     {
338 |         s_flow_charge = 1U;
339 |         test_put_str("[����] �������");
340 |         test_put_crlf();
341 |         test_print_flow_info();
342 |         uart_1_wire_set_stage(U1W_STAGE_CHARGE);
343 |         test_put_str("[����] �е������ѯ");
344 |         test_put_crlf();
345 |     }
346 | 
347 |     s_flow_info_tick_10ms++;
348 |     if(s_flow_info_tick_10ms >= MAIN_1WIRE_INFO_PERIOD_10MS)
349 |     {
350 |         s_flow_info_tick_10ms = 0U;
351 |         test_print_flow_info();
352 |     }
353 | }
354 | 
355 | 
356 | static void test_debug_key_task(void)
357 | {
358 |     u8 dat;
359 | 
360 |     while(s_ch_fifo.cnt != 0U)
361 |     {
362 |         dat = ch_fifo_pop();
363 | 
364 |         if((dat == 'g') || (dat == 'G'))
365 |         {
366 |             test_flow_start();
367 |         }
368 |         else if((dat == 'x') || (dat == 'X'))
369 |         {
370 |             test_flow_stop();
371 |         }
372 |         else if(dat == '?')
373 |         {
374 |             test_print_help();
375 |         }
376 |         else if(s_flow_run != 0U)
377 |         {
378 |             /* ���̲��������У�ֻ��Ӧ g/x/?�������������ԡ� */
379 |         }
380 |         else if((dat >= '0') && (dat <= '8'))
381 |         {
382 |             s_1w_test_idx = (u8)(dat - '0');
383 |             test_send_current_cmd();
384 |         }
385 |         else if((dat == 's') || (dat == 'S') || (dat == 'r') || (dat == 'R'))
386 |         {
387 |             test_send_current_cmd();
388 |         }
389 |         else if((dat == 'n') || (dat == 'N'))
390 |         {
391 |             test_next_cmd();
392 |         }
393 |         else if((dat == 'p') || (dat == 'P'))
394 |         {
395 |             test_prev_cmd();
396 |         }
397 |         else
398 |         {
399 |             /* ���Իس������к�������Ч������ */
400 |         }
401 |     }
402 | }
403 | 
404 | static void test_rx_task(void)
405 | {
406 |     u8 dat;
407 | 
408 |     if(s_com_fifo.cnt == 0U)
409 |     {
410 |         return;
411 |     }
412 | 
413 |     test_put_str("[1W] ����=");
414 |     while(s_com_fifo.cnt != 0U)
415 |     {
416 |         dat = com_fifo_pop();
417 |         test_put_hex8(dat);
418 |         test_put_char(' ');
419 |     }
420 |     test_put_crlf();
421 | }
422 | 
423 | static void usr_1wire_test_func(void)
424 | {
425 |     s_1w_test_idx = 0U;
426 | 
427 |     ch_fifo_clear();
428 |     com_fifo_clear();
429 |     com_uart_set_rx_mode();
430 |     next_10ms = timer_deadline_ms(TASK_10MS);
431 | 
432 |     test_print_help();
433 | 
434 |     for(;;)
435 |     {
436 |         if(timer_period_elapsed(&next_10ms, TASK_10MS) != 0)
437 |         {
438 |             wdt_feed();
439 |             test_debug_key_task();
440 |             if(s_flow_run != 0U)
441 |             {
442 |                 test_flow_task();
443 |             }
444 |             else
445 |             {
446 |                 test_rx_task();
447 |             }
448 |         }
449 |     }
450 | }
451 | #endif
452 | 
453 | void main(void)
454 | {
455 |     Sys_Init();
456 |     read_cal_data();
457 |     uart_1_wire_init();
458 | 
459 |     usr_printf("%s %s\r\n", PROJECT_NAME, VERSION);
460 | 
461 |     for(;;)
462 |     {
463 | #if (MAIN_1WIRE_TEST_EN != 0U)
464 |         usr_1wire_test_func();
465 | #else
466 |         if(flg_cal_mode == 0)
467 |         {
468 |             usr_ch_func();
469 |         }
470 |         else
471 |         {
472 |             usr_cal_func();
473 |         }
474 | #endif
475 |     }
476 | }
477 | 
```

### YAT-60V3A-Uart-1-wire/App/pc_uart.c

Bytes: 7778
SHA-256: e33979bdf9a11421a60154c73aa4904aeaa1a9a2c519821ed13e7f4452b9dd5f
Lines: 1-247 of 247

```text
  1 | /**
  2 |   ******************************************************************************
  3 |   * @file    pc_uart.c
  4 |   * @brief   DEBUG ���ڽ����߼���
  5 |   *
  6 |   * ���ݵ㣺
  7 |   * 1. P24/P25 DEBUG ͨ���Ľ����������� ch_rx_fifo��
  8 |   * 2. �յ� "*RST" ���� 1������������ usr_cal_func()��
  9 |   * 3. ���� "par" ������ӡ��"ENA" �Զ��ϴ��� 40 �ֽڵ���֡��ʽ��
 10 |   * 4. ֻ���� DEBUG ���Կڣ������� P30 ����ͨ��Э�顣
 11 |   ******************************************************************************
 12 |   */
 13 | #include "pc_uart.h"
 14 | #include "fifo.h"
 15 | #include "uart.h"
 16 | #include "adc.h"
 17 | #include "ch.h"
 18 | #include "timer.h"
 19 | #include "uart_1_wire.h"
 20 | #include "stm32_crc.h"
 21 | 
 22 | #define PC_UART_DATA_LEN                 (36U)
 23 | #define PC_UART_FRAME_LEN                (40U)
 24 | #define PC_UART_RX_BUF_LEN               (16U)
 25 | 
 26 | u8 idata data_len_bk;
 27 | 
 28 | static u8 idata s_auto_count;
 29 | static bit s_tx_auto;
 30 | static u8 xdata s_pc_tx_buf[PC_UART_FRAME_LEN] = { 0 };
 31 | static u8 xdata s_pc_rx_buf[PC_UART_RX_BUF_LEN];
 32 | 
 33 | /**
 34 |   * @brief  ���������־������ PC ���Թ�����ʾ��
 35 |   */
 36 | static u8 pc_uart_pack_flag(void)
 37 | {
 38 |     u8 flag;
 39 | 
 40 |     flag = 0U;
 41 |     if(ch_flag.ch_ntcErr != 0U) { flag |= BIT8(0); }
 42 |     if(ch_flag.ch_hotErr != 0U) { flag |= BIT8(1); }
 43 |     if(ch_flag.ch_hot != 0U)    { flag |= BIT8(2); }
 44 |     if(ch_flag.ch_ovp != 0U)    { flag |= BIT8(3); }
 45 |     if(ch_flag.ch_ocp != 0U)    { flag |= BIT8(4); }
 46 |     if(ch_flag.ch_vacErr != 0U) { flag |= BIT8(5); }
 47 |     if(ch_flag.ch_hard != 0U)   { flag |= BIT8(6); }
 48 | 
 49 |     return flag;
 50 | }
 51 | 
 52 | static void pc_uart_print_current_line(char *name, u16 current_ma)
 53 | {
 54 |     uart_printf("%s  %umA\n", name, current_ma);
 55 | }
 56 | 
 57 | static void pc_uart_print_voltage_line(char *name, u16 voltage_mv)
 58 | {
 59 |     uart_printf("%s  %umV\n", name, voltage_mv);
 60 | }
 61 | 
 62 | static void pc_uart_print_voltage_range_line(char *name, u16 low_mv, u16 high_mv)
 63 | {
 64 |     uart_printf("%s  %u-%umV\n", name, low_mv, high_mv);
 65 | }
 66 | 
 67 | /**
 68 |   * @brief  ��ӡ��������������ԭ���Կ� "par" ������ڡ�
 69 |   */
 70 | static void pc_uart_print_param(void)
 71 | {
 72 |     uart_printf("%s %s\n", MODEL_NAME, VERSION);
 73 |     uart_printf("\n%s\n", PROJECT_NAME);
 74 | 
 75 |     pc_uart_print_current_line("�޸�����", iREPAIR);
 76 |     pc_uart_print_current_line("Ԥ�����", iPRE);
 77 |     pc_uart_print_current_line("��������", iMAX);
 78 |     pc_uart_print_current_line("ת�Ƶ���", iGED);
 79 |     pc_uart_print_current_line("��������", iOCP);
 80 |     pc_uart_print_current_line("�����ָ�", iOCP_OK);
 81 | 
 82 |     pc_uart_print_voltage_line("\nʶ���ѹ", vSTART);
 83 |     pc_uart_print_voltage_line("�޸�����", vPRE_30V);
 84 |     pc_uart_print_voltage_range_line("Ԥ���ѹ", vPRE_30V, vPRE_37V5);
 85 |     pc_uart_print_voltage_line("��ߵ�ѹ", SET_vMAX);
 86 |     pc_uart_print_voltage_line("��ѹ����", vDCOVP);
 87 | 
 88 |     uart_printf("\n�ڲ�NTC:\n");
 89 |     uart_printf("���¹ض� ADC>%u\n", T_HOT_ERR);
 90 |     uart_printf("���»ָ� ADC<%u\n", T_HOT_ERR_OK);
 91 |     uart_printf("������ʾ ADC>%u\n", T_CH_HOT);
 92 |     uart_printf("��ʾ�ָ� ADC<%u\n", T_CH_HOT_OK);
 93 | 
 94 |     uart_printf("\nԤ�䶨ʱ %umin\n", TIM_PRE);
 95 |     uart_printf("CC+CV��ʱ %umin\n", TIM_CCCV);
 96 | 
 97 |     uart_printf("��������: %uk\n", (u16)R1);
 98 |     uart_printf("��������: %uk\n", (u16)R2);
 99 |     uart_printf("��������: %umR\n", (u16)Ra);
100 |     uart_printf("GAIN: %u\n", (u16)GAIN);
101 | 
102 |     uart_printf("\n״̬:\n0����\n1 ���\n2 ����\n3 Ԥ��\n4 CCCV\n5 ����\n6 OVP\n7 Ԥ��\n8 OTP\n");
103 |     uart_printf("9 OCP\n10 NTC\n11 Ӳ��\n12 Ƿѹ\n13 CV��ʱ\n14 BMS��\n15 BMS�쳣\n16 �޸�\n17 �ϻ�\n");
104 |     uart_printf("��ǰ״̬: %u\n", (u16)ch_state);
105 | }
106 | 
107 | /**
108 |   * @brief  ��ӡ��ǰ������״̬������ DEBUG �˹�������
109 |   */
110 | void pc_uart_print_batt(void)
111 | {
112 |     uart_printf("[���] �����ѹ=%u mV, �������=%u mA, NTC=%d(0.1C)\r\n",
113 |                 val.vout,
114 |                 val.curr,
115 |                 (s16)val.i_ntc);
116 |     uart_printf("[���] ���״̬=%bu, ���ϱ�־=%bu, ʱ��=%u\r\n",
117 |                 (u8)ch_state,
118 |                 pc_uart_pack_flag(),
119 |                 timer_get_tick_ms());
120 |     uart_printf("[BMS] �׶�=%bu, ����=%bu, ��ʱ=%bu, ��ʱ����=%bu\r\n",
121 |                 u1w_info.stage,
122 |                 u1w_info.handshake_ok,
123 |                 u1w_info.comm_timeout,
124 |                 u1w_info.key_timeout_cmd);
125 |     uart_printf("[BMS] ��ذ�=%bu��%bu��, ����=%u(0.1Ah), Ŀ��=%u mV/%u mA\r\n",
126 |                 uart_1_wire.cell_series,
127 |                 uart_1_wire.cell_parallel,
128 |                 uart_1_wire.cell_cap_01ah,
129 |                 u1w_info.target_voltage_mv,
130 |                 u1w_info.target_current_ma);
131 |     uart_printf("[BMS] �������=%u mV, ����=%bu, ����¶�=%dC, MOS�¶�=%dC, ״̬=%bu\r\n",
132 |                 u1w_info.cell_max_mv,
133 |                 u1w_info.soc_percent,
134 |                 (s16)u1w_info.batt_temp_degc,
135 |                 (s16)u1w_info.mos_temp_degc,
136 |                 u1w_info.charge_status);
137 | }
138 | /**
139 |   * @brief  д�� u16/u32 С�����ݣ����� 54.6V ԭ����֡�ֽ���
140 |   */
141 | static void pc_uart_put_u16(u8 *buf, u8 index, u16 value)
142 | {
143 |     buf[index + 0U] = (u8)value;
144 |     buf[index + 1U] = (u8)(value >> 8);
145 | }
146 | 
147 | static void pc_uart_put_u32(u8 *buf, u8 index, u32 value)
148 | {
149 |     buf[index + 0U] = (u8)value;
150 |     buf[index + 1U] = (u8)(value >> 8);
151 |     buf[index + 2U] = (u8)(value >> 16);
152 |     buf[index + 3U] = (u8)(value >> 24);
153 | }
154 | 
155 | /**
156 |   * @brief  ���� 54.6V ���� 40 �ֽ��Զ�����֡��
157 |   */
158 | static void pc_uart_send_auto_frame(void)
159 | {
160 |     u32 crc32;
161 | 
162 |     pc_uart_put_u32(s_pc_tx_buf, 0U,  (u32)val.vout);
163 |     pc_uart_put_u32(s_pc_tx_buf, 4U,  (u32)val.vdc);   /* ����Ŀ�޶��� VDC��val.vdc �� ADC �㾵�� val.vout�� */
164 |     pc_uart_put_u32(s_pc_tx_buf, 8U,  (u32)val.curr);
165 |     pc_uart_put_u32(s_pc_tx_buf, 12U, 0UL);
166 |     pc_uart_put_u16(s_pc_tx_buf, 16U, 0U);
167 |     pc_uart_put_u16(s_pc_tx_buf, 18U, (u16)val.i_ntc);
168 | 
169 |     s_pc_tx_buf[20] = (u8)ch_state;
170 |     s_pc_tx_buf[21] = pc_uart_pack_flag();
171 | 
172 |     pc_uart_put_u16(s_pc_tx_buf, 22U, 0U);
173 | 
174 |     pc_uart_put_u32(s_pc_tx_buf, 28U, 0UL);
175 |     pc_uart_put_u16(s_pc_tx_buf, 32U, 0U);
176 |     pc_uart_put_u16(s_pc_tx_buf, 34U, JIZHONG);
177 | 
178 |     crc32 = CRC32(s_pc_tx_buf, PC_UART_DATA_LEN);
179 |     pc_uart_put_u32(s_pc_tx_buf, 36U, crc32);
180 |     ch_uart_send_buf((char *)s_pc_tx_buf, PC_UART_FRAME_LEN);
181 | }
182 | 
183 | 
184 | 
185 | /**
186 |   * @brief  DEBUG ��������
187 |   *
188 |   * @retval 1: �յ� "*RST"������������У׼��0: ���ֵ�ǰ������̡�
189 |   */
190 | u8 pc_uart_func(void)
191 | {
192 |     u8 data_len;
193 |     u8 i;
194 |     u8 status;
195 | 
196 |     status = 0U;
197 | 
198 |     data_len = s_ch_fifo.cnt;
199 |     if((data_len == data_len_bk)&&(data_len != 0))
200 |     {
201 |         if(data_len > PC_UART_RX_BUF_LEN)
202 |         {
203 |             data_len = PC_UART_RX_BUF_LEN;
204 |         }
205 | 
206 |         for(i = 0U; i < data_len; i++)
207 |         {
208 |             s_pc_rx_buf[i] = ch_fifo_pop();
209 |         }
210 | 
211 |         if(data_len >= 4U)
212 |         {
213 |             if((s_pc_rx_buf[0] == '*') && (s_pc_rx_buf[1] == 'R') &&
214 |                (s_pc_rx_buf[2] == 'S') && (s_pc_rx_buf[3] == 'T'))
215 |             {
216 |                 status = 1U;
217 |             }
218 |             else if((s_pc_rx_buf[0] == 'b') && (s_pc_rx_buf[1] == 'a') &&
219 |                     (s_pc_rx_buf[2] == 't') && (s_pc_rx_buf[3] == 't'))
220 |             {
221 |                 pc_uart_print_batt();
222 |             }
223 |         }
224 | 
225 |         if(data_len >= 3U)
226 |         {
227 |             if((s_pc_rx_buf[0] == 'p') && (s_pc_rx_buf[1] == 'a') && (s_pc_rx_buf[2] == 'r'))
228 |             {
229 |                 pc_uart_print_param();
230 |             }
231 |             else if((s_pc_rx_buf[0] == 'E') && (s_pc_rx_buf[1] == 'N') && (s_pc_rx_buf[2] == 'A'))
232 |             {
233 |                 if(++s_auto_count >= 20U)
234 |                 {
235 |                     s_tx_auto = 1;
236 |                     s_auto_count = 0U;
237 |                     pc_uart_send_auto_frame();
238 |                 }
239 |             }
240 |         }
241 |     }
242 |     
243 |     data_len_bk = data_len;
244 |     
245 |     return status;
246 | }
247 | 
```

### YAT-60V3A-Uart-1-wire/App/pc_uart.h

Bytes: 402
SHA-256: 960bcf9440ce30508f5df450da15f9c1afbf7c6757187dee63a987bd76e0fbb4
Lines: 1-16 of 16

```text
 1 | /**
 2 |   ******************************************************************************
 3 |   * @file    pc_uart.h
 4 |   * @brief   DEBUG ���ڽ����ӿڣ����� 54.6V2.5A pc_uart.c ��Ϊ��
 5 |   ******************************************************************************
 6 |   */
 7 | #ifndef __PC_UART_H__
 8 | #define __PC_UART_H__
 9 | 
10 | #include "common.h"
11 | 
12 | u8 pc_uart_func(void);
13 | void pc_uart_print_batt(void);
14 | 
15 | #endif
16 | 
```

### YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c

Bytes: 8778
SHA-256: 79fffad8b0260b129c0cc83240abde1ee6f401f50380c59afaba7ef396668f23
Lines: 1-371 of 371

```text
  1 | /**
  2 |   ******************************************************************************
  3 |   * @file    uart_1_wire_debug.c
  4 |   * @brief   P30 单线 UART 轻量调试日志。
  5 |   *
  6 |   * 说明：
  7 |   * 1. 正常成功帧合并为一行，减少串口打印占用。
  8 |   * 2. 原始发送/接收数据统一用连续 16 进制显示。
  9 |   * 3. 运行时日志只用 ASCII，避免 C51 中文字符串乱码和代码空间增加。
 10 |   ******************************************************************************
 11 |   */
 12 | 
 13 | #include "uart_1_wire_debug.h"
 14 | #include "uart.h"
 15 | 
 16 | #if (U1W_DEBUG_EN != 0U)
 17 | 
 18 | /* 缓存最近一次发送帧，用于收到回复时合并打印一行日志。 */
 19 | static u8 idata s_dbg_tx_cmd;
 20 | static u8 idata s_dbg_tx_len;
 21 | static u8 idata s_dbg_tx_buf[4];
 22 | 
 23 | /* 发送一个调试字符，底层仍走调试 UART。 */
 24 | static void u1w_dbg_put_char(char ch)
 25 | {
 26 |     ch_uart_send_byte((u8)ch);
 27 | }
 28 | 
 29 | static void u1w_dbg_put_str(char *str)
 30 | {
 31 |     ch_uart_send_str(str);
 32 | }
 33 | 
 34 | static void u1w_dbg_put_hex4(u8 val)
 35 | {
 36 |     val &= 0x0FU;
 37 |     if(val < 10U)
 38 |     {
 39 |         u1w_dbg_put_char((char)('0' + val));
 40 |     }
 41 |     else
 42 |     {
 43 |         u1w_dbg_put_char((char)('A' + val - 10U));
 44 |     }
 45 | }
 46 | 
 47 | /* 打印 1 字节十六进制，不带 0x，例如 A0。 */
 48 | static void u1w_dbg_put_hex8(u8 val)
 49 | {
 50 |     u1w_dbg_put_hex4((u8)(val >> 4));
 51 |     u1w_dbg_put_hex4(val);
 52 | }
 53 | 
 54 | /* 命令过滤：0 表示全部打印，非 0 只打印指定命令。 */
 55 | static bit u1w_dbg_cmd_allowed(u8 cmd)
 56 | {
 57 | #if (U1W_DEBUG_CMD_FILTER != 0U)
 58 |     if(cmd != U1W_DEBUG_CMD_FILTER)
 59 |     {
 60 |         return 0;
 61 |     }
 62 | #else
 63 |     if(cmd == 0xFFU)
 64 |     {
 65 |         return 1;
 66 |     }
 67 | #endif
 68 |     return 1;
 69 | }
 70 | 
 71 | /* 命令短名，兼顾可读性和代码空间。 */
 72 | static char *u1w_dbg_cmd_name(u8 cmd)
 73 | {
 74 |     switch(cmd)
 75 |     {
 76 |     case U1W_CMD_A0: return "ID";
 77 |     case U1W_CMD_A1: return "CAP";
 78 |     case U1W_CMD_A4: return "VSET";
 79 |     case U1W_CMD_A6: return "ISET";
 80 |     case U1W_CMD_A7: return "TDER";
 81 |     case U1W_CMD_B1: return "CELLV";
 82 |     case U1W_CMD_B3: return "TEMP";
 83 |     case U1W_CMD_B4: return "SOC";
 84 |     case U1W_CMD_B6: return "MOS";
 85 |     default:         return "UNK";
 86 |     }
 87 | }
 88 | 
 89 | static void u1w_dbg_put_cmd_tag(u8 cmd)
 90 | {
 91 |     u1w_dbg_put_hex8(cmd);
 92 |     u1w_dbg_put_char('[');
 93 |     u1w_dbg_put_str(u1w_dbg_cmd_name(cmd));
 94 |     u1w_dbg_put_char(']');
 95 | }
 96 | 
 97 | /* 连续打印原始帧数据，中间不加空格以减少日志长度。 */
 98 | static void u1w_dbg_put_bytes(u8 *buf, u8 len)
 99 | {
100 |     u8 i;
101 | 
102 |     for(i = 0U; i < len; i++)
103 |     {
104 |         u1w_dbg_put_hex8(buf[i]);
105 |     }
106 | }
107 | 
108 | #if (U1W_DEBUG_PARSE_EN != 0U)
109 | static void u1w_dbg_put_u16(u16 dat)
110 | {
111 |     char buf[5];
112 |     u8 i;
113 |     u8 started;
114 | 
115 |     buf[0] = (char)('0' + (dat / 10000U));
116 |     dat %= 10000U;
117 |     buf[1] = (char)('0' + (dat / 1000U));
118 |     dat %= 1000U;
119 |     buf[2] = (char)('0' + (dat / 100U));
120 |     dat %= 100U;
121 |     buf[3] = (char)('0' + (dat / 10U));
122 |     buf[4] = (char)('0' + (dat % 10U));
123 | 
124 |     started = 0U;
125 |     for(i = 0U; i < 5U; i++)
126 |     {
127 |         if((buf[i] != '0') || (started != 0U) || (i == 4U))
128 |         {
129 |             started = 1U;
130 |             u1w_dbg_put_char(buf[i]);
131 |         }
132 |     }
133 | }
134 | 
135 | static void u1w_dbg_put_s8(s8 dat)
136 | {
137 |     if(dat < 0)
138 |     {
139 |         u1w_dbg_put_char('-');
140 |         dat = (s8)(0 - dat);
141 |     }
142 |     u1w_dbg_put_u16((u16)((u8)dat));
143 | }
144 | 
145 | static void u1w_dbg_put_01a(u16 raw)
146 | {
147 |     u1w_dbg_put_u16((u16)(raw / 10U));
148 |     u1w_dbg_put_char('.');
149 |     u1w_dbg_put_char((char)('0' + (raw % 10U)));
150 |     u1w_dbg_put_char('A');
151 | }
152 | 
153 | static u16 u1w_dbg_get_u16(u8 byte0, u8 byte1)
154 | {
155 |     return (u16)((u16)byte0 | ((u16)byte1 << 8));
156 | }
157 | 
158 | static void u1w_dbg_put_parse(u8 *buf, u8 len)
159 | {
160 |     u8 cmd;
161 |     u8 xy;
162 |     u16 raw1;
163 |     u16 raw2;
164 | 
165 |     if(len < 4U)
166 |     {
167 |         return;
168 |     }
169 | 
170 |     cmd = buf[0];
171 |     switch(cmd)
172 |     {
173 |     case U1W_CMD_A0:
174 |         xy = buf[2];
175 |         u1w_dbg_put_str(" ID=0x");
176 |         u1w_dbg_put_hex8(buf[1]);
177 |         u1w_dbg_put_char(' ');
178 |         u1w_dbg_put_u16((u16)(((xy >> 4) & 0x0FU) + 5U));
179 |         u1w_dbg_put_char('S');
180 |         u1w_dbg_put_u16((u16)((xy & 0x0FU) + 1U));
181 |         u1w_dbg_put_char('P');
182 |         break;
183 | 
184 |     case U1W_CMD_A1:
185 |         u1w_dbg_put_str(" CAP=");
186 |         u1w_dbg_put_u16((u16)(buf[2] / 10U));
187 |         u1w_dbg_put_char('.');
188 |         u1w_dbg_put_char((char)('0' + (buf[2] % 10U)));
189 |         u1w_dbg_put_str("Ah");
190 |         break;
191 | 
192 |     case U1W_CMD_A4:
193 |         if(len >= 6U)
194 |         {
195 |             raw1 = u1w_dbg_get_u16(buf[1], buf[2]);
196 |             raw2 = u1w_dbg_get_u16(buf[3], buf[4]);
197 |             u1w_dbg_put_str(" PRE=");
198 |             u1w_dbg_put_u16((u16)(raw1 * 10U));
199 |             u1w_dbg_put_str("mV FULL=");
200 |             u1w_dbg_put_u16((u16)(raw2 * 10U));
201 |             u1w_dbg_put_str("mV");
202 |         }
203 |         break;
204 | 
205 |     case U1W_CMD_A6:
206 |         if(len >= 6U)
207 |         {
208 |             raw1 = u1w_dbg_get_u16(buf[1], buf[2]);
209 |             raw2 = u1w_dbg_get_u16(buf[3], buf[4]);
210 |             u1w_dbg_put_str(" I_CELL=");
211 |             u1w_dbg_put_01a(raw1);
212 |             u1w_dbg_put_str(" RAW2=");
213 |             u1w_dbg_put_u16(raw2);
214 |         }
215 |         break;
216 | 
217 |     case U1W_CMD_A7:
218 |         if(len >= 6U)
219 |         {
220 |             u1w_dbg_put_str(" T=");
221 |             u1w_dbg_put_s8((s8)buf[1]);
222 |             u1w_dbg_put_str("..");
223 |             u1w_dbg_put_s8((s8)buf[2]);
224 |             u1w_dbg_put_str("C DER=");
225 |             u1w_dbg_put_01a(buf[4]);
226 |         }
227 |         break;
228 | 
229 |     case U1W_CMD_B1:
230 |         if(len >= 6U)
231 |         {
232 |             raw1 = u1w_dbg_get_u16(buf[1], buf[2]);
233 |             raw2 = u1w_dbg_get_u16(buf[3], buf[4]);
234 |             u1w_dbg_put_str(" MIN=");
235 |             u1w_dbg_put_u16((u16)(raw1 * 10U));
236 |             u1w_dbg_put_str("mV MAX=");
237 |             u1w_dbg_put_u16((u16)(raw2 * 10U));
238 |             u1w_dbg_put_str("mV");
239 |         }
240 |         break;
241 | 
242 |     case U1W_CMD_B3:
243 |         u1w_dbg_put_str(" BAT=");
244 |         u1w_dbg_put_s8((s8)buf[1]);
245 |         u1w_dbg_put_str("C MOS_T=");
246 |         u1w_dbg_put_s8((s8)buf[2]);
247 |         u1w_dbg_put_char('C');
248 |         break;
249 | 
250 |     case U1W_CMD_B4:
251 |         u1w_dbg_put_str(" SOC=");
252 |         u1w_dbg_put_u16(buf[1]);
253 |         u1w_dbg_put_str("% ST=0x");
254 |         u1w_dbg_put_hex8(buf[2]);
255 |         break;
256 | 
257 |     case U1W_CMD_B6:
258 |         u1w_dbg_put_str(" TYPE=0x");
259 |         u1w_dbg_put_hex8(buf[1]);
260 |         u1w_dbg_put_str(" VAL=0x");
261 |         u1w_dbg_put_hex8(buf[2]);
262 |         break;
263 | 
264 |     default:
265 |         break;
266 |     }
267 | }
268 | #endif
269 | 
270 | /* 只缓存主机发送帧，当前发送帧最长按 4 字节处理。 */
271 | static void u1w_dbg_copy_tx(u8 *src, u8 len)
272 | {
273 |     u8 i;
274 | 
275 |     for(i = 0U; i < len; i++)
276 |     {
277 |         s_dbg_tx_buf[i] = src[i];
278 |     }
279 | }
280 | 
281 | /* 发送时只缓存，不立即打印，避免一帧拆成多行。 */
282 | void u1w_dbg_tx(u8 *buf, u8 tx_len, u8 expect_len)
283 | {
284 |     u8 cmd;
285 | 
286 |     cmd = buf[0];
287 |     if(u1w_dbg_cmd_allowed(cmd) == 0)
288 |     {
289 |         return;
290 |     }
291 | 
292 |     if(tx_len > 4U)
293 |     {
294 |         tx_len = 4U;
295 |     }
296 | 
297 |     s_dbg_tx_cmd = cmd;
298 |     s_dbg_tx_len = tx_len;
299 |     u1w_dbg_copy_tx(buf, tx_len);
300 | 
301 |     if(expect_len > COM_FRAME_LEN)
302 |     {
303 |         s_dbg_tx_len = 0U;
304 |     }
305 | }
306 | 
307 | /* 收到合法回复后，把最近 TX 和当前 RX 合并成一行打印。 */
308 | void u1w_dbg_rx_ok(u8 *buf, u8 rx_len)
309 | {
310 |     u8 cmd;
311 | 
312 |     cmd = buf[0];
313 |     if(u1w_dbg_cmd_allowed(cmd) == 0)
314 |     {
315 |         return;
316 |     }
317 | 
318 |     u1w_dbg_put_str("1W ");
319 |     u1w_dbg_put_cmd_tag(cmd);
320 |     u1w_dbg_put_str(" TX=");
321 |     if((s_dbg_tx_len != 0U) && (s_dbg_tx_cmd == cmd))
322 |     {
323 |         u1w_dbg_put_bytes(s_dbg_tx_buf, s_dbg_tx_len);
324 |     }
325 |     else
326 |     {
327 |         u1w_dbg_put_char('-');
328 |     }
329 | 
330 |     u1w_dbg_put_str(" RX=");
331 |     u1w_dbg_put_bytes(buf, rx_len);
332 | #if (U1W_DEBUG_PARSE_EN != 0U)
333 |     u1w_dbg_put_parse(buf, rx_len);
334 | #endif
335 |     u1w_dbg_put_str(" OK\r\n");
336 | }
337 | 
338 | /* 坏帧日志：保留原始 RX 和必要错误原因。 */
339 | void u1w_dbg_rx_bad(u8 reason, u8 wait_cmd, u8 wait_len, u8 rx_len, u8 *buf, u8 chk_calc)
340 | {
341 |     if(u1w_dbg_cmd_allowed(wait_cmd) == 0)
342 |     {
343 |         return;
344 |     }
345 | 
346 |     u1w_dbg_put_str("BAD ");
347 |     u1w_dbg_put_cmd_tag(wait_cmd);
348 |     u1w_dbg_put_str(" RX=");
349 |     u1w_dbg_put_bytes(buf, rx_len);
350 |     if(reason == U1W_DBG_BAD_CHK)
351 |     {
352 |         u1w_dbg_put_str(" CALC=");
353 |         u1w_dbg_put_hex8(chk_calc);
354 |     }
355 |     else if(reason == U1W_DBG_BAD_LEN)
356 |     {
357 |         u1w_dbg_put_str(" LEN=");
358 |         u1w_dbg_put_hex8(rx_len);
359 |         u1w_dbg_put_char('/');
360 |         u1w_dbg_put_hex8(wait_len);
361 |     }
362 |     else if(reason == U1W_DBG_BAD_CMD)
363 |     {
364 |         u1w_dbg_put_str(" CMD");
365 |     }
366 |     u1w_dbg_put_str("\r\n");
367 | }
368 | 
369 | 
370 | #endif
371 | 
```

### YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h

Bytes: 2371
SHA-256: d02ecc0e552091ba6262097a68d6f4b965a58f04198d7512bd792f2015751d73
Lines: 1-57 of 57

```text
 1 | /**
 2 |   ******************************************************************************
 3 |   * @file    uart_1_wire_debug.h
 4 |   * @brief   P30 单线 UART 协议轻量调试日志接口。
 5 |   *
 6 |   * 设计目标：
 7 |   * 1. 保留协议调试能力。
 8 |   * 2. 默认关闭，不占正式版代码空间。
 9 |   * 3. 调试日志使用 ASCII + 16 进制，方便对照协议和抓包。
10 |   ******************************************************************************
11 |   */
12 | #ifndef __UART_1_WIRE_DEBUG_H__
13 | #define __UART_1_WIRE_DEBUG_H__
14 | 
15 | #include "uart_1_wire.h"
16 | 
17 | /* 总开关：0=关闭全部一线调试日志，1=打开。 */
18 | #ifndef U1W_DEBUG_EN
19 | #define U1W_DEBUG_EN                        (0U)
20 | #endif
21 | 
22 | /* 解析日志开关：当前成功帧已合并打印，本宏保留给调试配置使用。 */
23 | #ifndef U1W_DEBUG_PARSE_EN
24 | #define U1W_DEBUG_PARSE_EN                  (1U)
25 | #endif
26 | 
27 | /* 命令过滤：0=全部打印，其他值=只打印指定命令。 */
28 | #ifndef U1W_DEBUG_CMD_FILTER
29 | #define U1W_DEBUG_CMD_FILTER                (0U)
30 | #endif
31 | 
32 | /* 坏帧原因：用于 u1w_dbg_rx_bad() 判断打印哪类错误。 */
33 | #define U1W_DBG_BAD_NONE                    (0U)       /* 未指定错误原因。 */
34 | #define U1W_DBG_BAD_LEN                     (1U)       /* 长度不足或长度不匹配。 */
35 | #define U1W_DBG_BAD_CMD                     (2U)       /* 命令字不符合当前等待帧。 */
36 | #define U1W_DBG_BAD_CHK                     (3U)       /* 校验和错误。 */
37 | 
38 | #if (U1W_DEBUG_EN != 0U)
39 | /* 记录本次发送帧；只缓存，不立即打印，等待收到回复后合并输出。 */
40 | void u1w_dbg_tx(u8 *buf, u8 tx_len, u8 expect_len);
41 | 
42 | /* 收到合法回复帧；输出一行 TX/RX/OK 调试日志。 */
43 | void u1w_dbg_rx_ok(u8 *buf, u8 rx_len);
44 | 
45 | /* 收到异常回复帧；输出 BAD 日志和必要的错误细节。 */
46 | void u1w_dbg_rx_bad(u8 reason, u8 wait_cmd, u8 wait_len, u8 rx_len, u8 *buf, u8 chk_calc);
47 | 
48 | #else
49 | /* 正式版关闭日志时，所有调试接口编译为空操作，不占运行时间。 */
50 | #define u1w_dbg_tx(buf, tx_len, expect_len)                                      do{}while(0)
51 | #define u1w_dbg_rx_ok(buf, rx_len)                                                do{}while(0)
52 | #define u1w_dbg_rx_bad(reason, wait_cmd, wait_len, rx_len, buf, chk_calc)          do{}while(0)
53 | 
54 | #endif
55 | 
56 | #endif
57 | 
```

### YAT-60V3A-Uart-1-wire/App/uart_1_wire.c

Bytes: 23952
SHA-256: 3149377cc1ed227885225bd11391d9daa87ca17ab3f9773ae2f24743a736409f
Lines: 1-892 of 892

```text
  1 | /**
  2 |   ******************************************************************************
  3 |   * @file    uart_1_wire.c
  4 |   * @brief   P30 ���� UART ͨ��Э��ʵ�֣��첽��ѯ�档
  5 |   *
  6 |   * �����̣�
  7 |   * 1. ���գ�FIFO �ֽڽ��뻺��󣬴�ͷ for ɨ����֪ CMD��
  8 |   *    CMD ��֪�����ȹ���CHK ��ȷ������Ϊ�ǺϷ�֡���������ݡ�
  9 |   *    У��ʧ�ܲ�������������Ǵ���һ�ֽڼ�����֡ͷ��
 10 |   *
 11 |   * 2. ���ͣ����ٵȴ�ĳһ֡�ظ���
 12 |   *    ch.c ���� STOP / HANDSHAKE / CHARGE �׶κ󣬱��ļ�ÿ 100ms ��һ֡��
 13 |   *    HANDSHAKE �׶���ѯ A0/A1/A4/A6/A7/B1/B3/B4��
 14 |   *    CHARGE �׶���ѯ B1/B3/B4/B6��B6 �̶�Ҫ��򿪳�� MOS��
 15 |   *
 16 |   * 3. ��ʱ��
 17 |   *    2 ��û������Ϸ�֡��ͨ�ų�ʱ��
 18 |   *    3 ��ؼ�֡û��ˢ�£�Ҳ��ͨ�ų�ʱ��
 19 |   ******************************************************************************
 20 |   */
 21 | 
 22 | #include "uart_1_wire.h"
 23 | #include "uart.h"
 24 | #include "fifo.h"
 25 | #include "gpio.h"
 26 | #include "timer.h"
 27 | #include "uart_1_wire_debug.h"
 28 | 
 29 | #ifndef U1W_TX_PERIOD_10MS
 30 | #define U1W_TX_PERIOD_10MS                 (10U)      /* 100ms ��һ֡ */
 31 | #endif
 32 | 
 33 | #ifndef U1W_ANY_RX_TIMEOUT_10MS
 34 | #define U1W_ANY_RX_TIMEOUT_10MS            (200U)     /* 2��û������Ϸ�֡ */
 35 | #endif
 36 | 
 37 | #ifndef U1W_KEY_RX_TIMEOUT_10MS
 38 | #define U1W_KEY_RX_TIMEOUT_10MS            (300U)     /* 3��ؼ�֡û��ˢ�� */
 39 | #endif
 40 | 
 41 | #ifndef U1W_FULL_DISPLAY_10MS
 42 | #define U1W_FULL_DISPLAY_10MS              (18000U)   /* ������ʾ3���� */
 43 | #endif
 44 | 
 45 | #define U1W_LIMIT_VOLTAGE(v)               (((v) == 0U) ? SET_vMAX : (((v) > SET_vMAX) ? SET_vMAX : (v)))
 46 | #define U1W_LIMIT_CURRENT(i)               (((i) == 0U) ? iMAX     : (((i) > iMAX)     ? iMAX     : (i)))
 47 | 
 48 | typedef enum
 49 | {
 50 |     U1W_KEY_A0 = 0U,
 51 |     U1W_KEY_A1,
 52 |     U1W_KEY_A4,
 53 |     U1W_KEY_A6,
 54 |     U1W_KEY_A7,
 55 |     U1W_KEY_B1,
 56 |     U1W_KEY_B3,
 57 |     U1W_KEY_B4,
 58 |     U1W_KEY_MAX
 59 | } U1W_KEY_Types;
 60 | 
 61 | #define U1W_HANDSHAKE_MASK                 (0xFFU)
 62 | 
 63 | typedef struct
 64 | {
 65 |     u8  stage;                              /* UART_1WIRE_STAGE_Types */
 66 |     u8  tx_index;                           /* ��ǰ���ͱ��±� */
 67 |     u8  tx_tick_10ms;                       /* 100ms ���ͽ��� */
 68 |     u8  rx_len;                             /* ���ջ�����Ч���� */
 69 |     u8  rx_fifo_last_cnt;                   /* ��һ�ο����� FIFO ������������ģ����տ��� */
 70 |     u16 any_rx_age_10ms;                    /* ����Ϸ�֡���û�յ� */
 71 |     u16 full_display_10ms;                  /* ������ʾ����ʱ�� */
 72 |     u16 key_age_10ms[U1W_KEY_MAX];          /* �ؼ�֡���û�յ� */
 73 | } U1W_CTRL_Types;
 74 | 
 75 | static U1W_CTRL_Types idata s_u1w;
 76 | static u8 xdata s_rx_buf[COM_FRAME_LEN];
 77 | static u8 idata s_tx_buf[4];
 78 | 
 79 | static u8 code s_handshake_cmd[] =
 80 | {
 81 |     U1W_CMD_A0,
 82 |     U1W_CMD_A1,
 83 |     U1W_CMD_A4,
 84 |     U1W_CMD_A6,
 85 |     U1W_CMD_A7,
 86 |     U1W_CMD_B1,
 87 |     U1W_CMD_B3,
 88 |     U1W_CMD_B4,
 89 | };
 90 | 
 91 | static u8 code s_charge_cmd[] =
 92 | {
 93 |     U1W_CMD_B1,
 94 |     U1W_CMD_B3,
 95 |     U1W_CMD_B4,
 96 |     U1W_CMD_B6,
 97 | };
 98 | 
 99 | static u8 code s_temp_wait_cmd[] =
100 | {
101 |     U1W_CMD_B3,
102 |     U1W_CMD_B4,
103 | };
104 | 
105 | UART_1WIRE_INFO_Types xdata uart_1_wire;
106 | UART_1WIRE_CHARGE_INFO_Types idata u1w_info;
107 | 
108 | static void u1w_release_com(void)
109 | {
110 |     com_uart_set_rx_mode();
111 | }
112 | 
113 | static void u1w_pull_com_low(void)
114 | {
115 |     /*
116 |      * ������������ͨ�ţ�
117 |      * 1. �ر� UART1 ����/������ã�
118 |      * 2. P30 ��Ϊ��ͨ���������
119 |      * 3. ����͵�ƽ�����ֵ��ε�ػ��������֡�
120 |      *
121 |      * ˵����Э���еġ�COM�͵�ƽ����30ms���Ǹ�BMS�ж������Ͽ��ģ�
122 |      * �����಻�ٰ�����͵�ƽ�����쳣��⡣
123 |      */
124 |     __DisableIRQ(UART1_IRQn);
125 |     UART1_CON0 = 0U;
126 |     FIN_S8 = 0U;
127 |     FOUT_S30 = 0U;
128 |     COM_UART_OUTPUT();
129 |     COM_PIN = 0U;
130 |     __EnableIRQ(UART1_IRQn);
131 | }
132 | 
133 | /*============================== С���ߺ��� ==================================*/
134 | 
135 | static u8 u1w_sum(u8 *buf, u8 len)
136 | {
137 |     u8 i;
138 |     u8 sum;
139 | 
140 |     sum = 0U;
141 |     for(i = 0U; i < len; i++)
142 |     {
143 |         sum = (u8)(sum + buf[i]);
144 |     }
145 | 
146 |     return sum;
147 | }
148 | 
149 | static u16 u1w_get_u16_bus(u8 byte0, u8 byte1)
150 | {
151 |     return (u16)((u16)byte0 | ((u16)byte1 << 8));
152 | }
153 | 
154 | static u16 u1w_001v_to_mv(u16 raw_001v)
155 | {
156 |     if(raw_001v > 6500U)
157 |     {
158 |         raw_001v = 6500U;
159 |     }
160 | 
161 |     return (u16)(raw_001v * 10U);
162 | }
163 | 
164 | static u16 u1w_cell_01a_to_pack_ma(u16 raw_cell_01a)
165 | {
166 |     u8 parallel;
167 |     u32 ma;
168 | 
169 |     parallel = uart_1_wire.cell_parallel;
170 |     if((parallel == 0U) || (parallel > 16U))
171 |     {
172 |         parallel = 1U;
173 |     }
174 | 
175 |     ma = (u32)raw_cell_01a * 100UL * (u32)parallel;
176 |     if(ma > 0xFFFFUL)
177 |     {
178 |         ma = 0xFFFFUL;
179 |     }
180 | 
181 |     return (u16)ma;
182 | }
183 | 
184 | static u8 u1w_reply_len(u8 cmd)
185 | {
186 |     switch(cmd)
187 |     {
188 |     case U1W_CMD_A4:
189 |     case U1W_CMD_A6:
190 |     case U1W_CMD_A7:
191 |     case U1W_CMD_B1:
192 |         return 6U;
193 | 
194 |     case U1W_CMD_A0:
195 |     case U1W_CMD_A1:
196 |     case U1W_CMD_B3:
197 |     case U1W_CMD_B4:
198 |     case U1W_CMD_B6:
199 |         return 4U;
200 | 
201 |     default:
202 |         return 0U;
203 |     }
204 | }
205 | 
206 | static void u1w_refresh_target_current(void)
207 | {
208 |     u16 target_ma;
209 | 
210 |     target_ma = U1W_LIMIT_CURRENT(uart_1_wire.max_charge_current_ma);
211 | 
212 |     if((uart_1_wire.derate_current_ma != 0U) &&
213 |        (uart_1_wire.derate_current_ma < target_ma) &&
214 |        ((uart_1_wire.batt_temp_degc < uart_1_wire.derate_low_degc) ||
215 |         (uart_1_wire.batt_temp_degc > uart_1_wire.derate_high_degc)))
216 |     {
217 |         target_ma = uart_1_wire.derate_current_ma;
218 |     }
219 | 
220 |     if((target_ma != 0U) && (target_ma < 100U))
221 |     {
222 |         target_ma = 100U;
223 |     }
224 | 
225 |     uart_1_wire.target_current_ma = target_ma;
226 | }
227 | 
228 | /*============================== ���գ�ɨ��Ϸ�֡ ==============================*/
229 | 
230 | static void u1w_parse_frame(u8 *frame)
231 | {
232 |     u8 cmd;
233 |     u8 xy;
234 |     u8 key;
235 |     u32 pack_mv;
236 | 
237 |     cmd = frame[0];
238 |     uart_1_wire.last_error = U1W_ERR_NONE;
239 |     uart_1_wire.offline_count_10ms = 0U;
240 |     s_u1w.any_rx_age_10ms = 0U;
241 | 
242 |     key = U1W_KEY_MAX;
243 | 
244 |     switch(cmd)
245 |     {
246 |     case U1W_CMD_A0:
247 |         key = U1W_KEY_A0;
248 |         /* A0 ZZ XY CHK��ZZ=ʶ���룬X=�������룬Y=�������롣 */
249 |         uart_1_wire.pack_id = frame[1];
250 |         xy = frame[2];
251 |         uart_1_wire.cell_series = (u8)(((xy >> 4) & 0x0FU) + 5U);
252 |         uart_1_wire.cell_parallel = (u8)((xy & 0x0FU) + 1U);
253 | 
254 |         if((uart_1_wire.cell_series < 5U) || (uart_1_wire.cell_series > 20U))
255 |         {
256 |             uart_1_wire.cell_series = BAT_SERIES;
257 |         }
258 |         if((uart_1_wire.cell_parallel == 0U) || (uart_1_wire.cell_parallel > 16U))
259 |         {
260 |             uart_1_wire.cell_parallel = 1U;
261 |         }
262 |         break;
263 | 
264 |     case U1W_CMD_A1:
265 |         key = U1W_KEY_A1;
266 |         /* A1 XX YY CHK��YY=���� 0.1Ah�� */
267 |         uart_1_wire.cell_cap_01ah = frame[2];
268 |         break;
269 | 
270 |     case U1W_CMD_A4:
271 |         key = U1W_KEY_A4;
272 |         /* A4 xxxx yyyy CHK��xxxx=Ԥ���ֹ��yyyy=���䣬��λ0.01V�� */
273 |         uart_1_wire.cell_pre_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[1], frame[2]));
274 |         pack_mv = (u32)u1w_001v_to_mv(u1w_get_u16_bus(frame[3], frame[4])) *
275 |                   (u32)uart_1_wire.cell_series;
276 |         if(pack_mv > 0xFFFFUL)
277 |         {
278 |             pack_mv = 0xFFFFUL;
279 |         }
280 |         uart_1_wire.target_voltage_mv = U1W_LIMIT_VOLTAGE((u16)pack_mv);
281 |         break;
282 | 
283 |     case U1W_CMD_A6:
284 |         key = U1W_KEY_A6;
285 |         /* A6 xxxx yyyy CHK��xxxx=����о��������0.1A�� */
286 |         uart_1_wire.max_charge_current_ma =
287 |             U1W_LIMIT_CURRENT(u1w_cell_01a_to_pack_ma(u1w_get_u16_bus(frame[1], frame[2])));
288 |         u1w_refresh_target_current();
289 |         break;
290 | 
291 |     case U1W_CMD_A7:
292 |         key = U1W_KEY_A7;
293 |         /* A7 XX YY 00 ZZ CHK��XX/YY=����������ZZ=����о��������0.1A�� */
294 |         uart_1_wire.derate_low_degc = (s8)frame[1];
295 |         uart_1_wire.derate_high_degc = (s8)frame[2];
296 |         uart_1_wire.derate_current_ma = U1W_LIMIT_CURRENT(u1w_cell_01a_to_pack_ma(frame[4]));
297 |         u1w_refresh_target_current();
298 |         break;
299 | 
300 |     case U1W_CMD_B1:
301 |         key = U1W_KEY_B1;
302 |         /* B1 xxxx yyyy CHK��xxxx=��͵��ڣ�yyyy=��ߵ��ڣ���λ0.01V�� */
303 |         uart_1_wire.cell_min_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[1], frame[2]));
304 |         uart_1_wire.cell_max_mv = u1w_001v_to_mv(u1w_get_u16_bus(frame[3], frame[4]));
305 |         break;
306 | 
307 |     case U1W_CMD_B3:
308 |         key = U1W_KEY_B3;
309 |         /* B3 XX YY CHK��XX=����¶ȣ�YY=��� MOS �¶ȡ� */
310 |         uart_1_wire.batt_temp_degc = (s8)frame[1];
311 |         uart_1_wire.mos_temp_degc = (s8)frame[2];
312 |         u1w_refresh_target_current();
313 |         break;
314 | 
315 |     case U1W_CMD_B4:
316 |         key = U1W_KEY_B4;
317 |         /* B4 XX YY CHK��XX=SOC��YY=״̬λ��״̬λֻ��¼����ֱ�ӿ���B6�� */
318 |         uart_1_wire.soc_percent = frame[1];
319 |         if(uart_1_wire.soc_percent > 100U)
320 |         {
321 |             uart_1_wire.soc_percent = 100U;
322 |         }
323 |         uart_1_wire.charge_status = frame[2];
324 |         break;
325 | 
326 |     case U1W_CMD_B6:
327 |         /* B6 ���ԣ��յ��Ϸ�֡���ɡ� */
328 |         break;
329 | 
330 |     default:
331 |         break;
332 |     }
333 | 
334 |     if(key < U1W_KEY_MAX)
335 |     {
336 |         s_u1w.key_age_10ms[key] = 0U;
337 |         if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
338 |         {
339 |             uart_1_wire.handshake_mask |= (u8)(1U << key);
340 |         }
341 |     }
342 | 
343 |     if((uart_1_wire.handshake_mask & U1W_HANDSHAKE_MASK) == U1W_HANDSHAKE_MASK)
344 |     {
345 |         uart_1_wire.link_state = U1W_LINK_ONLINE;
346 |     }
347 |     else if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
348 |     {
349 |         uart_1_wire.link_state = U1W_LINK_HANDSHAKE;
350 |     }
351 | 
352 | }
353 | 
354 | /**
355 |   * @brief  �ӽ��ջ���ͷ��ɾ��ָ���ֽ�����
356 |   * @note   ���ڶ����ѽ���������֡��������֡ǰ�����Ч���ݡ�
357 |   */
358 | static void u1w_remove_rx_bytes(u8 count)
359 | {
360 |     u8 i;
361 | 
362 |     /* ɾ������������ǰ���泤�ȣ�ֱ����ջ��档 */
363 |     if(count >= s_u1w.rx_len)
364 |     {
365 |         s_u1w.rx_len = 0U;
366 |         return;
367 |     }
368 | 
369 |     /* ��ʣ������ǰ�ƣ��������滹û���������ݡ� */
370 |     for(i = 0U; i < (u8)(s_u1w.rx_len - count); i++)
371 |     {
372 |         s_rx_buf[i] = s_rx_buf[(u8)(i + count)];
373 |     }
374 | 
375 |     /* ���»�����Ч���ȡ� */
376 |     s_u1w.rx_len = (u8)(s_u1w.rx_len - count);
377 | }
378 | 
379 | /**
380 |   * @brief  һ��ͨ�Ž�������
381 |   * @note   ���� FIFO ���Ȳ��ٱ仯��ģ����տ��У��ٴӻ�����ɨ��Ϸ�֡��
382 |   */
383 | static void u1w_rx_task(void)
384 | {
385 |     u8 i;
386 |     u8 len;
387 |     u8 cmd;
388 |     u8 chk;
389 |     u8 fifo_cnt;
390 | 
391 |     /*
392 |      * ģ����տ����жϣ�
393 |      * FIFO �����ݣ������γ��Ⱥ��ϴβ�ͬ��˵�����ܻ��ڼ������գ��Ȳ�������
394 |      * �������� FIFO ������ͬ������Ϊ��һ�����������ꡣ
395 |      */
396 |     fifo_cnt = s_com_fifo.cnt;
397 |     if(fifo_cnt != 0U)
398 |     {
399 |         if(fifo_cnt != s_u1w.rx_fifo_last_cnt)
400 |         {
401 |             s_u1w.rx_fifo_last_cnt = fifo_cnt;
402 |             return;
403 |         }
404 |     }
405 |     s_u1w.rx_fifo_last_cnt = fifo_cnt;
406 | 
407 |     /* FIFO �����ȶ����ٰ����ݰᵽЭ����ջ��档 */
408 |     while(s_com_fifo.cnt != 0U)
409 |     {
410 |         if(s_u1w.rx_len >= COM_FRAME_LEN)
411 |         {
412 |             u1w_remove_rx_bytes(1U);
413 |         }
414 | 
415 |         s_rx_buf[s_u1w.rx_len] = com_fifo_pop();
416 |         s_u1w.rx_len++;
417 |     }
418 | 
419 |     /* FIFO �Ѱ�գ����ϴ� FIFO ���ȼ�¼�� */
420 |     s_u1w.rx_fifo_last_cnt = 0U;
421 | 
422 |     /*
423 |      * �ӽ��ջ�����ɨ��֡ͷ CMD��
424 |      * У��ɹ��ͽ�����
425 |      * У��ʧ�ܾʹ���һ�ֽڼ����ҡ�
426 |      */
427 |     i = 0U;
428 |     while(i < s_u1w.rx_len)
429 |     {
430 |         cmd = s_rx_buf[i];
431 |         len = u1w_reply_len(cmd);
432 | 
433 |         /* ������֪ CMD��������ǰ�ֽڡ� */
434 |         if(len == 0U)
435 |         {
436 |             i++;
437 |             continue;
438 |         }
439 | 
440 |         /*
441 |          * �ҵ�����֡ͷ����ʣ�೤�Ȳ�����
442 |          * �������֡ͷ���������ݣ��ȴ��´ν��ա�
443 |          */
444 |         if((u8)(s_u1w.rx_len - i) < len)
445 |         {
446 |             if(i != 0U)
447 |             {
448 |                 u1w_remove_rx_bytes(i);
449 |             }
450 |             return;
451 |         }
452 | 
453 |         /* �����㹻������У�顣 */
454 |         chk = u1w_sum(&s_rx_buf[i], (u8)(len - 1U));
455 |         if(chk == s_rx_buf[(u8)(i + len - 1U)])
456 |         {
457 |             /* �Ϸ�֡��������ɾ����֡��ǰ�����Ч���ݡ� */
458 |             u1w_dbg_rx_ok(&s_rx_buf[i], len);
459 |             u1w_parse_frame(&s_rx_buf[i]);
460 |             u1w_remove_rx_bytes((u8)(i + len));
461 |             i = 0U;
462 |             continue;
463 |         }
464 | 
465 |         /* ��ǰ CMD ��֡ͷ��У�鲻�ԣ�����һ�ֽڼ����ҡ� */
466 |         uart_1_wire.last_error = U1W_ERR_FRAME;
467 |         u1w_dbg_rx_bad(U1W_DBG_BAD_CHK, cmd, len, len, &s_rx_buf[i], chk);
468 |         i++;
469 |     }
470 | 
471 |     /* ɨ������󣬶����Ѿ�ȷ�����õ����ݡ� */
472 |     if(i != 0U)
473 |     {
474 |         u1w_remove_rx_bytes(i);
475 |     }
476 | }
477 | 
478 | /*============================== ���ͣ����׶���ѯ ==============================*/
479 | 
480 | /**
481 |   * @brief  ��װ������һ֡һ��ͨ�����
482 |   * @param  cmd Ҫ���͵������롣
483 |   * @retval 1�����ͳɹ���0������ʧ�ܡ�
484 |   */
485 | static bit u1w_send_frame(u8 cmd)
486 | {
487 |     u8 tx_len;
488 | 
489 |     /* ����֡�� 1 �ֽڶ��������롣 */
490 |     s_tx_buf[0] = cmd;
491 | 
492 |     if(cmd == U1W_CMD_A0)
493 |     {
494 |         /* A0 ֡��ʽ���⣺A0 00 ������ַ CHK�� */
495 |         s_tx_buf[1] = 0x00U;
496 |         s_tx_buf[2] = U1W_MASTER;
497 |         s_tx_buf[3] = u1w_sum(s_tx_buf, 3U);
498 |         tx_len = 4U;
499 |     }
500 |     else if(cmd == U1W_CMD_B6)
501 |     {
502 |         /*
503 |          * B6 ����������/��ʾ���
504 |          * ������ʾ�׶η��� SOC��
505 |          * �������׶ι̶�Ҫ��򿪳�� MOS��
506 |          */
507 |         if(s_u1w.stage == U1W_STAGE_FULL_DISPLAY)
508 |         {
509 |             s_tx_buf[1] = U1W_B6_TYPE_SOC;
510 |             s_tx_buf[2] = uart_1_wire.soc_percent;
511 |         }
512 |         else
513 |         {
514 |             s_tx_buf[1] = U1W_B6_TYPE_MOS;
515 |             s_tx_buf[2] = U1W_MOS_CHG_ON;  
516 |         }
517 | 
518 |         s_tx_buf[3] = u1w_sum(s_tx_buf, 3U);
519 |         tx_len = 4U;
520 |     }
521 |     else
522 |     {
523 |         /*
524 |          * ������ѯ����Ϊ 2 �ֽڣ�
525 |          * �� 1 �ֽڣ������룻
526 |          * �� 2 �ֽڣ�У��͡����� cmd + cmd = 0������У���ֽڵ��� cmd��
527 |          */
528 |         s_tx_buf[1] = cmd;
529 |         s_tx_buf[2] = 0U;
530 |         s_tx_buf[3] = 0U;
531 |         tx_len = 2U;
532 |     }
533 | 
534 |     /* ����ʧ��ʱ��¼���󣬽��������̺��������� */
535 |     if(com_uart_send_buf(s_tx_buf, tx_len) != BSP_OK)
536 |     {
537 |         uart_1_wire.last_error = U1W_ERR_SEND;
538 |         return 0;
539 |     }
540 | 
541 |     /* ���Դ�ӡ���������ݺ������ظ����ȡ� */
542 |     u1w_dbg_tx(s_tx_buf, tx_len, u1w_reply_len(cmd));
543 |     return 1;
544 | }
545 | 
546 | /**
547 |   * @brief  һ��ͨ�ŷ�������
548 |   * @note   ÿ 100ms ����ǰͨ�Ž׶η���һ֡�����ȴ��ظ���
549 |   */
550 | static void u1w_tx_task(void)
551 | {
552 |     u8 cmd;
553 |     u8 list_len;
554 | 
555 |     /*
556 |      * ֹͣ�׶Ρ��������� COM �׶Ρ�ͨ���ѳ�ʱ��
557 |      * ���ٷ���Э��֡�����ѷ��ͽ��ı���Ϊ���ѵ�ʱ�䡱��
558 |      */
559 |     if((s_u1w.stage == U1W_STAGE_STOP) ||
560 |        (s_u1w.stage == U1W_STAGE_PULL_LOW) ||
561 |        (uart_1_wire.comm_timeout != 0U))
562 |     {
563 |         s_u1w.tx_tick_10ms = U1W_TX_PERIOD_10MS;
564 |         return;
565 |     }
566 | 
567 |     /* 100ms ���ͽ���δ����������ʱ�� */
568 |     if(s_u1w.tx_tick_10ms < U1W_TX_PERIOD_10MS)
569 |     {
570 |         s_u1w.tx_tick_10ms++;
571 |         return;
572 |     }
573 | 
574 |     /* ���ĵ���׼�����ͱ��׶���һ֡�� */
575 |     s_u1w.tx_tick_10ms = 0U;
576 | 
577 |     if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
578 |     {
579 |         /* ���ֽ׶Σ�ѭ������ A0/A1/A4/A6/A7/B1/B3/B4�� */
580 |         list_len = (u8)ARRAY_SIZE(s_handshake_cmd);
581 |         if(s_u1w.tx_index >= list_len)
582 |         {
583 |             s_u1w.tx_index = 0U;
584 |         }
585 | 
586 |         cmd = s_handshake_cmd[s_u1w.tx_index];
587 |         s_u1w.tx_index++;
588 |     }
589 |     else if(s_u1w.stage == U1W_STAGE_TEMP_WAIT)
590 |     {
591 |         /* �¶��쳣�ȴ��׶Σ�ֻ��ѯ�¶�/״̬���֡�� */
592 |         list_len = (u8)ARRAY_SIZE(s_temp_wait_cmd);
593 |         if(s_u1w.tx_index >= list_len)
594 |         {
595 |             s_u1w.tx_index = 0U;
596 |         }
597 | 
598 |         cmd = s_temp_wait_cmd[s_u1w.tx_index];
599 |         s_u1w.tx_index++;
600 |     }
601 |     else if(s_u1w.stage == U1W_STAGE_FULL_DISPLAY)
602 |     {
603 |         /*
604 |          * ������ʾ�׶Σ����ڷ��� B6 ��ʾ SOC��
605 |          * ���� 3 ���Ӻ��������� COM��֪ͨ BMS ��������ͨ�š�
606 |          */
607 |         if(s_u1w.full_display_10ms >= U1W_FULL_DISPLAY_10MS)
608 |         {
609 |             u1w_pull_com_low();
610 |             return;
611 |         }
612 | 
613 |         cmd = U1W_CMD_B6;
614 |     }
615 |     else
616 |     {
617 |         /* �������׶Σ�ѭ������ B1/B3/B4/B6�� */
618 |         list_len = (u8)ARRAY_SIZE(s_charge_cmd);
619 |         if(s_u1w.tx_index >= list_len)
620 |         {
621 |             s_u1w.tx_index = 0U;
622 |         }
623 | 
624 |         cmd = s_charge_cmd[s_u1w.tx_index];
625 |         s_u1w.tx_index++;
626 |     }
627 | 
628 |     /* ����ѡ�е�����֡������ʧ���� u1w_send_frame() ��¼���� */
629 |     (void)u1w_send_frame(cmd);
630 | }
631 | 
632 | /*============================== ��ʱ���� =====================================*/
633 | 
634 | /**
635 |   * @brief  һ��ͨ�ż�ʱ����10ms ����һ�Ρ�
636 |   * @note   ����ͳ����ͨ�ų�ʱ���ؼ�֡��ʱ���Լ�������ʾ 3 ���Ӽ�ʱ��
637 |   */
638 | static void u1w_age_task_10ms(void)
639 | {
640 |     u8 i;
641 |     u8 start_key;
642 |     u8 end_key;
643 | 
644 |     /* ֹͣͨ�Ż��������� COM ʱ������ͳ��ͨ�ų�ʱ�� */
645 |     if((s_u1w.stage == U1W_STAGE_STOP) || (s_u1w.stage == U1W_STAGE_PULL_LOW))
646 |     {
647 |         return;
648 |     }
649 | 
650 |     /*
651 |      * ������ʾ�׶�ֻͳ����ʾʱ�䡣
652 |      * �� 3 ���Ӻ������ COM �������ɷ�����������
653 |      */
654 |     if(s_u1w.stage == U1W_STAGE_FULL_DISPLAY)
655 |     {
656 |         if(s_u1w.full_display_10ms < U1W_FULL_DISPLAY_10MS)
657 |         {
658 |             s_u1w.full_display_10ms++;
659 |         }
660 |         return;
661 |     }
662 | 
663 |     /* ����Ϸ�֡��ʱ�������յ�����Ϸ�֡���ڽ��ս��������㡣 */
664 |     if(s_u1w.any_rx_age_10ms < U1W_ANY_RX_TIMEOUT_10MS)
665 |     {
666 |         s_u1w.any_rx_age_10ms++;
667 |     }
668 |     uart_1_wire.offline_count_10ms = s_u1w.any_rx_age_10ms;
669 | 
670 |     /*
671 |      * ���ݵ�ǰ�׶Σ�ѡ����Ҫ��صĹؼ�֡��Χ��
672 |      * ���ֽ׶Σ�A0/A1/A4/A6/A7/B1/B3/B4 ��Ҫˢ�£�
673 |      * �¶ȵȴ���ֻ�� B3/B4��
674 |      * ���׶Σ�ֻ�� B1/B3/B4��
675 |      */
676 |     if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
677 |     {
678 |         start_key = U1W_KEY_A0;
679 |         end_key = U1W_KEY_MAX;
680 |     }
681 |     else if(s_u1w.stage == U1W_STAGE_TEMP_WAIT)
682 |     {
683 |         start_key = U1W_KEY_B3;
684 |         end_key = U1W_KEY_B4 + 1U;
685 |     }
686 |     else
687 |     {
688 |         start_key = U1W_KEY_B1;
689 |         end_key = U1W_KEY_B4 + 1U;
690 |     }
691 | 
692 |     /* ��ǰ�׶���Ҫ�Ĺؼ�֡��ʱ�� */
693 |     for(i = start_key; i < end_key; i++)
694 |     {
695 |         if(s_u1w.key_age_10ms[i] < U1W_KEY_RX_TIMEOUT_10MS)
696 |         {
697 |             s_u1w.key_age_10ms[i]++;
698 |         }
699 |     }
700 | 
701 |     /* 2 ����û���յ��κκϷ�֡���ж�ͨ�Ŷ�ʧ�� */
702 |     if(s_u1w.any_rx_age_10ms >= U1W_ANY_RX_TIMEOUT_10MS)
703 |     {
704 |         uart_1_wire.comm_timeout = 1U;
705 |         uart_1_wire.retry_over = 1U;
706 |         uart_1_wire.last_error = U1W_ERR_COMM_LOST;
707 |         uart_1_wire.key_timeout_cmd = 0U;
708 |         uart_1_wire.link_state = U1W_LINK_OFFLINE;
709 |         return;
710 |     }
711 | 
712 |     /* �ؼ�֡���� 3 ��δˢ�£�Ҳ�ж�ͨ���쳣������¼����һ֡��ʱ�� */
713 |     for(i = start_key; i < end_key; i++)
714 |     {
715 |         if(s_u1w.key_age_10ms[i] >= U1W_KEY_RX_TIMEOUT_10MS)
716 |         {
717 |             uart_1_wire.comm_timeout = 1U;
718 |             uart_1_wire.retry_over = 1U;
719 |             uart_1_wire.last_error = U1W_ERR_COMM_LOST;
720 |             uart_1_wire.link_state = U1W_LINK_OFFLINE;
721 | 
722 |             switch(i)
723 |             {
724 |             case U1W_KEY_A0: uart_1_wire.key_timeout_cmd = U1W_CMD_A0; break;
725 |             case U1W_KEY_A1: uart_1_wire.key_timeout_cmd = U1W_CMD_A1; break;
726 |             case U1W_KEY_A4: uart_1_wire.key_timeout_cmd = U1W_CMD_A4; break;
727 |             case U1W_KEY_A6: uart_1_wire.key_timeout_cmd = U1W_CMD_A6; break;
728 |             case U1W_KEY_A7: uart_1_wire.key_timeout_cmd = U1W_CMD_A7; break;
729 |             case U1W_KEY_B1: uart_1_wire.key_timeout_cmd = U1W_CMD_B1; break;
730 |             case U1W_KEY_B3: uart_1_wire.key_timeout_cmd = U1W_CMD_B3; break;
731 |             case U1W_KEY_B4: uart_1_wire.key_timeout_cmd = U1W_CMD_B4; break;
732 |             default:         uart_1_wire.key_timeout_cmd = 0U; break;
733 |             }
734 |             return;
735 |         }
736 |     }
737 | }
738 | 
739 | /*============================== ����ӿ� =====================================*/
740 | 
741 | void uart_1_wire_init(void)
742 | {
743 |     memclr(&uart_1_wire, sizeof(uart_1_wire));
744 |     memclr(&s_u1w, sizeof(s_u1w));
745 |     memclr(s_rx_buf, sizeof(s_rx_buf));
746 |     memclr(s_tx_buf, sizeof(s_tx_buf));
747 | 
748 |     uart_1_wire.link_state = U1W_LINK_OFFLINE;
749 |     uart_1_wire.last_error = U1W_ERR_NONE;
750 |     uart_1_wire.cell_series = BAT_SERIES;
751 |     uart_1_wire.cell_parallel = 1U;
752 |     uart_1_wire.cell_pre_mv = CELL_PRE_MV;
753 |     uart_1_wire.target_voltage_mv = SET_vMAX;
754 |     uart_1_wire.max_charge_current_ma = iMAX;
755 |     uart_1_wire.derate_current_ma = iMAX;
756 |     uart_1_wire.target_current_ma = iMAX;
757 |     uart_1_wire.derate_low_degc = 10;
758 |     uart_1_wire.derate_high_degc = 40;
759 | 
760 |     s_u1w.stage = U1W_STAGE_STOP;
761 |     s_u1w.tx_tick_10ms = U1W_TX_PERIOD_10MS;
762 | 
763 |     com_fifo_clear();
764 |     u1w_release_com();
765 | }
766 | 
767 | void uart_1_wire_reset_link(void)
768 | {
769 |     uart_1_wire_init();
770 | }
771 | 
772 | /**
773 |   * @brief  ����һ��ͨ�Ž׶Ρ�
774 |   * @note   �л��׶�ʱ�������÷��ͽ��ġ����ջ��桢��ʱ�����͹ؼ�֡��ʱ��
775 |   */
776 | void uart_1_wire_set_stage(u8 stage)
777 | {
778 |     u8 i;
779 | 
780 |     /* �Ƿ��׶�ͳһתΪֹͣͨ�ţ�����״̬Խ�硣 */
781 |     if(stage > U1W_STAGE_PULL_LOW)
782 |     {
783 |         stage = U1W_STAGE_STOP;
784 |     }
785 | 
786 |     /* �׶�δ�仯�����ظ������������Ӱ�쵱ǰͨ�š� */
787 |     if(s_u1w.stage == stage)
788 |     {
789 |         return;
790 |     }
791 | 
792 |     /* �л�ͨ�Ž׶κ����´ӱ��׶ε�һ֡��ʼ���͡� */
793 |     s_u1w.stage = stage;
794 |     s_u1w.tx_index = 0U;
795 |     s_u1w.tx_tick_10ms = U1W_TX_PERIOD_10MS;
796 |     s_u1w.rx_len = 0U;
797 | 
798 |     /* ��ͨ�ų�ʱ��ؼ����ͱ�־�� */
799 |     s_u1w.any_rx_age_10ms = 0U;
800 |     s_u1w.full_display_10ms = 0U;
801 |     uart_1_wire.offline_count_10ms = 0U;
802 |     uart_1_wire.comm_timeout = 0U;
803 |     uart_1_wire.retry_over = 0U;
804 |     uart_1_wire.key_timeout_cmd = 0U;
805 | 
806 |     /* ����ؼ�֡ˢ�¼�ʱ�� */
807 |     for(i = 0U; i < U1W_KEY_MAX; i++)
808 |     {
809 |         s_u1w.key_age_10ms[i] = 0U;
810 |     }
811 | 
812 |     if(stage == U1W_STAGE_HANDSHAKE)
813 |     {
814 |         /* ��������ʱ�������������� A0/A1/A4/A6/A7/B1/B3/B4�� */
815 |         uart_1_wire.handshake_mask = 0U;
816 |         uart_1_wire.link_state = U1W_LINK_HANDSHAKE;
817 |         u1w_release_com();
818 |     }
819 |     else if(stage == U1W_STAGE_STOP)
820 |     {
821 |         /* ֹͣͨ�ţ��ͷ� COM�����������͡� */
822 |         uart_1_wire.link_state = U1W_LINK_OFFLINE;
823 |         u1w_release_com();
824 |     }
825 |     else if(stage == U1W_STAGE_PULL_LOW)
826 |     {
827 |         /* �������� COM��֪ͨ BMS ��������ͨ�š� */
828 |         uart_1_wire.link_state = U1W_LINK_OFFLINE;
829 |         u1w_pull_com_low();
830 |     }
831 |     else
832 |     {
833 |         /* ����ͨ�Ž׶���Ҫ�ͷ� COM�������������շ��� */
834 |         u1w_release_com();
835 |     }
836 | 
837 |     /* �׶��л��󣬶����ɻ��棬������һ�׶β�������Ӱ���½׶Ρ� */
838 |     com_fifo_clear();
839 | }
840 | 
841 | static void u1w_refresh_info(void)
842 | {
843 |     u1w_info.stage = s_u1w.stage;
844 |     u1w_info.handshake_ok = ((uart_1_wire.handshake_mask & U1W_HANDSHAKE_MASK) == U1W_HANDSHAKE_MASK) ? 1U : 0U;
845 |     u1w_info.comm_timeout = uart_1_wire.comm_timeout;
846 |     u1w_info.key_timeout_cmd = uart_1_wire.key_timeout_cmd;
847 |     u1w_info.soc_percent = uart_1_wire.soc_percent;
848 |     u1w_info.charge_status = uart_1_wire.charge_status;
849 |     u1w_info.batt_temp_degc = uart_1_wire.batt_temp_degc;
850 |     u1w_info.mos_temp_degc = uart_1_wire.mos_temp_degc;
851 |     u1w_info.target_voltage_mv = U1W_LIMIT_VOLTAGE(uart_1_wire.target_voltage_mv);
852 |     u1w_info.target_current_ma = U1W_LIMIT_CURRENT(uart_1_wire.target_current_ma);
853 |     u1w_info.cell_max_mv = uart_1_wire.cell_max_mv;
854 |     u1w_info.no_rx_10ms = s_u1w.any_rx_age_10ms;
855 | }
856 | 
857 | /**
858 |   * @brief  һ��ͨ�� 10ms ��������
859 |   * @note   ������ÿ 10ms ����һ�Σ��ڲ���ɽ��ա���ʱ��ʱ�����ڷ��͡�
860 |   */
861 | void uart_1_wire_poll_10ms(void)
862 | {
863 |     /*
864 |      * �������� COM �׶Σ�
865 |      * �������� COM �͵�ƽ�������շ�Э��֡��
866 |      */
867 |     if(s_u1w.stage == U1W_STAGE_PULL_LOW)
868 |     {
869 |         u1w_pull_com_low();
870 |         u1w_refresh_info();
871 |         return;
872 |     }
873 | 
874 |     /* �ȴ������գ��Ϸ�֡����ͨ�ż�ʱ������Э�����ݡ� */
875 |     u1w_rx_task();
876 | 
877 |     /* �ٴ�����ʱ��ʱ���ж���ͨ�Ż�ؼ�֡�Ƿ�ʱ�� */
878 |     u1w_age_task_10ms();
879 | 
880 |     /* ��󰴵�ǰ�׶κ� 100ms ���ķ�����һ֡�� */
881 |     u1w_tx_task();
882 | 
883 |     /* ���¸� ch.c ֱ�Ӷ�ȡ��ͨ�ſ��ա� */
884 |     u1w_refresh_info();
885 | }
886 | 
887 | void uart_1_wire_task_10ms(void)
888 | {
889 |     uart_1_wire_poll_10ms();
890 | }
891 | 
892 | 
```

### YAT-60V3A-Uart-1-wire/App/uart_1_wire.h

Bytes: 5090
SHA-256: 9234d9761cb49ce4e869bfe6416d108d9c8710f896c27c65074a55d6c891e62a
Lines: 1-137 of 137

```text
  1 | /**
  2 |   ******************************************************************************
  3 |   * @file    uart_1_wire.h
  4 |   * @brief   P30 ���� UART ͨ��Э��ӿڣ��첽��ѯ�档
  5 |   *
  6 |   * ��Ʊ߽磺
  7 |   * 1. uart_1_wire.c ֻ����Э���շ���֡ɨ�衢У�顢������ͨ�ų�ʱ��
  8 |   * 2. ch.c ֻ����ͨ�Ž׶Σ��������������/ͨ�ų�ʱ���Ƴ�����̡�
  9 |   * 3. ���Ͳ��١���һ֡��һ֡�ظ��������ǰ��׶�ÿ 100ms ��ѯ���͡�
 10 |   * 4. ���մӻ�����ɨ����֪ CMD��CMD + ���� + CHK ��ȷ�Ÿ������ݡ�
 11 |   ******************************************************************************
 12 |   */
 13 | #ifndef __UART_1_WIRE_H__
 14 | #define __UART_1_WIRE_H__
 15 | 
 16 | #include "common.h"
 17 | 
 18 | /* �����֣�ֻ��������������õ������ */
 19 | #define U1W_CMD_A0                         (0xA0U)    /* ��ѯ�ӻ����� */
 20 | #define U1W_CMD_A1                         (0xA1U)    /* ��ѯ��о�������� */
 21 | #define U1W_CMD_A4                         (0xA4U)    /* ��ѯԤ��/�����ѹ */
 22 | #define U1W_CMD_A6                         (0xA6U)    /* ��ѯ����ŵ���� */
 23 | #define U1W_CMD_A7                         (0xA7U)    /* ��ѯ�������������� */
 24 | #define U1W_CMD_B1                         (0xB1U)    /* ��ѯ���/��ߵ��ڵ�ѹ */
 25 | #define U1W_CMD_B3                         (0xB3U)    /* ��ѯ����¶�/MOS�¶� */
 26 | #define U1W_CMD_B4                         (0xB4U)    /* ��ѯSOC�ͳ��״̬ */
 27 | #define U1W_CMD_B6                         (0xB6U)    /* ���Ƶ�ذ� MOS */
 28 | 
 29 | #ifndef U1W_MASTER
 30 | #define U1W_MASTER                         (0x10U)    /* A0 �������ͣ������ */
 31 | #endif
 32 | 
 33 | #define U1W_B6_TYPE_MOS                    (0x01U)    /* B6��MOS���� */
 34 | #define U1W_B6_TYPE_SOC                    (0x03U)    /* B6��������ʾSOC */
 35 | #define U1W_MOS_CHG_ON                     (0x01U)    /* B6��Ҫ��򿪳��MOS */
 36 | 
 37 | /* B4 ״̬λ����ǰ�汾ֻ��¼����������Щλ���� B6 ���ء� */
 38 | #define U1W_B4_OV                          (0x01U)
 39 | #define U1W_B4_LOW_TEMP                    (0x02U)
 40 | #define U1W_B4_HIGH_TEMP                   (0x04U)
 41 | #define U1W_B4_MOS_HOT                     (0x08U)
 42 | #define U1W_B4_OCP                         (0x10U)
 43 | #define U1W_B4_SHORT                       (0x20U)
 44 | #define U1W_B4_TIMEOUT                     (0x40U)
 45 | #define U1W_B4_FAIL                        (0x80U)
 46 | #define U1W_B4_FAULT_MASK                  (0xFFU)
 47 | 
 48 | /* �����롣 */
 49 | #define U1W_ERR_NONE                       (0x00U)
 50 | #define U1W_ERR_FRAME                      (0x01U)
 51 | #define U1W_ERR_TIMEOUT                    (0x02U)    /* ���ݵ��Դ�ӡ */
 52 | #define U1W_ERR_RETRY_OVER                 (0x03U)    /* ���ݵ��Դ�ӡ */
 53 | #define U1W_ERR_BUS_LOW                    (0x04U)    /* ���ݵ��Դ�ӡ */
 54 | #define U1W_ERR_COMM_LOST                  (0x05U)
 55 | #define U1W_ERR_SEND                       (0x06U)
 56 | #define U1W_ERR_NOT_READY                  (0x07U)    /* ���ݵ��Դ�ӡ */
 57 | 
 58 | /* ch.c ����ͨ�Ų㴦���ĸ��׶Ρ� */
 59 | typedef enum
 60 | {
 61 |     U1W_STAGE_STOP = 0U,                    /* ��ͨ�ţ��ͷ� COM */
 62 |     U1W_STAGE_HANDSHAKE,                    /* ��ѯ A0/A1/A4/A6/A7/B1/B3/B4 */
 63 |     U1W_STAGE_CHARGE,                       /* ��ѯ B1/B3/B4/B6��B6Ҫ��MOS */
 64 |     U1W_STAGE_FULL_DISPLAY,                 /* �������B6 03 SOC��3���Ӻ�����COM */
 65 |     U1W_STAGE_TEMP_WAIT,                    /* �¶��쳣�ȴ��ָ���������ѯB3/B4 */
 66 |     U1W_STAGE_PULL_LOW                      /* ��������COM����֪BMS�����Ͽ� */
 67 | } UART_1WIRE_STAGE_Types;
 68 | 
 69 | typedef enum
 70 | {
 71 |     U1W_LINK_OFFLINE = 0U,
 72 |     U1W_LINK_HANDSHAKE,
 73 |     U1W_LINK_ONLINE
 74 | } UART_1WIRE_LINK_Types;
 75 | 
 76 | /* Э���������������Ժ���������㷨��ȡ�� */
 77 | typedef struct
 78 | {
 79 |     u8  link_state;
 80 |     u8  last_error;
 81 |     u8  comm_timeout;                       /* 1=��ͨ�Ż�ؼ�֡��ʱ */
 82 |     u8  key_timeout_cmd;                    /* �����ʱ�Ĺؼ�֡CMD��0��ʾ�ܳ�ʱ */
 83 |     u8  handshake_mask;                     /* A0/A1/A4/A6/A7/B1/B3/B4 �����־ */
 84 |     u8  retry_over;                         /* ���ݾɵ��Ժ��壺ͨ���ѳ�ʱ */
 85 | 
 86 |     u8  pack_id;
 87 |     u8  cell_series;
 88 |     u8  cell_parallel;
 89 |     u8  soc_percent;
 90 |     u8  charge_status;
 91 | 
 92 |     s8  batt_temp_degc;
 93 |     s8  mos_temp_degc;
 94 |     s8  derate_low_degc;
 95 |     s8  derate_high_degc;
 96 | 
 97 |     u16 cell_cap_01ah;
 98 |     u16 cell_pre_mv;
 99 |     u16 cell_min_mv;
100 |     u16 cell_max_mv;
101 |     u16 target_voltage_mv;
102 |     u16 max_charge_current_ma;
103 |     u16 derate_current_ma;
104 |     u16 target_current_ma;
105 | 
106 |     u16 offline_count_10ms;                 /* �����ϴκϷ�֡��ʱ�� */
107 | } UART_1WIRE_INFO_Types;
108 | 
109 | /* ch.c ÿ 10ms ��ȡһ��������գ���ֱ�ӹ���ͨ���ڲ������� */
110 | typedef struct
111 | {
112 |     u8  stage;
113 |     u8  handshake_ok;
114 |     u8  comm_timeout;
115 |     u8  key_timeout_cmd;
116 |     u8  soc_percent;
117 |     u8  charge_status;
118 |     s8  batt_temp_degc;
119 |     s8  mos_temp_degc;
120 |     u16 target_voltage_mv;
121 |     u16 target_current_ma;
122 |     u16 cell_max_mv;
123 |     u16 no_rx_10ms;                         /* �����ϴκϷ�֡��ʱ�䣬������аε���ж� */
124 | } UART_1WIRE_CHARGE_INFO_Types;
125 | 
126 | extern UART_1WIRE_INFO_Types xdata uart_1_wire;
127 | extern UART_1WIRE_CHARGE_INFO_Types idata u1w_info;
128 | 
129 | void uart_1_wire_init(void);
130 | void uart_1_wire_reset_link(void);
131 | void uart_1_wire_set_stage(u8 stage);
132 | void uart_1_wire_poll_10ms(void);
133 | void uart_1_wire_task_10ms(void);           /* ���ݾɽӿڣ���ͬ�� poll_10ms */
134 | 
135 | 
136 | #endif
137 | 
```

## Skipped Files

None.
