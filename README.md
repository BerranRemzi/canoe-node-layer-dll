# CANoe NodeLayerDLL Minimal Template

Minimal, working starter repository for CANoe NodeLayer DLL development.

## Project Structure

- `CMakeLists.txt`: Top-level build configuration.
- `src/`: NodeLayer implementation and export definition (`NodeLayer.cpp`, `NodeLayer.def`).
- `include/`: Vector headers (`cdll.h`, `VIA.h`, `VIA_CDLL.h`).
- `tools/`: Build entry scripts for MSVC and MinGW (`.ps1` and `.cmd`).
- `bin/`: Toolchain-specific deliverables:
  - `bin/msvc-win32/NodeLayer.dll`
  - `bin/mingw32/NodeLayer.dll`
- `build/`: Generated CMake build trees (intermediate output).
- `.github/`: Copilot instructions and reusable skill documents.

## Prerequisites

- Windows
- CMake 3.15+
- One toolchain:
  - Visual Studio 2022 (MSVC, Win32 target)
  - MinGW-w64 (32-bit, mingw32/i686 toolchain)

## Build (Scripts)

MSVC Win32:

```powershell
./tools/build-msvc-win32.ps1
```

CMD wrapper:

```cmd
tools\build-msvc-win32.cmd
```

MinGW 32-bit:

```powershell
./tools/build-mingw32.ps1
```

CMD wrapper:

```cmd
tools\build-mingw32.cmd
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

- MSVC: `bin/msvc-win32/NodeLayer.dll` (copied from `build/msvc-win32/Release/NodeLayer.dll`)
- MinGW: `bin/mingw32/NodeLayer.dll` (copied from `build/mingw32/NodeLayer.dll`)

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
