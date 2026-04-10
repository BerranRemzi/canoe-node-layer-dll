# CANoe NodeLayerDLL Minimal Template

Minimal, working starter repository for CANoe NodeLayer DLL development.

## What Is Included

- Single working source file: `sources/NodeLayer.cpp`
- Vector headers: `includes/cdll.h`, `includes/VIA.h`, `includes/VIA_CDLL.h`
- Build scripts for MSVC Win32 and MinGW 32-bit

## Prerequisites

- Windows
- CMake 3.15+
- One toolchain:
  - Visual Studio 2022 (MSVC, Win32 target)
  - MinGW-w64 (32-bit)

## Build (Scripts)

MSVC Win32:

```powershell
./scripts/build-msvc-win32.ps1
```

MinGW 32-bit:

```powershell
./scripts/build-mingw32.ps1
```

## Build (Direct CMake)

MSVC Win32:

```powershell
cmake -S . -B build/msvc-win32 -G "Visual Studio 17 2022" -A Win32
cmake --build build/msvc-win32 --config Release
```

MinGW 32-bit:

```powershell
cmake -S . -B build/mingw32 -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build/mingw32 --config Release
```

## Output

- MSVC: `build/msvc-win32/Release/NodeLayer.dll`
- MinGW: `build/mingw32/NodeLayer.dll`

## Smoke Test In CANoe

1. Load built `NodeLayer.dll` in CANoe.
2. Open CAPL Browser.
3. Call `NL_TestAdd(2, 3)` and verify result is `5`.

## Logging

The DLL supports `NODELAYERDLL_LOG` environment variable for log path behavior.

Example:

```powershell
$env:NODELAYERDLL_LOG = "C:\\temp\\NodeLayerDLL.log"
```
