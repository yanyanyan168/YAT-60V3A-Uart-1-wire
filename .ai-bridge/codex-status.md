# Codex Status

更新时间: 2026-06-27

## 当前状态
- 已先提交当前工作区，提交: 2574a0a checkpoint before plan execution
- 当前分支: master
- 工作区: 提交后干净
- 已读取: AGENTS.md, USER.md, .ai-bridge/current-plan.md
- 已读取参考文档: 一线通信代码实现思路.md, 一线通信异步轮询协议重构总结.md

## CodeGraph
- 检测到 .codegraph
- codegraph status 失败: PowerShell 执行策略阻止脚本；cmd /c codegraph status 报 unable to open database file
- 处理方式: 暂按计划使用 rg 和源码人工复核，修改前输出影响范围判断

## 当前计划步骤
1. 修正充电中拔电池判定: 1s 无通信且电流小于 iGED 后，先关输出，等待约 20ms，再用 vout < vRESET 确认拔电池，否则进入 BMS_ERR。
2. 检查并补齐 CH_IDLE 低压电池插入稳定确认，避免残压下降或插入瞬态误判。
3. 复核 B4 状态分类、FULL_DISPLAY、复充、LED 1Hz 是否符合 current-plan。
4. 每步修改后运行 Keil 编译/链接验证，并记录 code size。

