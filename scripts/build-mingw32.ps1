param(
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $Root "build/mingw32"

cmake -S $Root -B $BuildDir -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=$Config
cmake --build $BuildDir --config $Config

Write-Host "Build completed: $BuildDir/NodeLayer.dll"
