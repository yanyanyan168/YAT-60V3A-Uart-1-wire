# memory.md：YAT-60V3A-Uart-1-wire 项目接续记忆

更新时间：2026-06-28

本文是后续新话题、ChatGPT、CODEX、人工维护者继续本工程时的固定入口。它不是普通总结，而是按本话题发生顺序整理的事后复盘和接手说明。读它的人应该能理解这个话题从哪里开始、为什么这样改、现在做到哪里、后续不能踩哪些坑。

## 1. 接手前先读这一节

继续本工程时，先读 memory.md，再读 .ai-bridge/current-plan.md，再用 CodexPro.show_changes 看当前 diff，最后按需读源码和 Keil build log。不要只看聊天末尾，也不要只听 CODEX 口头总结。

当前工程目录是 D:/Users/CODE/YAT-60V3A-Uart-1-wire，workspace_id 是 ws_4eb4de4a7c358b9a6f97f743。这个话题围绕 YAT 工程，不是 SC0479 MQTT，也不是 iOS App。后续不要把别的项目上下文混进来。

接手后必须先复述当前状态：

1. 当前后续协作默认一线调试日志打开：U1W_DEBUG_EN=1、U1W_DEBUG_PARSE_EN=1，用于查看最大 CODE 占用；需要改 0 时由用户自己改。
2. App/uart_1_wire_debug.c 和 App/uart_1_wire_debug.h 是 UTF-8 无 BOM。
3. 这两个文件可以中文注释，但运行时日志字符串只用 ASCII。
4. 一线日志现在是可读短日志，例如 1W B4[SOC] TX=... RX=... OK。
5. 临时全开日志能编译过，但按 code + const + 128 计算后只剩约 332B。
6. ch.c 中充电中疑似拔电池逻辑已检查，但真机还要验证 20ms 后 vout 是否小于 1V。
7. 后续不要把高频一线日志做重，不要再塞 SOC、电流、电压、温度等十进制解析值。

## 2. 用户习惯和协作方式

用户是嵌入式固件背景，关注的是能不能落地、能不能编译、现场能不能测。回答要直接，不要拍马屁，不要空泛说可以。需要选择时用 1/2/3 或 A/B/C，方便用户直接回复。

用户非常反感胡诌，尤其反感没有看源码、没有看 diff、没有看 build log 就下结论。以后回答这类工程问题，必须把依据说清楚：看了哪个文件、改了哪里、编译结果是什么、剩多少空间、还有什么风险。

用户偏好的代码风格是小改、清楚、可读、注释足够，不炫技，不做大重构，不搞复杂宏系统，不批量格式化，不批量转码。对 C51 这种空间紧张工程，宁愿朴素、可控，也不要漂亮但占空间的方案。

本话题里用户多次纠正了方向：不能只看 code，要算 const 和 EEPROM 预留；uart_printf 已经在工程里用了，不能说它是第一次引入；极简日志 U A0 T... R... OK 看不懂；文件名应按 memory.md 规则，而不是随便起中文文件名；memory.md 内容必须连贯，不是条目堆叠。这些纠正以后都要记住。

## 3. 本话题是怎么开始的

本话题前半段不是改日志，而是先处理 YAT 工程里充电中拔电池判断。用户关心的是充电状态下如果电池被拔掉，不能让后续充电逻辑重新打开输出，也不能误判。

围绕 ch.c 的 ch_battery_removed_check_10ms() 检查后，当前逻辑可以概括为：如果通信已经超过 1 秒没有收到一线回复，并且电流低于 iGED，就认为疑似拔电池。首次满足条件时立即关输出，设置 s_remove_cnt=1，并返回 1。返回 1 后 usr_ch_func() 会 continue，跳过后续充电状态逻辑，所以不会被后续流程重新打开输出。

进入疑似拔除等待期后，每个 10ms 周期仍继续 ch_output_all_off，直到 s_remove_cnt 到 3，约等于等待 20ms。到点后看 val.vout：如果 val.vout < vRESET，认为正常拔电池，关闭分压、重置一线链路、重置 vout 探测并回 CH_IDLE；如果 val.vout >= vRESET，认为异常拔出，进入 BMS_ERR。

这个逻辑的关键点是：通信中断大于 1 秒 + 电流小于 iGED，先关输出，再等约 20ms，再看 vout。已确认当前没有继续使用 iGED/2 作为充电中拔除阈值。

这部分虽然代码层面已检查，但不能说完全没风险。真机还必须验证拔电池后 20ms 内 vout 是否能降到 1V 以下，异常拔出是否进 BMS_ERR，低压插入 2 秒稳定判断以及 B4 故障分类是否符合现场预期。

## 4. 为什么后来转到一线调试日志

