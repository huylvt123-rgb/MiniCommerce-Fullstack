@echo off
cd /d "%~dp0"
where g++ >nul 2>nul
if errorlevel 1 (
  echo Please install MinGW-w64 g++ and add it to PATH.
  pause
  exit /b 1
)
g++ -std=c++17 -O2 -Wall -Wextra SevenLabs.cpp -o sevenlabs.exe
if errorlevel 1 (pause & exit /b 1)
sevenlabs.exe
pause
