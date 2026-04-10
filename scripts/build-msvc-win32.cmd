@echo off
setlocal
powershell -ExecutionPolicy Bypass -File "%~dp0build-msvc-win32.ps1" %*
