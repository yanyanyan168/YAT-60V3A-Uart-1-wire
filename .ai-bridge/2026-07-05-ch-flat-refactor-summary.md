# YAT-60V3A ch.c 扁平化整理总结

日期：2026-07-05
范围：`YAT-60V3A-Uart-1-wire/App/ch.c`

## 用户风格约束

1. 本轮只改 `ch.c`。
2. 小 MCU / C51 工程，不要到处切小函数。
3. 不要多个 `switch(ch_state)`，也不要把 `ch_state` 判断散落在多个 if / helper 中。
4. 尽量扁平化：每个状态干什么，要在主 `switch(ch_state)` 的对应 case 里一眼看懂。
5. 明确比较数字不要再做小宏定义，数字处加简短中文注释即可。
6. 保留必要日志，但不要浪费太多代码空间。
7. 读写源码必须保持原编码，不能引入中文乱码。

## 已完成改动

只修改了：

```text
YAT-60V3A-Uart-1-wire/App/ch.c
```

删除了分散状态判断/分散处理函数：

```text
ch_prepare_vout_sample_10ms()
ch_battery_removed_check_10ms()
ch_relay_charge_state()
ch_need_vout_probe()
ch_vout_probe_reset()
s_vout_sample_valid
```

删除了新增的小数字宏，改成数字处直接注释：

```text
BMS_HANDSHAKE_TIMEOUT_S
CH_VOUT_PROBE_*
CH_NO_CURRENT_*
```

保留一个主状态机：

```c
switch(ch_state)
```

各状态 case 内直接处理：

```text
1. 一线通信阶段设置
2. BATT_DIVIDER_EN 控制
3. ADC 采样
4. 拔电池判断
5. 有压无流判断
6. 当前状态的业务跳转
```

## 关键行为保持

### CH_IDLE

- 输出关闭。
- 分压常开，用于判断电池接入。
- `val.vout >= vSTART` 连续 500ms 后进入 `BMS_HANDSHAKE`。
- `vRESET < val.vout < vSTART` 稳定 2 秒后进入 `CH_UVP`。

### BMS_HANDSHAKE

- 输出关闭。
- 分压常开。
- 握手最多等待 20 秒，超时进入 `BMS_ERR`。

### CH_Check

- 输出关闭。
- 分压常开。
- 根据电池电压进入 `CH_REPAIR` / `CH_Pre1` / `CH_CCCV`。

### CH_REPAIR

- 分压常开。
- 继电器断开，只开修复小电流。
- 达到 `vPRE_30V` 后转 `CH_Pre1`。

### CH_Pre1 / CH_CCCV

- 继电器闭合，分压常开。
- 充电中拔电池直接在 case 内处理：
  - `val.curr < iGED` 且 `u1w_info.no_rx_10ms >= 100U`，认为疑似拔电池。
  - 先 `ch_output_all_off()`。
  - 单独打开 `BATT_DIVIDER_EN`。
  - 打开 20ms 后，用当前周期 `adc_sample_all()` 换算后的 `val.vout` 判断。
  - `val.vout < vRESET`：回 `CH_IDLE`，日志 `拔电池`。
  - 否则：进入 `BMS_ERR`，日志 `拔出异常`。
- 有压无流仍通过 `ch_no_current_fault_check_10ms()` 判断，但只在 `CH_Pre1` / `CH_CCCV` case 中调用，不再自己判断 `ch_state`。
- `CH_CCCV` 中接近目标电压 500mV 内不判有压无流，避免误判满电末端。

### CH_FULL

- 输出关闭。
- 分压默认关。
- 每 1 秒短开一次分压。
- 打开 20ms 后 ADC 有效。
- 最多打开 100ms，降低漏电/倒灌风险。
- `val.vout < vRESET` 连续 2 次有效采样：回 `CH_IDLE`。
- `val.vout < vCH60` 连续 2 次有效采样：重新握手。

### 异常/保护状态

- 输出关闭。
- 分压默认关。
- 每 1 秒短开一次，20ms 后判断当前电池端电压。
- `val.vout < vRESET` 连续 2 次有效采样：回 `CH_IDLE`。
- `BMS_TEMP_ERR` 保留温度恢复重新握手逻辑。
- `CH_OTP` 保留充电器温度恢复重新握手逻辑。

## 编译结果

Keil 命令：

```text
D:/KEIL_V5/UV4/UV4.exe -b Project.uvproj -o build.log
```

结果：

```text
0 Error(s), 0 Warning(s)
Program Size: data=157.3 xdata=315 const=146 code=15586
```

## 空间情况

程序区：

```text
0x0000 ~ 0x3F7F = 16256 bytes
```

校准区保留：

```text
0x3F80 ~ 0x3FFF = 128 bytes
```

保守按 `code + const` 计算：

```text
15586 + 146 = 15732 bytes
16256 - 15732 = 524 bytes 剩余
```

## 编码检查

`ch.c` 当前编码检查：

```text
BOM: 无
UTF-8: FAIL
GBK: OK
GB18030: OK
CP936: OK
```

说明：保持了旧工程 GBK/CP936 风格，没有转 UTF-8。

## 注意事项

1. 当前 code 空间已较紧，后续只适合小修。
2. 后续不要再新增大段日志、复杂字符串或大函数。
3. 若需要进一步减 code，可考虑合并异常状态中重复的短开分压逻辑，但不要破坏“主 switch 一眼看懂”的风格。
4. 本轮为了减少重复 code，在同一个 `switch(ch_state)` 内使用了 `stopped_state_probe:` 标签，供异常/保护 case 复用短开分压检测段。