ch.c 逻辑检查后，问题转向一线 UART / 1-wire 协议调试。用户需要在调试时看清楚一线协议的 TX/RX 原始帧，但工程是 16K C51 项目，空间非常紧。原来的中文长日志和多行日志不适合长期保留，既占空间，也会影响串口时间。

最开始为了省空间，曾考虑过很短的日志形式，例如 U A0 T:00001212 R:00112233 OK。用户明确反馈这种太简陋，看不懂。于是方案改为可读短日志：保留 1W 前缀、命令字、命令短名、TX、RX、OK/BAD/ERR/STOP，例如 1W B4[SOC] TX=B400 RX=B46403C8 OK。

这个阶段的原则是：日志要比中文长日志短，但不能短到用户看不懂。命令短名 A0[ID]、B4[SOC] 是可读性和空间之间的折中，后续不要删掉。

## 5. 曾经走错的一步：十进制解析值太重

为了让日志更直观，曾尝试在成功日志后追加十进制解析值，例如 SOC=100、I=3000mA、V=60000mV、BAT=25C、CAP=52.0Ah。这个版本虽然好看，但引入了 u16 十进制打印、s8 有符号打印、0.1Ah 小数打印、单位字符串和大量 switch 分支。

用户上传的 Keil build log 显示旧版全开失败：Program Size 为 data=160.4 xdata=379 const=146 code=16380，并报 ERROR L107: ADDRESS SPACE OVERFLOW。按本工程实际 Flash 布局，必须计算 code + const + 128。旧版就是 16380 + 146 + 128 = 16654，超过 16384 约 270B，所以不能用。

因此最终删除了这些重型解析值，只保留命令短名和原始 TX/RX 16 进制。具体电池电压、电流、SOC、温度等值，后续走 pc_uart 按需查询，不塞回一线高频日志。

## 6. 当前 uart_1_wire_debug.c 的设计

App/uart_1_wire_debug.c 当前是 UTF-8 无 BOM。中文只用于注释，运行时字符串只用 ASCII。

现在 u1w_dbg_tx() 只缓存最近一次发送帧，不立即打印。这样成功时可以把 TX 和 RX 合并成一行，避免发送一行、接收一行、解析一行导致刷屏。u1w_dbg_rx_ok() 在收到合法回复后打印一行 1W CMD[NAME] TX=xxxx RX=xxxx OK。u1w_dbg_parse_ok() 不再单独打印，避免成功帧重复。

u1w_dbg_rx_bad() 用于坏帧，保留 RX 原始数据和必要错误细节，例如 CALC、LEN、CMD。u1w_dbg_fail() 打印 ERR，例如 TIMEOUT、RETRY、BUS_LOW 等。u1w_dbg_stop() 打印 STOP 原因。

为了节省空间，TX 只缓存 4 字节；RX 不再单独缓存，收到时直接打印。已经删除 u1w_dbg_put_u16_dec、u1w_dbg_put_s8_dec、u1w_dbg_put_01ah、u1w_dbg_put_parse。不要把它们加回来。

当前成功日志示例：

    1W A0[ID] TX=00001212 RX=00112233 OK
    1W A6[ISET] TX=A600 RX=A6123456 OK
    1W B3[TEMP] TX=B300 RX=B3192A OK
    1W B4[SOC] TX=B400 RX=B46403C8 OK

当前异常日志示例：

    BAD A0[ID] RX=00112233 CALC=55
    BAD A0[ID] RX=00112233 LEN=03/04
    ERR B4[SOC] TIMEOUT
    STOP BUS_LOW

## 7. 当前 uart_1_wire_debug.h 的设计

App/uart_1_wire_debug.h 当前也是 UTF-8 无 BOM。它补了中文注释，包括总开关、解析开关、命令过滤、坏帧原因、每个调试接口，以及关闭日志时空宏说明。

当前用户最新规则：后续先默认打开一线调试，用于直接观察最大 CODE 占用。不要再擅自把默认值改回 0；如果需要关闭，由用户自己改。

    U1W_DEBUG_EN = 1
    U1W_DEBUG_PARSE_EN = 1
    U1W_DEBUG_CMD_FILTER = 0

如果只看某个命令，例如 B4，可以把 U1W_DEBUG_CMD_FILTER 设为 U1W_CMD_B4。注意：U1W_DEBUG_PARSE_EN 现在保留配置意义，但不会恢复 SOC/I/V/TEMP 这些十进制解析值。

## 8. 命令短名必须保留

当前命令短名如下：

    A0 -> ID      电池识别 / 串并数
    A1 -> CAP     容量
    A4 -> VSET    目标 / 预充电压
    A6 -> ISET    最大充电电流
    A7 -> TDER    温度降额
    B1 -> CELLV   单体最低 / 最高电压
    B3 -> TEMP    电池 / MOS 温度
    B4 -> SOC     SOC / 状态位
    B6 -> MOS     MOS 控制或满电显示

