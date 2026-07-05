# 当前交接：YAT-60V3A ch.c 扁平化整理

## 当前任务状态

已完成实现并编译通过。

只改动源码：

```text
YAT-60V3A-Uart-1-wire/App/ch.c
```

同时新增交接记录：

```text
.ai-bridge/2026-07-05-ch-flat-refactor-summary.md
.ai-bridge/current-handoff.md
.ai-bridge/memory-current.md
```

## 需要继续遵守的风格

- 小 MCU / C51 工程，不要过度抽象。
- 不要多个 `switch(ch_state)`。
- 不要让 `ch_state` 判断散在多个函数里。
- 每个状态做什么，应尽量在主 `switch(ch_state)` 的 case 内看清楚。
- 数字判断优先直接写在比较处，旁边加中文注释，不要为每个数字都加宏。
- 中文注释要简短，但关键路径不能省。
- 读写源码前必须确认编码；当前 `ch.c` 是 GBK/CP936，无 BOM，CRLF。

## 当前实现重点

1. 删除旧的分散函数：
   - `ch_prepare_vout_sample_10ms()`
   - `ch_battery_removed_check_10ms()`
   - `ch_relay_charge_state()`
   - `ch_need_vout_probe()`
   - `ch_vout_probe_reset()`
2. 保留一个主 `switch(ch_state)`。
3. 各状态内部直接处理分压、ADC、拔电池、有压无流、状态跳转。
4. `CH_Pre1` / `CH_CCCV` 中直接处理充电中拔电池。
5. `CH_FULL` / 异常状态中直接处理短开分压和低压拔电池。
6. 由于异常/保护状态较多，为减少 code，占用在同一个 switch 内用 `stopped_state_probe:` 标签复用短开分压检测段。

## 当前编译结果

```text
0 Error(s), 0 Warning(s)
Program Size: data=157.3 xdata=315 const=146 code=15586
```

## 当前空间

程序区：`0x0000 ~ 0x3F7F`。

校准区：`0x3F80 ~ 0x3FFF`，保留 128 bytes。

保守剩余：

```text
16256 - (15586 + 146) = 524 bytes
```

## 不要提交的当前杂项

当前工作区里可能存在这些构建/参考产物，不属于本次提交：

```text
YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt
YAT-60V3A-Uart-1-wire/Project/Keil/build.log
YAT-21V4A-6A/
YAT-60V3A-Uart-1-wire.zip
```

提交时只应包含：

```text
YAT-60V3A-Uart-1-wire/App/ch.c
.ai-bridge/2026-07-05-ch-flat-refactor-summary.md
.ai-bridge/current-handoff.md
.ai-bridge/memory-current.md
```
