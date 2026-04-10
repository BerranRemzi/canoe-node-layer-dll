@echo off
setlocal
powershell -ExecutionPolicy Bypass -File "%~dp0build-mingw32.ps1" %*