这些短名是用户可读性的关键。不要再退回 U A0 T... 这种格式。

## 9. 16K 空间计算规则

本工程不能只看 Keil Program Size 的 code。必须按 code + const + 128 计算。128B 来自 0x3F80 到 0x3FFF 的模拟 EEPROM / 校准区预留。

相关文件包括 Bsp/flash_layout.h、Bsp/flash.c、App/cal.c、App/cal.h。Flash 布局是 0x0000 到 0x3F7F 为 Code 区，0x3F80 到 0x3FFF 为校准参数区。

旧版全开失败计算：

    16380 + 146 + 128 = 16654
    16654 - 16384 = 270

当前轻量全开计算：

    15778 + 146 + 128 = 16052
    16384 - 16052 = 332

曾经默认关闭时的 Program Size 是 data=157.4 xdata=315 const=146 code=14957，Keil 编译 0 Error / 0 Warning；后续仍应优先记录默认打开日志时的最大 CODE 占用。

以后只要涉及日志或字符串，就必须重新算这条公式。

## 10. uart_printf 的准确结论

工程中 Bsp/common.h 定义 usr_printf 和 uart_printf 都等价于 printf，而且 pc_uart.c、ch.c 已经大量使用 uart_printf。用户指出基础 printf 空间已经占用，这个判断是对的。

但一线调试日志仍不改成 uart_printf。准确理由不是 printf 完全没引入，而是：一线日志是高频路径，每 100ms 可能打印一帧；当前全开只剩约 332B；新增格式字符串、变参调用点和格式路径仍可能增加 code/const；C51 printf 对宽度、十六进制等格式支持也不如手写可控。

所以 pc_uart 这种人工低频查询可以继续用 uart_printf；uart_1_wire_debug 这种高频协议帧日志继续用手写 put_char / put_str / put_hex8。

## 11. 编码规则和已经踩过的坑

用户要求 debug c/h 用 UTF-8。最终规则是：App/uart_1_wire_debug.c 和 App/uart_1_wire_debug.h 用 UTF-8 无 BOM；memory.md 用 UTF-8-BOM，方便 Notepad++ 打开中文。

已经实测：如果给 Keil C51 编译的 C/H 文件加 UTF-8-BOM，会报 unprintable character 0xEF、0xBB、0xBF。所以 C/H 文件不能加 BOM。

CodexPro.read 有时显示中文 mojibake，不一定代表源码真乱码，要结合实际编码和 Keil 编译判断。

另一个坑是写 Markdown 时不能直接把带反引号的文本塞进 shell 命令，否则 shell 会把反引号内容当命令执行，出现 command not found。以后写文档要用安全方法，写完必须读回确认。

## 12. CODEX 协作情况

本话题里 CODEX 参与了部分修改和计划，但中途因为额度/权限问题无法继续，用户说 CODEX 罢工了，老板又催。之后由 ChatGPT 直接接手修改 uart_1_wire_debug.c/h、处理编码、补注释、跑 Keil 编译、写交接。

以后不要只信 CODEX 的状态文件。codex-status.md 可能包含累积旧内容，不一定代表当前工作区真实状态。要用 CodexPro.show_changes 看实际 diff，用 build log 看真实编译。

当前 .ai-bridge/current-plan.md 已指向 YAT-60V3A-Uart-1-wire/memory.md。后续 CODEX 应先读 memory.md，再继续。

## 13. 后续不要做的事

除非用户明确要求，否则不要改一线协议收发逻辑，不要改 uart_1_wire.c 状态机，不要改 pc_uart，不要改 DBG_BAUD=115200UL，不要改 EEPROM/Flash 布局，不要批量转码，不要批量格式化，不要新增日志 FIFO、中断发送日志、大数组、大字符串、复杂日志宏系统或动态内存。

不要把运行时日志改回中文。不要把 SOC、电流、电压、温度、Ah 等十进制值塞回一线高频日志。不要擅自把 U1W_DEBUG_EN 默认改成 0；后续先按 1U 编译以查看最大 CODE 占用，需要关闭时由用户自己改。

## 14. 后续还需要做什么

代码层面当前已经完成：ch.c 拔电池逻辑检查确认，一线调试日志轻量化，debug c/h UTF-8 无 BOM，中文注释补充，Keil 编译验证，CODEX 交接，memory.md。

剩余主要是真机验证：充电中拔电池后是否按预期先关输出；等待约 20ms 后 vout 是否能小于 1V；正常拔除是否回 CH_IDLE；异常拔出是否进 BMS_ERR；打开日志调试时是否影响现场一线通信时序；B4 故障分类是否符合预期。

如果用户决定提交 git，建议只提交相关源码、.ai-bridge 交接和 memory.md，不要提交无关临时日志。

