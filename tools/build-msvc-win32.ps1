param(
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $Root "build/msvc-win32"
$BinDir = Join-Path $Root "bin/msvc-win32"

cmake -S $Root -B $BuildDir -G "Visual Studio 17 2022" -A Win32
cmake --build $BuildDir --config $Config

if (-not (Test-Path $BinDir)) {
    New-Item -ItemType Directory -Path $BinDir | Out-Null
}

$OutputDll = Join-Path $BuildDir "$Config/NodeLayer.dll"
$DestinationDll = Join-Path $BinDir "NodeLayer.dll"
Copy-Item -Path $OutputDll -Destination $DestinationDll -Force

Write-Host "Build completed: $OutputDll"
Write-Host "Copied to: $DestinationDll"
