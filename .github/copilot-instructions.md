# Copilot Instructions for `canoe-node-layer-dll`

## Purpose
This repository is a minimal starter for building a CANoe NodeLayer DLL (`NodeLayer.dll`) for Windows 32-bit runtime compatibility.

## Repository Structure
- `include/`: External API headers used by the project (`cdll.h`, `VIA.h`, `VIA_CDLL.h`)
- `src/`: Project source files (`NodeLayer.cpp`, `NodeLayer.def`)
- `tools/`: Build scripts (`build-msvc-win32.ps1/.cmd`, `build-mingw32.ps1/.cmd`)
- `bin/`: Generated deliverable DLLs grouped by toolchain
- `build/`: CMake build folders (generated, not source of truth)
- `docs/`: Notes and additional documentation

## Non-Negotiable Constraints
- Keep NodeLayer runtime architecture compatible with CANoe 32-bit RTK.
- Preserve required undecorated exports for CANoe linking:
  - `VIARequiredVersion`
  - `VIASetService`
  - `VIAGetModuleApi`
  - `VIAReleaseModuleApi`
- Do not modify third-party headers in `include/` unless explicitly requested.
- Prefer minimal, targeted changes. Avoid broad refactors unless requested.

## Build and Verification
- MSVC build:
  - `./tools/build-msvc-win32.ps1`
- MinGW build:
  - `./tools/build-mingw32.ps1`
- Verify exports when touching ABI surface:
  - `dumpbin /exports bin/<toolchain>/NodeLayer.dll`

## Extension Guidelines
- Add new CAPL-callable APIs with `NL_` prefix (for example `NL_ReadStatus`).
- Register new APIs in the CAPL export table in `src/NodeLayer.cpp`.
- Keep the DLL entry points and existing behavior backward compatible.
- If new exports are required for CANoe linking behavior, update `src/NodeLayer.def` and verify with `dumpbin`.

## Logging Convention
- Environment variable override: `NODELAYERDLL_LOG`
- Default log filename in current implementation: `NodeLayerDLL.log`
