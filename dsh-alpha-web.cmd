@echo off
chcp 65001 >nul
title DSH web - 0.1.5-alpha.1
cd /d "%~dp0"
echo ============================================
echo   DSH web  版本: 0.1.5-alpha.1 (alpha)
echo   自动关闭 3080 端口的旧实例后启动
echo ============================================
echo.
echo [1/2] 检查并释放 3080 端口 ...
for /f "tokens=5" %%P in ('netstat -ano -p tcp ^| findstr /C:":3080" ^| findstr /C:"LISTENING"') do (
  echo       关闭旧实例 PID %%P ...
  taskkill /PID %%P /F >nul 2>&1
)
echo [2/2] 启动新版 dsh web (按 Ctrl+C 停止) ...
npx -y @deepseek-ai/dsh@0.1.5-alpha.1 web
echo.
echo dsh 进程已退出。
pause
