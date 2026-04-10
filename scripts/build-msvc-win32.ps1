param(
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $Root "build/msvc-win32"

cmake -S $Root -B $BuildDir -G "Visual Studio 17 2022" -A Win32
cmake --build $BuildDir --config $Config

Write-Host "Build completed: $BuildDir/$Config/NodeLayer.dll"
