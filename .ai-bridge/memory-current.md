# 当前记忆：YAT-60V3A / ch.c

## 用户偏好

- 中文交流。
- 嵌入式 C 工程优先可维护、好理解。
- 不喜欢过度封装、小函数切太碎。
- 不喜欢多个 `switch(ch_state)` 或散落的状态判断。
- 小工程中经验数字可直接写在判断处，旁边加简短中文注释；不必都做宏。
- 日志必要时要保留，但注意 code 空间。
- 代码注释要简短但关键路径不能省。
- 旧 MCU 源码读写必须保持原编码，不能引入乱码。

## 当前工程/文件

- Workspace：`D:\Users\CODE`
- 工程：`YAT-60V3A-Uart-1-wire`
- 当前主改文件：`YAT-60V3A-Uart-1-wire/App/ch.c`
- 编码：GBK/CP936，无 BOM，CRLF。

## 本轮完成

- 将充电流程整理成一个主 `switch(ch_state)`。
- 将分压控制、ADC采样、拔电池判断、有压无流判断放回对应状态 case。
- 删除 `ch_prepare_vout_sample_10ms()` / `ch_battery_removed_check_10ms()` / `ch_relay_charge_state()` / `ch_need_vout_probe()` / `ch_vout_probe_reset()`。
- 删除新增小宏 `BMS_HANDSHAKE_TIMEOUT_S`、`CH_VOUT_PROBE_*`、`CH_NO_CURRENT_*`。
- 保留 `ch_no_current_fault_check_10ms()`，但它不再自己判断 `ch_state`，只由 `CH_Pre1` / `CH_CCCV` 调用。
- `BATT_DIVIDER_EN` 现在在主状态 case 内直接可见。

## 编译

```text
0 Error(s), 0 Warning(s)
Program Size: data=157.3 xdata=315 const=146 code=15586
```

## 空间

- 程序区：`0x0000 ~ 0x3F7F`。
- 校准区：`0x3F80 ~ 0x3FFF`，不能占用。
- 保守剩余：约 524 bytes。

## 后续注意

1. code 空间很紧，后续只适合小修。
2. 不要提交构建日志、参考工程、压缩包。
3. 当前不应提交：
   - `YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt`
   - `YAT-60V3A-Uart-1-wire/Project/Keil/build.log`
   - `YAT-21V4A-6A/`
   - `YAT-60V3A-Uart-1-wire.zip`
4. 提交应只包含：
   - `YAT-60V3A-Uart-1-wire/App/ch.c`
   - `.ai-bridge/2026-07-05-ch-flat-refactor-summary.md`
   - `.ai-bridge/current-handoff.md`
   - `.ai-bridge/memory-current.md`
