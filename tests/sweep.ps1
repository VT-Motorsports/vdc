# run2 fixed case (15 m/s, 1.4 g, 50% throttle, 5 steps) and slip case (throttle 0.3,
# FR +2%) across lltd_f and k_mz_ay values: DLL output torques plus fz and trq_cap.
param(
  [string]$Dll = "",   # DLL under test (default: build64\tv_engine.dll)
  [string]$Src = "",   # engine source dir for caps.exe (default: src\engine)
  [string[]]$Lltd = @("default", "0.40", "0.50", "0.60"), # "default" = no lltd_f in tv_config
  [int[]]$K = @(0, 20),
  [string]$Work = ""   # scratch dir (default: build64\tv_tests\sweep)
)
$ErrorActionPreference = "Stop"
$root = Split-Path $PSScriptRoot -Parent
if (-not $Dll)  { $Dll  = Join-Path $root "build64\tv_engine.dll" }
if (-not $Src)  { $Src  = Join-Path $root "src\engine" }
if (-not $Work) { $Work = Join-Path $root "build64\tv_tests\sweep" }
if (Test-Path "C:\msys64\mingw64\bin") { $env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH }
New-Item -ItemType Directory -Force $Work | Out-Null
$d = $Work

& g++ -O2 -std=gnu++17 "$PSScriptRoot\step.cpp" -o "$d\step.exe" -static
if ($LASTEXITCODE -ne 0) { throw "step.exe build failed" }
& g++ -O3 -DNDEBUG -std=gnu++17 "-I$Src" "$PSScriptRoot\caps.cpp" -o "$d\caps.exe" -static
if ($LASTEXITCODE -ne 0) { throw "caps.exe build failed" }

foreach ($k in $K) {
  ""
  "k_mz_ay = $k"
  "lltd_f   | DLL out FL    FR       RL       RR     | slip FL   FR       RL       RR     | fz FL   FR      RL     RR     | trq_cap FL  FR       RL       RR"
  foreach ($l in $Lltd) {
    $dir = Join-Path $d "s${k}_$l"
    New-Item -ItemType Directory -Force $dir | Out-Null
    Copy-Item $Dll (Join-Path $dir "tv_engine.dll") -Force
    Copy-Item "$d\caps.exe" (Join-Path $dir "caps.exe") -Force
    $cfg = "k_mz_ay=$k"; if ($l -ne "default") { $cfg += "`nlltd_f=$l" }
    Set-Content -Encoding ascii (Join-Path $dir "tv_config.txt") $cfg
    $dll = Join-Path $dir "tv_engine.dll"
    $t = (& "$d\step.exe" $dll) -split ' '
    $s = (& "$d\step.exe" $dll slip 0.3 1.02) -split ' '
    $c = & (Join-Path $dir "caps.exe")
    $m = [regex]::Match($c, 'cap (\S+) (\S+) (\S+) (\S+)  fz (\S+) (\S+) (\S+) (\S+)  mz (\S+)')
    $g = $m.Groups
    "{0,-8} | {1,8} {2,8} {3,8} {4,8} | {5,8} {6,8} {7,8} {8,8} | {9,7} {10,7} {11,7} {12,7} | {13,8} {14,8} {15,8} {16,8}  mz={17}" -f `
      $l, $t[0], $t[1], $t[2], $t[3], $s[0], $s[1], $s[2], $s[3], `
      $g[5].Value, $g[6].Value, $g[7].Value, $g[8].Value, $g[1].Value, $g[2].Value, $g[3].Value, $g[4].Value, $g[9].Value
  }
}
