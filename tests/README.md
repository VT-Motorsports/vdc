# TV regression and sweep scripts

PowerShell scripts that drive `build64\tv_engine.dll` through its exported
`tv_in` / `tv_step` / `tv_out` interface, the same way ChassisSim does. Run them
from any folder. Paths default to locations relative to the repo root.

Requirements: 64-bit MinGW `g++` and `tar` on `PATH` (`C:\msys64\mingw64\bin` is
added automatically if it exists), plus a built DLL:

```
cd build64
mingw32-make tv_engine
```

Each script stages copies of the DLL, with its own `tv_config.txt`, under
`build64\tv_tests\` (gitignored), and compiles the harness from `step.cpp` and `caps.cpp`.
The DLL in `build64` and its config are never modified.

The fixed case used throughout is 15 m/s, 1.4 g lateral, 50% throttle, run for
5 steps. The slip case is throttle 0.3 with the FR wheel speed +2%.

## run2p.ps1: torques, slip and internals

```
.\tests\run2p.ps1
.\tests\run2p.ps1 -PreDll <ref.dll> -NoGuardDll <ref.dll>
```

This prints:

- the fixed-case wheel torques at `k_mz_ay` = 0, 20 and 40
- the slip case
- the internal `trq_cap`, `fz`, `mz_des` and `trq` values, from `caps.exe`, which
  compiles `src\engine\tv_engine.cpp` in directly

`-PreDll` adds a fixed-case comparison and a 2000-random-input comparison against
a reference DLL. `-NoGuardDll` adds a second DLL's slip results next to the slip
case. Both are optional. Use `-Dll` and `-Src` to test another DLL or source tree.

## exact.ps1: bit-exact regression against a baseline

```
.\tests\exact.ps1 -BaseDll <baseline tv_engine.dll> -BaseRef <git ref it was built from>
```

This compares the baseline and the current DLL in `%a` hex at `k_mz_ay` = 0, 20
and 40, using the default config with no other keys. It covers the fixed case, the
slip case, 2000 seeded random inputs, and the internal `fz` / `trq_cap` / `mz_des` /
`trq` values. The baseline's internals come from `src/` at `-BaseRef` (default
`HEAD`), extracted with `git archive`. The script exits 1 if anything differs.

To get a baseline, copy `build64\tv_engine.dll` before changing the code. You can
also build an older commit in place, starting from a clean working tree:

```
git switch --detach <ref>
cd build64; mingw32-make tv_engine; copy tv_engine.dll $env:TEMP\base.dll; cd ..
git switch -
cd build64; mingw32-make tv_engine; cd ..
```

Both DLLs should come from the same `build64` setup. A different compiler or
different flags can change float results.

## sweep.ps1: lltd_f / k_mz_ay sweep

```
.\tests\sweep.ps1
.\tests\sweep.ps1 -Lltd default,0.35,0.45,0.55 -K 0,20,40
```

This prints one row per `lltd_f` value with the fixed-case torques, the slip-case
torques, per-wheel `fz` and `trq_cap`, and `mz_des`. There is one table per
`k_mz_ay`. `default` leaves `lltd_f` out of `tv_config.txt`, so the `tv_io`
default applies. Other values are written as `lltd_f=<value>` (the DLL accepts 0 to 1).
