# Skill: Extend Existing `NodeLayer.dll`

## When to Use
Use this skill when adding or modifying functionality in the existing NodeLayer DLL implementation without breaking CANoe compatibility.

## Goal
Safely extend behavior in `src/NodeLayer.cpp` while preserving required exports, ABI expectations, and 32-bit runtime compatibility.

## Inputs
- Requested feature or behavior change
- Whether API is internal only or CAPL-visible
- Target toolchain(s): MSVC, MinGW, or both

## Required Safety Checks
1. Preserve required undecorated VIA exports:
   - `VIARequiredVersion`
   - `VIASetService`
   - `VIAGetModuleApi`
   - `VIAReleaseModuleApi`
2. Keep build output as `NodeLayer.dll`.
3. Keep Win32 compatibility for CANoe RTK.
4. Ensure MinGW export names remain undecorated (no `@N` suffix).

## Step-by-Step Workflow
1. Read `src/NodeLayer.cpp` and identify extension point.
2. If CAPL-visible API is needed:
   - Add function with `NL_` prefix.
   - Add entry to CAPL DLL export table.
3. If export-level behavior changes:
   - Update `src/NodeLayer.def` if needed.
4. Build both toolchains when possible:
   - `./tools/build-msvc-win32.ps1`
   - `./tools/build-mingw32.ps1`
5. Verify exports:
   - `dumpbin /exports bin/msvc-win32/NodeLayer.dll`
   - `dumpbin /exports bin/mingw32/NodeLayer.dll`
6. Validate that required VIA exports are still present and undecorated.

## Do and Don’t
- Do keep changes focused and incremental.
- Do keep existing exported behavior stable unless explicitly asked to change it.
- Do update `README.md` when public usage changes.
- Don’t rename core VIA functions.
- Don’t introduce x64-only assumptions.
- Don’t modify third-party `include/*.h` API headers without explicit request.

## Completion Criteria
- Build succeeds for requested toolchain(s).
- `NodeLayer.dll` generated under `bin/<toolchain>/`.
- Required CANoe-linkable exports are intact.
- New feature is documented when externally visible.
