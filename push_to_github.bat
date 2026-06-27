@echo off
chcp 65001 >nul
echo ========================================
echo   60V3A 充电器 - 推送代码到 GitHub
echo ========================================
echo.

cd /d "%~dp0"

REM 清理之前的 git 残留
if exist .git rmdir /s /q .git
if exist .gitignore del .gitignore

REM 创建 .gitignore
echo # Keil 编译产物> .gitignore
echo Project/Keil/Release/>> .gitignore
echo Project/Keil/.vscode/>> .gitignore
echo Project/Keil/*.uvgui.*>> .gitignore
echo Project/Keil/*.uvopt>> .gitignore
echo Project/Keil/*.ini>> .gitignore
echo Project/Keil/*.log>> .gitignore
echo Project/Keil/*.lock>> .gitignore
echo Project/Keil/*.bat>> .gitignore
echo Project/Keil/*.bak*>> .gitignore
echo Project/Keil/TXLinkLog.txt>> .gitignore
echo.>> .gitignore
echo # 通用>> .gitignore
echo *.o>> .gitignore
echo *.obj>> .gitignore
echo *.lst>> .gitignore
echo *.hex>> .gitignore
echo *.bin>> .gitignore
echo *.map>> .gitignore
echo *.lnp>> .gitignore
echo *.dep>> .gitignore
echo *.d>> .gitignore
echo *.crf>> .gitignore
echo *.axf>> .gitignore
echo *.elf>> .gitignore
echo *.i>> .gitignore
echo *.s>> .gitignore
echo *.bak>> .gitignore
echo *.orig>> .gitignore
echo *~>> .gitignore
echo __pycache__/>> .gitignore
echo .DS_Store>> .gitignore
echo Thumbs.db>> .gitignore

REM 初始化仓库
git init
git config user.name "yanyanyan168"
git config user.email "yanyanyan168@users.noreply.github.com"

REM 添加文件
git add -A

REM 提交
git commit -m "初始版本: 60V3A充电器 UART-1-Wire通信

- TX8C126x MCU, Keil C51工程
- P04单总线UART通信协议 (4800bps)
- 充电状态机: 待机/握手/预充/恒流恒压/充满/异常保护
- BMS电池包通信: A0-A7/B1-B6命令集
- DEBUG串口日志与参数打印
- NTC温度保护/OCP过流保护/OVP过压保护"

REM 添加远程仓库并推送
git remote add origin https://github.com/yanyanyan168/YAT-60V3A-Uart-1-wire.git
git push -u origin master

echo.
echo ========================================
echo   推送完成！按任意键关闭...
echo ========================================
pause >nul