## 风险与约束
- 严格遵守 ENCODING_GUARD + GIT_GUARD。
- 不改串口收发流程为同步等待。
- 不默认打开协议日志。
- 不引入大宏、大日志、大字符串，优先小代码。
## Step 1 结果: 充电中拔电池判定
- 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
- 修改点: `val.curr < iGED` 且 `no_rx_10ms >= 100` 后先关输出，约 20ms 后检查 `val.vout < vRESET`。
- 正常拔出: 复位一线链路并回到 CH_IDLE。
- 非正常拔出: 进入 BMS_ERR。
- 编码: ch.c 保持 CP936, 无 BOM, LF。
- 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
- Program Size: data=155.4 xdata=315 const=146 code=14713。
- 备注: Git 提示该文件受 autocrlf 影响，下次 Git 触碰可能转换 CRLF；本次写回仍保持 LF。
## Step 2 结果: CH_IDLE 低压插入稳定确认
- 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
- 修改点: 待机时 `vRESET < val.vout < vSTART` 不立即报错，需连续约 2s 且相邻 10ms 变化不超过 100mV。
- 正常插入: `val.vout >= vSTART` 仍按原 50 次确认后进 BMS_HANDSHAKE。
- 残压放空: `val.vout <= vRESET` 清低压候选，保持 CH_IDLE。
- 低压确认: 进入 CH_UVP，输出保持关闭。
- 编码: ch.c 保持 CP936, 无 BOM, LF。
- 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
- Program Size: data=157.4 xdata=315 const=146 code=14893。
## Step 3 结果: B4 分类、满电显示、LED、协议禁项复核
- 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
- B4 bit0: 进入 CH_FULL，等效满电处理。
- B4 bit1/2/3: 进入 BMS_TEMP_ERR，输出关闭，继续轮询 B3/B4，不主动拉低 COM。
- B4 bit4/5/6/7: 进入 BMS_ERR，输出关闭，通信阶段为 PULL_LOW。
- BMS_TEMP_ERR 中若后续出现 bit4~7，也会转 BMS_ERR。
- FULL_DISPLAY: 保持协议层发送 B6 03 SOC，3 分钟后拉低 COM；未改 uart_1_wire.c 收发流程。
- 复充: FULL_DISPLAY 阶段不轮询 B1，当前仍用整包 vCH60 近似判断，并已在代码注释说明。
- LED: Red_Flash/Ged_Flash/RGed_Flash(50) 在 10ms 调度下 500ms 翻转一次，完整周期约 1Hz；低压/普通异常为红灯约 1Hz。
- 协议日志: U1W_DEBUG_EN=0, U1W_DEBUG_PARSE_EN=0，默认关闭。
- 禁项复核: 未新增 A2/A3/A5/B0/B2/B5/B7/B8；未恢复 wait_cmd/wait_len/wait_start_ms/retry_count/busy/can_send 同步等待模型。
- 编码: ch.c 保持 CP936, 无 BOM, LF。
- 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
- Program Size: data=157.4 xdata=315 const=146 code=14940。
## Final Check
- 最终 Keil Build: 0 Error(s), 0 Warning(s)。
- 最终 Program Size: data=157.4 xdata=315 const=146 code=14940。
- 16K code 上限 0x3F80/16256 字节，当前剩余约 1316 字节。
- ch.c 编码: CP936/GBK, 无 BOM, LF, 未发现 emoji。
- codex-status.md 编码: UTF-8 无 BOM, LF。
- implementation-diff.patch 已生成。
- Keil 构建更新了 Project/Keil/TXLinkLog.txt，未擅自回退。
## Remove Return Fix
- 计划来源: 重新读取 .ai-bridge/current-plan.md，目标为修复充电中拔除等待期间重新开输出问题。
- 修改文件: YAT-60V3A-Uart-1-wire/App/ch.c
- CodeGraph: 已 sync，索引最新；目标函数 ch_battery_removed_check_10ms 只有 usr_ch_func 一个上游调用者。
- 修复点: 疑似拔除启动后立即 return 1，让主循环 continue，避免本轮继续执行 CH_REPAIR/CH_Pre1/CH_CCCV 重新开输出。
- 修复点: 20ms 等待期间每轮 ch_output_all_off() 后 return 1，后续状态逻辑不会运行。
- 修复点: 20ms 后 vout >= vRESET 进入 BMS_ERR 后 return 1，不再继续执行后续充电逻辑。
- 检查: 未发现 iGED / 2 作为充电中拔除阈值。
- 编码: ch.c 保持 CP936/GBK, 无 BOM, LF, 未发现 emoji。
- 验证: Keil Build 通过，0 Error(s), 0 Warning(s)。
- Program Size: data=157.4 xdata=315 const=146 code=14957，距 16K 上限约 1299 字节。
## Current Plan Run: Remove Wait + C51 Chinese FD Check
- 计划来源: 已重新读取 .ai-bridge/current-plan.md，本次以该计划为准。
- 任务1结果: 已确认 ch_battery_removed_check_10ms 修复疑似拔除后 return 0 导致后续充电状态重新开输出的问题。
- 疑似拔除启动: `val.curr < iGED` 且 `no_rx_10ms >= 100U` 后，立即 ch_output_all_off(), s_remove_cnt=1U, return 1。
- 20ms 等待: s_remove_cnt != 0U 分支每轮先 ch_output_all_off()，未到 20ms return 1。
- 20ms 后: `val.vout < vRESET` 回 CH_IDLE；`val.vout >= vRESET` 进入 BMS_ERR；两条路径都 return 1，不继续执行后续充电逻辑。
- 检查: 未发现 `iGED / 2` 作为充电中拔除阈值。
- 任务2结果: 已检查 uart_printf/printf/pc_uart_print_current_line/u1w_dbg_put_str 等运行时中文打印字符串中的 C51 0xFD/\xFD 问题。
- 已补 \xFD: YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c 三处 `" 数据="` 改为 `" 数据\xFD="`。
- 已确认保留中文，未改英文/ASCII，未修改普通中文注释。
- 复扫结果: 运行时打印里 GBK 第二字节为 FD 的中文位置均已有 \xFD；pc_uart.c 原有 `过\xFD流恢复`、`放大倍数\xFD` 保持不动。
- 默认协议日志: U1W_DEBUG_EN=0, U1W_DEBUG_PARSE_EN=0，未打开。
- CodeGraph: 已 sync；目标函数唯一上游为 usr_ch_func，影响范围集中在 ch.c 状态机接管逻辑。
- Keil Build: 0 Error(s), 0 Warning(s)。
- Program Size: data=157.4 xdata=315 const=146 code=14957；按 16K 上限 16256 计算，剩余约 1299 字节。
- 编码: ch.c 与 uart_1_wire_debug.c 均保持 CP936/GBK、无 BOM、LF，未发现 emoji。
## 2026-06-28 本轮进度: uart_1_wire 握手标志修复
- 工作目录: D:\Users\CODE。
- 已按要求先读取 YAT-60V3A-Uart-1-wire/memory.md，再读取 .ai-bridge/current-plan.md。
- 已只回退 App/uart_1_wire.c 中 ChatGPT 刚才新增的 u1w_mark_valid_frame(u8 cmd) 及相关调用结构；未回退其它源码文件。
- 最终 App/uart_1_wire.c 只保留最小补丁: handshake_mask 只在 s_u1w.stage == U1W_STAGE_HANDSHAKE 时更新。
- 校验失败帧不会调用 u1w_parse_frame()，因此不会更新 handshake_mask；长度不够、未知 CMD 也不会更新 handshake_mask。
- 非 HANDSHAKE 阶段收到合法帧只刷新数据和 key_age，不再增加 handshake_mask。
- App/uart_1_wire.c 保持 CP936/GBK、无 BOM、CRLF；未新增中文注释，未批量格式化，未批量转码。
- 按用户最新规则，后续默认 U1W_DEBUG_EN=1U、U1W_DEBUG_PARSE_EN=1U，用于查看最大 CODE 占用；U1W_DEBUG_CMD_FILTER=0U；需要改 0 由用户自己改。
- 未改 pc_uart；未改 DBG_BAUD=115200UL；未改 Flash/EEPROM 布局。
- Keil Build 命令: D:\Keil_v5\UV4\UV4.exe -b Project.uvproj -j0。
- Keil Build 结果: 0 Error(s), 0 Warning(s)。
- Program Size: data=163.4 xdata=315 const=146 code=15784。
- 按 code + const + 128 计算: 15784 + 146 + 128 = 16058，小于 16384，剩余约 326 字节。
- 已将“后续默认调试打开，需要改 0 由用户自己改”的规则写入 YAT-60V3A-Uart-1-wire/memory.md。
- memory.md 为未跟踪文件，写入前已备份: YAT-60V3A-Uart-1-wire/memory.md.bak_encoding_20260628_105912。
- 真机风险: 仍需实测坏校验帧不会推进握手完成、HANDSHAKE 完成后正常切阶段、打开一线日志时串口时序是否影响现场通信。
