# Building Spooky View

Two supported toolchains:

| Toolchain         | Configs supported                     | Use when                              |
| ----------------- | ------------------------------------- | ------------------------------------- |
| **MSBuild / VS**  | `Debug`, `Release`, `Release MSI`, `Release Store`, all platforms | Producing a release, MSI, or MSIX     |
| **MinGW-w64**     | `Debug` only                          | Quick local builds, VS Code workflow  |

The repository ships a top-level `Makefile` that wraps the MinGW path.

---

## Quickstart (MinGW-w64)

From the repo root:

```sh
make restore       # one-time: nuget restore
make check-tools   # verify g++, windres, make are on PATH
make               # Debug build → SpookyView/bin/SpookyView.exe
```

Other targets: `make clean`, `make rebuild`, `make help`.

---

## Installing the MinGW-w64 toolchain

You need three things on `PATH`: **g++**, **windres**, and **GNU make**.
Pick one package manager:

### Option A — Chocolatey

```powershell
# Run PowerShell as Administrator
choco install -y mingw make nuget.commandline
```

`mingw` provides both `g++` and `windres`. `make` installs GNU make as
`make.exe` (no need for the `mingw32-make.exe` alias). After install, open a
**new** shell so PATH updates take effect.

Verify:

```powershell
g++ --version
windres --version
make --version
nuget help | Select-Object -First 1
```

### Option B — Scoop

```powershell
scoop install mingw make nuget
```

Scoop installs to your user profile and avoids needing admin. Same verification
commands as above.

### Option C — MSYS2 (manual)

If you already use MSYS2, install `mingw-w64-x86_64-gcc` and `make` from the
`MSYS2 MinGW 64-bit` shell. Make sure the MinGW `bin/` directory is on the
Windows `PATH` you build from (not just the MSYS shell PATH).

---

## Installing MSBuild / Visual Studio (Release builds)

Required for `Release`, `Release MSI`, `Release Store`, and any ARM64 build.

1. Install **Visual Studio 2022** (Community is fine) with the
   *Desktop Development with C++* workload.
2. Add the individual component:
   *C++ Windows XP Support for VS 2017 (v141) tools*. The `Release` configs
   use the `v141_xp` platform toolset; without it MSBuild will fail with
   *"The v141_xp build tools cannot be found"*.
3. (Optional, for MSI) install the **WiX Toolset v3** Visual Studio extension.
4. (Optional, for MSIX) ensure the *Windows 10/11 SDK* component is installed.

Build from Visual Studio (open `SpookyView.sln`) or from the command line:

```powershell
# From a "Developer PowerShell for VS 2022" prompt:
nuget restore SpookyView.sln
msbuild SpookyView.sln /p:Configuration=Release /p:Platform=x64
```

Replace `Release` with `Release MSI` / `Release Store` for installers.

---

## NuGet packages

The project depends on `nlohmann.json` (see `SpookyView/packages.config`).
The `packages/` directory is `.gitignore`d, so run `make restore` (or
`nuget restore SpookyView.sln`) before the first build. Visual Studio does
this automatically on solution load.

---

## Output locations

| Toolchain   | Path                                                 |
| ----------- | ---------------------------------------------------- |
| MinGW-w64   | `SpookyView/bin/SpookyView.exe`                      |
| MSBuild     | `SpookyView/bin/<Platform>/<Configuration>/SpookyView.exe` |
| MSI         | `WiXSetup/bin/<Platform>/<Configuration>/*.msi`      |
| MSIX        | `StorePackaging/AppPackages/...`                     |

---

## Logs and crash dumps

Spooky View writes a runtime log and (on unhandled SEH crashes) a minidump
under:

```
%LOCALAPPDATA%\SpookyView\
  spookyview.log
  spookyview.log.old   (after 1 MiB rotation)
  spookyview-crash-YYYYMMDD-HHMMSS-PID.dmp
```

When filing a bug, attach `spookyview.log` and the latest `.dmp` if present.
Open the dump in Visual Studio (`File → Open → File…`) or WinDbg to inspect
the crashing stack.

---

## Troubleshooting

* **`'make' is not recognized`** — install via choco/scoop (above) and open a
  new shell. If only `mingw32-make.exe` is on PATH, run
  `make MAKE_BIN=mingw32-make`.
* **`fatal error: nlohmann/json.hpp: No such file`** — run `make restore`.
* **`v141_xp not found`** during MSBuild — install the VS 2017 XP tools
  component listed above.
* **windres errors on `Manifest.rc`** — confirm you're invoking windres from
  inside `SpookyView/` (the top-level `Makefile` already handles this).
