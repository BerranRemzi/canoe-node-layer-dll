param(
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $Root "build/mingw32"

# Prefer a real MinGW 32-bit toolchain if present.
$Mingw32BinCandidates = @(
    "C:\msys64\mingw32\bin",
    "D:\msys64\mingw32\bin",
    "D:\portapps\eclipse\msys64\mingw32\bin"
)

$foundMingw32 = $false
foreach ($candidate in $Mingw32BinCandidates) {
    if (Test-Path $candidate) {
        $gccI686 = Join-Path $candidate "i686-w64-mingw32-gcc.exe"
        $gxxI686 = Join-Path $candidate "i686-w64-mingw32-g++.exe"
        $gcc = Join-Path $candidate "gcc.exe"
        $gxx = Join-Path $candidate "g++.exe"

        if ((Test-Path $gccI686) -and (Test-Path $gxxI686)) {
            $env:PATH = "$candidate;$env:PATH"
            $env:CC = $gccI686
            $env:CXX = $gxxI686
            $foundMingw32 = $true
            break
        } elseif ((Test-Path $gcc) -and (Test-Path $gxx)) {
            $env:PATH = "$candidate;$env:PATH"
            $env:CC = $gcc
            $env:CXX = $gxx
            $foundMingw32 = $true
            break
        }
    }
}

if (-not $foundMingw32) {
    throw "No MinGW 32-bit toolchain found. Install/use MSYS2 mingw32 (i686) and ensure a mingw32\\bin path is available."
}

if (Test-Path (Join-Path $BuildDir "CMakeCache.txt")) {
    Remove-Item $BuildDir -Recurse -Force
}

cmake -S $Root -B $BuildDir -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=$Config -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_SHARED_LINKER_FLAGS="-m32 -static-libgcc -static-libstdc++"
if ($LASTEXITCODE -ne 0) { throw "CMake configure failed for MinGW 32-bit build." }

cmake --build $BuildDir --config $Config
if ($LASTEXITCODE -ne 0) { throw "CMake build failed for MinGW 32-bit build." }

Write-Host "Build completed: $BuildDir/NodeLayer.dll"
