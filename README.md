# WxToDoListDemo

Minimal wxWidgets + Conan + CMake skeleton for a Windows app, configured
for a fully self-contained `.exe` (static wx libs + static MSVC runtime,
no VC++ Redistributable required).

**Division of labour:** Conan's only job here is to build wxWidgets and
write a `conan_toolchain.cmake` describing where it landed (include dirs,
libs, defines, system libs). It does **not** pick your generator or build
system — that's yours to choose in CMake (CMake-GUI or the command line).

## Prerequisites

- Windows 10/11 (x64) — the build system targets Windows exclusively (MSVC static runtime, Win32 API)
- Visual Studio 2022/2026 with the "Desktop development with C++" workload
- CMake 3.21+
- Conan 2.x  (`pip install -U conan`)

## 1. Build wxWidgets with Conan (once)

From the repo root, in a shell where Conan is on PATH:

```powershell
conan install . -of out -pr ./profiles/conan/compiler/msvc195-cpp20-static -s build_type=Release --build=missing
```

Add a Debug variant if you want to develop in Debug:

```powershell
conan install . -of out -pr ./profiles/conan/compiler/msvc195-cpp20-static -s build_type=Debug --build=missing
```

The first run compiles wxWidgets from source (static + `/MT` has no
prebuilt binary), so expect a wait. `-of out` just tells Conan to drop
its generated files into `out/`; the one you care about is:

```
out/conan_toolchain.cmake
```

That's the file that points CMake at the wxWidgets Conan just built.

> Conan also drops a `CMakePresets.json` / `CMakeUserPresets.json`. You
> can ignore them entirely with the GUI flow below — they're already
> gitignored.

## 2. Generate IDE project files with CMake-GUI

1. Open CMake-GUI.
2. **Where is the source code:** the repo root.
3. **Where to build the binaries:** the `out` folder (or any folder).
4. Click **Configure**.
5. In the dialog:
   - Pick your generator (e.g. *Visual Studio 17 2022*, or *Ninja*).
   - Select **Specify toolchain file for cross-compiling**.
   - Click **Next** and point it at `out/conan_toolchain.cmake`.
   - Finish.
6. Click **Configure** again if needed, then **Generate**.

CMake-GUI now writes the project files for the generator you chose — a
`.sln` for Visual Studio, `build.ninja` for Ninja, etc. — under your build
folder. Open them in your IDE.

> **Important:** the toolchain file must be set on the **first** Configure.
> CMake caches it. If you Configure once without it, `find_package`
> (wxWidgets) fails and the cache holds the wrong compiler/runtime guess —
> delete the cache (File -> Delete Cache) and start at step 4 again.

### Command-line equivalent

If you'd rather not use the GUI, the same thing in one line (pick your
own `-G`):

```powershell
cmake -S . -B out -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=out/conan_toolchain.cmake
cmake --build out --config Release
```

## Verifying it's truly self-contained

```powershell
dumpbin /dependents build\Release\WxToDoListDemo.exe
```

You should see only core Windows DLLs (kernel32, user32, gdi32, etc.) and
**no** `MSVCP*.dll` / `VCRUNTIME*.dll`, confirming the static CRT.

## Notes

- **Toolset version.** `profiles/conan/compiler/msvc195-cpp20-static` pins `compiler.version=195`
  (VS 2026 / `cl` 19.5x, toolset v145). On an older Visual Studio, edit
  that line to match your `cl` — `194` for VS 2022 17.10+, `193` for
  earlier 2022 — or delete it and run `conan profile detect` from an x64
  developer shell. Conan rebuilds wx per toolset, so it must match the
  compiler you generate against in CMake-GUI.

- **Generator name.** If CMake-GUI's generator dropdown doesn't list your
  Visual Studio, your CMake predates support for it — update CMake, or
  choose Ninja.

- **Architecture.** The profile pins `arch=x86_64`, so the Conan build is
  x64 regardless of which shell you launch from. Make sure the generator
  you pick in CMake-GUI also targets x64 (for the VS generator, set the
  optional platform to `x64`; the "Win32" default would mismatch).

## Trimming further

`conanfile.txt` already disables the heaviest unused components (stc,
webview, mediactrl, opengl, aui, ribbon, richtext, propgrid). Disable
more (e.g. `html`, `xrc`) the same way if you don't use them.

## If the dependency build fails

The 3.3.0 recipe had a known source-build break. If you hit trouble on a
`3.3.x` version, pin the mature stable line instead — change the require in
`conanfile.txt` to:

```ini
wxwidgets/3.2.10
```
