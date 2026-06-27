@echo off
if /I "%~1" NEQ "__RUN__" (
  start "CodexPro Check" cmd.exe /k ""%~f0" __RUN__"
  exit /b
)

cd /d "%~dp0"

set "LOG=%~dp0codexpro_workspace_check.log"

echo ===============================================
echo CodexPro local workspace check
echo This window should NOT close automatically.
echo Current folder: %cd%
echo Log file: %LOG%
echo ===============================================
echo.

echo =============================================== > "%LOG%"
echo CodexPro local workspace check >> "%LOG%"
echo Date: %date% %time% >> "%LOG%"
echo Script folder: %~dp0 >> "%LOG%"
echo Current folder: %cd% >> "%LOG%"
echo =============================================== >> "%LOG%"
echo. >> "%LOG%"

echo [1/7] Windows and path
echo. >> "%LOG%"
echo --- 1. Windows and path --- >> "%LOG%"
ver >> "%LOG%" 2>&1
echo USERPROFILE=%USERPROFILE% >> "%LOG%"
echo OneDrive=%OneDrive% >> "%LOG%"
echo CURRENT=%cd% >> "%LOG%"
echo SCRIPT=%~dp0 >> "%LOG%"

echo [2/7] Workspace files
echo. >> "%LOG%"
echo --- 2. Workspace files --- >> "%LOG%"
if exist ".git" (echo OK: .git found >> "%LOG%") else (echo WARN: .git not found >> "%LOG%")
if exist "package.json" (echo OK: package.json found >> "%LOG%") else (echo INFO: package.json not found >> "%LOG%")
if exist "AGENTS.md" (echo OK: AGENTS.md found >> "%LOG%") else (echo INFO: AGENTS.md not found >> "%LOG%")
if exist "README.md" (echo OK: README.md found >> "%LOG%") else (echo INFO: README.md not found >> "%LOG%")

echo [3/7] Git
echo. >> "%LOG%"
echo --- 3. Git --- >> "%LOG%"
where git >> "%LOG%" 2>&1
if errorlevel 1 (
  echo FAIL: git not found >> "%LOG%"
) else (
  git --version >> "%LOG%" 2>&1
  git status --short >> "%LOG%" 2>&1
)

echo [4/7] Node and npm
echo. >> "%LOG%"
echo --- 4. Node and npm --- >> "%LOG%"
where node >> "%LOG%" 2>&1
if errorlevel 1 (echo FAIL: node not found >> "%LOG%") else (node -v >> "%LOG%" 2>&1)

where npm >> "%LOG%" 2>&1
if errorlevel 1 (
  echo FAIL: npm not found >> "%LOG%"
) else (
  npm -v >> "%LOG%" 2>&1
  npm config get prefix >> "%LOG%" 2>&1
)

echo [5/7] Python
echo. >> "%LOG%"
echo --- 5. Python --- >> "%LOG%"
where python >> "%LOG%" 2>&1
if errorlevel 1 (echo INFO: python not found >> "%LOG%") else (python --version >> "%LOG%" 2>&1)
where py >> "%LOG%" 2>&1
if errorlevel 1 (echo INFO: py launcher not found >> "%LOG%") else (py --version >> "%LOG%" 2>&1)

echo [6/7] Codex and CodexPro
echo. >> "%LOG%"
echo --- 6. Codex and CodexPro --- >> "%LOG%"
where codexpro >> "%LOG%" 2>&1
if errorlevel 1 (echo WARN: codexpro command not found >> "%LOG%") else (codexpro --version >> "%LOG%" 2>&1)
where codex >> "%LOG%" 2>&1
if errorlevel 1 (echo WARN: codex command not found >> "%LOG%") else (codex --version >> "%LOG%" 2>&1)

echo [7/7] npm global packages
echo. >> "%LOG%"
echo --- 7. npm global packages --- >> "%LOG%"
where npm >nul 2>&1
if errorlevel 1 (
  echo INFO: npm missing, skip global package list >> "%LOG%"
) else (
  npm list -g --depth=0 >> "%LOG%" 2>&1
)

echo.
echo ===============================================
echo DONE
echo Log saved:
echo %LOG%
echo ===============================================
echo.
echo Please send codexpro_workspace_check.log to ChatGPT.
echo.
pause
