# 只补本轮干净 implementation-diff.patch

Updated: 2026-06-28T04:17:43.204Z
Workspace: D:\Users\CODE
Target agent: Codex (codex)

## Plan

# 本轮任务：只补干净的 implementation-diff.patch，不再改源码

## 0. 当前状态

ChatGPT Pro 已经检查过本轮源码实现：

1. `YAT-60V3A-Uart-1-wire/App/uart_1_wire.c` 当前逻辑基本正确；
2. `u1w_key_index()` 已删除；
3. `u1w_parse_frame()` 中 A0/A1/A4/A6/A7/B1/B3/B4 已在各自 case 内直接设置 `key`；
4. `key_age_10ms` 在 switch 之后统一刷新；
5. `handshake_mask` 只在 `s_u1w.stage == U1W_STAGE_HANDSHAKE` 时更新；
6. `u1w_rx_task()` 的校验流程没有被修改，仍然只有校验正确才调用 `u1w_parse_frame()`；
7. Keil 编译结果已确认：0 Error(s), 0 Warning(s)，Program Size 为 `data=163.4 xdata=315 const=146 code=15752`；
8. `code + const + 128 = 15752 + 146 + 128 = 16026`，小于 16384，剩余 358 字节。

所以当前不要再改源码。

## 1. 本轮只允许做什么

本轮只允许补交付文件：

```text
.ai-bridge/implementation-diff.patch
.ai-bridge/codex-status.md
```

其中重点是重新生成 `.ai-bridge/implementation-diff.patch`。

## 2. 本轮禁止做什么

严禁修改以下源码或配置：

```text
YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
YAT-60V3A-Uart-1-wire/App/ch.c
YAT-60V3A-Uart-1-wire/App/pc_uart.c
YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
YAT-60V3A-Uart-1-wire/App/usr_cfg.h
YAT-60V3A-Uart-1-wire/memory.md
YAT-60V3A-Uart-1-wire/memory.md.bak_encoding_20260628_105912
```

严禁再做：

1. 重新改 `u1w_parse_frame()`；
2. 重新改握手逻辑；
3. 删除或修改 `u1w_dbg_parse_ok()`；
4. 改 `ch.c`；
5. 改 `pc_uart.c`；
6. 改默认调试开关；
7. 改编码；
8. 运行会自动改源码的格式化工具；
9. 混入旧任务的 `ch.c` 大 diff。

## 3. 为什么要补 patch

ChatGPT Pro 发现当前 `.ai-bridge/implementation-diff.patch` 不是本轮干净 diff，里面混入了旧任务的 `ch.c` 大 diff。

这会影响后续审查和交接，所以要重新生成一个只对应本轮的干净 patch。

## 4. implementation-diff.patch 应包含什么

新的 `.ai-bridge/implementation-diff.patch` 只应包含本轮相关内容：

必须包含：

```text
YAT-60V3A-Uart-1-wire/App/uart_1_wire.c
```

可以包含，如果你确实更新了状态：

```text
.ai-bridge/codex-status.md
```

可以包含，如果 current-plan 本身被更新后需要记录：

```text
.ai-bridge/current-plan.md
```

但不要把历史旧任务混进来。

尤其不要包含：

```text
YAT-60V3A-Uart-1-wire/App/ch.c
YAT-60V3A-Uart-1-wire/App/pc_uart.c
YAT-60V3A-Uart-1-wire/App/usr_cfg.h
YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.c
YAT-60V3A-Uart-1-wire/App/uart_1_wire_debug.h
YAT-60V3A-Uart-1-wire/Project/Keil/TXLinkLog.txt
YAT-60V3A-Uart-1-wire/Project/Keil/*.log
YAT-60V3A-Uart-1-wire/memory.md
```

## 5. 推荐生成方式

不要直接用全仓库 `git diff` 覆盖 patch，因为工作区里有很多历史未提交内容。

推荐只针对本轮文件生成：

```text
git diff -- YAT-60V3A-Uart-1-wire/App/uart_1_wire.c .ai-bridge/codex-status.md > .ai-bridge/implementation-diff.patch
```

如果你的工具不允许重定向，就用等价方式写入，但内容必须只来自这两个路径的 diff。

生成后必须检查 patch 内容，确认没有出现：

```text
App/ch.c
App/pc_uart.c
App/usr_cfg.h
uart_1_wire_debug.c
uart_1_wire_debug.h
Project/Keil
memory.md
```

如果出现这些路径，说明 patch 不干净，必须重新生成。

## 6. 对本轮源码的复核要求，只读，不写

生成 patch 后，只读确认下面几项：

1. 搜索 `u1w_key_index`，应无结果；
2. 读取 `u1w_parse_frame()` 片段，确认 `key = U1W_KEY_MAX;` 在 switch 前；
3. 确认 A0/A1/A4/A6/A7/B1/B3/B4 的 case 内各自设置 `key`；
4. 确认 B6 不设置 `key`；
5. 确认 switch 后统一执行：

```c
if(key < U1W_KEY_MAX)
{
    s_u1w.key_age_10ms[key] = 0U;
    if(s_u1w.stage == U1W_STAGE_HANDSHAKE)
    {
        uart_1_wire.handshake_mask |= (u8)(1U << key);
    }
}
```

这里只读检查，不允许改源码。

## 7. 本轮报告格式

完成后只报告下面内容，不要贴大量命令流水账：

```text
1. 是否只更新了 .ai-bridge/implementation-diff.patch 和必要状态文件；
2. patch 中包含哪些文件；
3. patch 中是否排除了 ch.c、pc_uart.c、memory.md、Keil 日志等无关内容；
4. 是否确认 u1w_key_index 已无残留；
5. 是否确认 uart_1_wire.c 逻辑仍保持本轮结果；
6. 是否没有再改源码；
7. 如更新 codex-status.md，说明新增了哪一小段。
```

## 8. 额外通用规则

ChatGPT Pro 已经在 `AGENTS.md` 追加第 26 节“小范围修改的高效执行规则”。

本轮你应先读取 `AGENTS.md` 第 26 节，再执行本计划。

重点记住：

1. 小范围修改必须先限定函数范围，不能全文件替换；
2. CP936/GBK 文件不能用默认 UTF-8 方式硬写；
3. 计划外 BUG 只记录，不顺手改；
4. `implementation-diff.patch` 必须对应本轮任务，不能混入旧任务 diff。

## Implementation contract

- Work from this plan in small, reviewable steps.
- Keep edits scoped to the requested task and existing project conventions.
- Run focused verification before handing work back.
- Update .ai-bridge/agent-status.md with files touched, checks run, results, blockers, and review notes.
- Save the final review diff to .ai-bridge/implementation-diff.patch when practical.
- Append notable execution events to .ai-bridge/execution-log.jsonl when the implementation agent supports logging.