## 15. 最终接续结论

当前方案是工程折中：比中文长日志短，比极简日志可读；全开能编译过，但空间很紧；当前按用户最新规则默认打开一线日志以查看最大 CODE 占用；不改一线协议业务逻辑，不改 pc_uart，不改波特率，不改 Flash/EEPROM 布局。后续维护原则是小改、可读、可编译、可验证，不要再把高频一线日志做重。


## 16. 2026-06-28 GBK/CP936 读取技能与本轮小优化

ChatGPT 已在 AGENTS.md 增加第 27 节：GBK/CP936 中文源码读取与保编码修改技能。核心规则：先读原始 bytes 判断编码；GBK 文件用 gbk/cp936 decode；Windows/MSYS 输出中文时加 PYTHONIOENCODING=utf-8；写回 GBK 文件必须继续 encode gbk，不能默认 write_text UTF-8；C/H 文件不能加 UTF-8-BOM。

本轮已确认源码中文不是坏了，而是 CodexPro 默认 UTF-8 读取造成 mojibake。用 GBK/CP936 可以正常看到 App/usr_cfg.h、App/ch.c、App/pc_uart.c、App/uart_1_wire.c 等中文注释和中文日志。

用户确认：内部 NTC 不做降额，只做保护/提示。因此 T_CH_HOT/T_CH_HOT_OK 只保留为内部高温提示阈值，不参与充电电流降额逻辑。

本轮已做低风险优化：删除未调用的 ch_fault_flag_active；删除未调用/空转的一线调试接口 u1w_dbg_parse_ok、u1w_dbg_fail、u1w_dbg_stop 及相关未用错误打印函数；CH_Check/CH_Pre1/CH_CCCV 中只在实际需要时计算 pre_end_voltage_mv 和 cccv_timeout_min，不再主循环每 10ms 全状态计算；清理运行时日志里的 xFD 规避写法，改成不含 GBK 0xFD 字节的词，例如 等BMS握手、高压保护、高温保护、电流保护、电流恢复、GAIN。

本轮还修正了 PC 参数打印：内部高温相关打印从高温降额/降额恢复改为高温提示/提示恢复；CC+CV 定时单位由 h 改为 min，因为 TIM_CCCV 的单位是分钟。

未改内容：没有修改 A4/A6/B1 字节序；没有修改 A1 容量含义；没有修改满电回充策略；没有改 Flash/EEPROM；没有改波特率；没有改一线异步轮询架构。

编译验证：Keil C51 编译通过，0 Error(s), 0 Warning(s)。Program Size: data=161.4 xdata=315 const=146 code=15661。按 code + const + 128 计算为 15935，16K 剩余约 449 字节。


## 17. uart_1_wire_get_info 指针接口结论

用户提出 uart_1_wire_get_info(UART_1WIRE_CHARGE_INFO_Types *info) 看起来不需要指针。ChatGPT 实测过改为返回结构体：UART_1WIRE_CHARGE_INFO_Types uart_1_wire_get_info(void)，调用处改为 s_u1w_info = uart_1_wire_get_info(); Keil C51 可编译通过，但 Program Size 从 code=15661 增加到 code=15808，增加约 147 字节。当前 16K 空间紧张，因此已回退为指针输出参数。

结论：从接口可读性看返回结构体更舒服，但在本 C51 工程中不划算。保留指针接口是为了节省 code，并保留空指针保护。后续不要再把该函数改成结构体返回，除非用户明确接受代码体积增加。


## 18. u1w_info 全局快照结论

用户进一步指出：这种小 MCU 中，给 ch.c 用的 s_u1w_info 直接做成全局就可以。ChatGPT 实测并采用该方案：删除 uart_1_wire_get_info() 外部接口，删除 ch.c 内部静态 s_u1w_info，改为 uart_1_wire.c 定义全局 UART_1WIRE_CHARGE_INFO_Types idata u1w_info，uart_1_wire.h extern 声明，ch.c 直接读取 u1w_info。

实现细节：协议层内部控制变量 s_u1w 仍保持 static，不暴露；只暴露给充电流程使用的快照 u1w_info。uart_1_wire_poll_10ms() 在正常路径末尾调用 u1w_refresh_info() 更新快照；PULL_LOW 提前 return 分支也先更新快照再返回，避免 ch.c 读到旧状态。

编译验证：Keil C51 通过，0 Error(s), 0 Warning(s)。Program Size: data=161.4 xdata=315 const=146 code=15563。相比原指针 getter 版本 code=15661，减少约 98 字节；相比结构体返回版本 code=15808，减少约 245 字节。

结论：本项目最终采用 u1w_info 全局快照方案。不要再恢复 uart_1_wire_get_info()，除非后续有明确的封装需求。
