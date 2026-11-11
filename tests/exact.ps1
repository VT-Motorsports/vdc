# Bit-exact (%a hex) comparison of a baseline DLL against the DLL under test, with
# the default tv_config (k_mz_ay only) at k_mz_ay = 0/20/40: fixed case, slip case,
# 2000 random inputs, and the internal trq_cap / fz / mz_des / trq via caps.exe.
# Exits 1 if anything differs.
param(
  [Parameter(Mandatory = $true)][string]$BaseDll, # baseline tv_engine.dll
  [string]$BaseRef = "HEAD",  # git ref whose src/ the baseline was built from (for caps.exe)
  [string]$Dll = "",          # DLL under test (default: build64\tv_engine.dll)
  [string]$Src = "",          # engine source dir for the test caps.exe (default: src\engine)
  [string]$Work = ""          # scratch dir (default: build64\tv_tests\exact)
)
$ErrorActionPreference = "Stop"
$root = Split-Path $PSScriptRoot -Parent
if (-not $Dll)  { $Dll  = Join-Path $root "build64\tv_engine.dll" }
if (-not $Src)  { $Src  = Join-Path $root "src\engine" }
if (-not $Work) { $Work = Join-Path $root "build64\tv_tests\exact" }
if (Test-Path "C:\msys64\mingw64\bin") { $env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH }
New-Item -ItemType Directory -Force $Work | Out-Null
$d = $Work

# Baseline sources from git (written to a file: PS 5.1 pipes would corrupt the tar)
$baseTree = Join-Path $d "base_src"
if (Test-Path $baseTree) { Remove-Item -Recurse -Force $baseTree }
New-Item -ItemType Directory -Force $baseTree | Out-Null
& git -C $root archive -o "$d\base_src.tar" $BaseRef src
if ($LASTEXITCODE -ne 0) { throw "git archive $BaseRef failed" }
& tar -xf "$d\base_src.tar" -C $baseTree
if ($LASTEXITCODE -ne 0) { throw "tar extract failed" }
$baseSrc = Join-Path $baseTree "src\engine"

& g++ -O2 -std=gnu++17 -DSTEP_HEX "$PSScriptRoot\step.cpp" -o "$d\step_hex.exe" -static
if ($LASTEXITCODE -ne 0) { throw "step_hex.exe build failed" }
& g++ -O3 -DNDEBUG -std=gnu++17 -DCAPS_HEX "-I$baseSrc" "$PSScriptRoot\caps.cpp" -o "$d\caps_hex_base.exe" -static
if ($LASTEXITCODE -ne 0) { throw "caps_hex_base.exe build failed" }
& g++ -O3 -DNDEBUG -std=gnu++17 -DCAPS_HEX "-I$Src" "$PSScriptRoot\caps.cpp" -o "$d\caps_hex_new.exe" -static
if ($LASTEXITCODE -ne 0) { throw "caps_hex_new.exe build failed" }

function Stage($src, $name, $k, $exe) {
  $dir = Join-Path $d $name
  New-Item -ItemType Directory -Force $dir | Out-Null
  Copy-Item $src (Join-Path $dir "tv_engine.dll") -Force
  Copy-Item $exe (Join-Path $dir "caps_hex.exe") -Force
  Set-Content -Encoding ascii (Join-Path $dir "tv_config.txt") "k_mz_ay=$k"
  Join-Path $dir "tv_engine.dll"
}
$total = 0; $bad = 0
foreach ($k in 0, 20, 40) {
  $bd = Stage $BaseDll "xb$k" $k "$d\caps_hex_base.exe"
  $nd = Stage $Dll     "xn$k" $k "$d\caps_hex_new.exe"
  foreach ($mode in @(@(), @("slip", "0.3", "1.02"), @("rand"))) {
    $a = @(& "$d\step_hex.exe" $bd @mode)
    $b = @(& "$d\step_hex.exe" $nd @mode)
    $n = 0; for ($i = 0; $i -lt [Math]::Max($a.Count, $b.Count); $i++) { if ($a[$i] -ne $b[$i]) { $n++ } }
    $label = if ($mode.Count) { $mode[0] } else { "default" }
    "k=$k  $label  lines=$($a.Count)/$($b.Count)  differing=$n"
    $total += $a.Count; $bad += $n
  }
  $ca = & "$d\xb$k\caps_hex.exe"; $cb = & "$d\xn$k\caps_hex.exe"
  "k=$k  caps(fz/trq_cap/mz/trq) identical=$($ca -eq $cb)"
  $total++; if ($ca -ne $cb) { $bad++ }
}
"TOTAL lines compared: $total  differing: $bad"
if ($bad -ne 0) { exit 1 }
