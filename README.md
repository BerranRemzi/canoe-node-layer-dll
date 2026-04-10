# CANoe NodeLayerDLL Template (Win32)

Minimal but production-oriented starter repository for building a CANoe NodeLayer DLL (`NodeLayer.dll`).

## What Is A NodeLayer DLL

A NodeLayer DLL is a CANoe-loadable module that implements Vector VIA interfaces so CANoe can:

- check version compatibility,
- pass runtime services to the DLL,
- acquire the NodeLayer API object,
- call CAPL-visible helper functions (optional).

This template gives you a known-good baseline that already loads in CANoe when built with compatible settings.

## Why Win32 Is Mandatory Here

Your CANoe runtime in this workflow expects a 32-bit compatible DLL. If the architecture does not match, CANoe rejects the module before your code executes.

Use one of these:

- MSVC Win32 (`-A Win32`)
- MinGW i686/mingw32 toolchain

## Required CANoe Loader Exports

CANoe must resolve these names:

- `VIARequiredVersion`
- `VIASetService`
- `VIAGetModuleApi`
- `VIAReleaseModuleApi`

This repository keeps export naming stable via `src/NodeLayer.def` and MinGW linker flags.

## Project Structure

- `CMakeLists.txt`: Top-level build configuration.
- `src/`: NodeLayer implementation and export definition.
- `include/`: Vector headers (`cdll.h`, `VIA.h`, `VIA_CDLL.h`).
- `tools/`: Build entry scripts for MSVC and MinGW (`.ps1` and `.cmd`).
- `bin/`: Final toolchain-specific DLL outputs.
- `build/`: Generated CMake build trees (intermediate output).
- `.github/`: Copilot project instructions and skills.

## Build Prerequisites

- Windows
- CMake 3.15+
- One toolchain:
- Visual Studio 2022 (MSVC, Win32 target)
- MinGW-w64 32-bit (mingw32/i686)

## Build (Recommended Scripts)

MSVC Win32:

```powershell
./tools/build-msvc-win32.ps1
```

MSVC wrapper:

```cmd
tools\build-msvc-win32.cmd
```

MinGW 32-bit:

```powershell
./tools/build-mingw32.ps1
```

MinGW wrapper:

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

## Output Artifacts

- MSVC output copy: `bin/msvc-win32/NodeLayer.dll`
- MinGW output copy: `bin/mingw32/NodeLayer.dll`

## Quick Smoke Test In CANoe

1. Load `NodeLayer.dll` from one toolchain output into CANoe.
2. Open CAPL Browser.
3. Call `NL_TestAdd(2, 3)`.
4. Expected result: `5`.

## Logging

Logging path selection order in `src/NodeLayer.cpp`:

- `NODELAYERDLL_LOG` environment variable path (if set)
- DLL folder as `NodeLayerDLL.log`
- temp folder fallback as `NodeLayerDLL.log`

Example:

```powershell
$env:NODELAYERDLL_LOG = "C:\\temp\\NodeLayerDLL.log"
```

## Troubleshooting

Architecture mismatch:

- Symptom: "module could not be loaded with current RTK architecture"
- Fix: build x86/Win32 only.

Missing VIA functions at load:

- Symptom: CANoe cannot link `VIARequiredVersion`, `VIASetService`, `VIAGetModuleApi`, `VIAReleaseModuleApi`
- Fix: verify exports with `dumpbin /exports` and keep `src/NodeLayer.def` in build.

MinGW runtime dependency problems:

- Symptom: load failure with correct architecture
- Fix: use the provided MinGW script, which builds with static libgcc/libstdc++ and i686 toolchain selection.

## Extending The DLL

When adding CAPL-callable APIs:

- Use `NL_` prefix for new functions.
- Register function in CAPL export table in `src/NodeLayer.cpp`.
- Keep existing loader exports backward-compatible.
- If ABI surface changes, update `src/NodeLayer.def` and re-verify exports.
