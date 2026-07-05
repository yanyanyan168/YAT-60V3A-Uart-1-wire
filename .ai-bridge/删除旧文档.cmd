@echo off
chcp 65001 >nul
cd /d "%~dp0"

echo 本脚本只删除 .ai-bridge 根目录旧的零散 Markdown 文件。
echo 不会删除以下新目录：记忆、总结、交接、履历。
echo.
pause

del /f /q "2026-07-05-ch-flat-refactor-summary.md" 2>nul
del /f /q "agent-status.md" 2>nul
del /f /q "codex-status.md" 2>nul
del /f /q "codexpro-self-test.md" 2>nul
del /f /q "current-handoff.md" 2>nul
del /f /q "current-plan.md" 2>nul
del /f /q "decisions.md" 2>nul
del /f /q "memory-current.md" 2>nul
del /f /q "open-questions.md" 2>nul
del /f /q "pro-context.md" 2>nul

echo.
echo 已删除旧的零散 Markdown 文件。
echo 保留 README.md、execution-log.jsonl、session-log.jsonl、implementation-diff.patch。
echo.
pause
