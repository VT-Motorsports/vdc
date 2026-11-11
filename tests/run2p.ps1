# run2: fixed-case torques at k_mz_ay = 0/20/40, random-input and slip comparisons
# against optional reference DLLs, and internal trq_cap / fz / mz_des via caps.exe.
param(
  [string]$Dll = "",        # DLL under test (default: build64\tv_engine.dll)
  [string]$Src = "",        # engine source dir for caps.exe (default: src\engine)
  [string]$PreDll = "",     # optional reference DLL for the "pre" and random comparisons
  [string]$NoGuardDll = "", # optional reference DLL for the slip comparison
  [string]$Work = ""        # scratch dir for staged DLLs (default: build64\tv_tests\run2)
)
$ErrorActionPreference = "Stop"
$root = Split-Path $PSScriptRoot -Parent
if (-not $Dll)  { $Dll  = Join-Path $root "build64\tv_engine.dll" }
if (-not $Src)  { $Src  = Join-Path $root "src\engine" }
if (-not $Work) { $Work = Join-Path $root "build64\tv_tests\run2" }
if (Test-Path "C:\msys64\mingw64\bin") { $env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH }
New-Item -ItemType Directory -Force $Work | Out-Null
$d = $Work

& g++ -O2 -std=gnu++17 "$PSScriptRoot\step.cpp" -o "$d\step.exe" -static
if ($LASTEXITCODE -ne 0) { throw "step.exe build failed" }

function Stage($src, $name, $k) {
  $dir = Join-Path $d $name
  New-Item -ItemType Directory -Force $dir | Out-Null
  Copy-Item $src (Join-Path $dir "tv_engine.dll") -Force
  Set-Content -Encoding ascii (Join-Path $dir "tv_config.txt") "k_mz_ay=$k"
  Join-Path $dir "tv_engine.dll"
}
foreach ($k in 0, 20, 40) {
  $r = & "$d\step.exe" (Stage $Dll "n$k" $k)
  "new k=$k  $r"
}
if ($PreDll) {
  $p = & "$d\step.exe" (Stage $PreDll "p0" 0)
  "pre k=0  $p"
  $a = & "$d\step.exe" (Stage $Dll "n0" 0) rand
  $b = & "$d\step.exe" (Stage $PreDll "p0" 0) rand
  $diff = 0; for ($i = 0; $i -lt $a.Count; $i++) { if ($a[$i] -ne $b[$i]) { $diff++ } }
  "random k=0 cases: $($a.Count), differing: $diff"
} else {
  "pre / random comparison skipped (no -PreDll)"
}

"slip case: throttle 0.3, FR wheel speed +2%"
foreach ($k in 0, 20, 40) {
  $g = & "$d\step.exe" (Stage $Dll "n$k" $k) slip 0.3 1.02
  if ($NoGuardDll) {
    $u = & "$d\step.exe" (Stage $NoGuardDll "u$k" $k) slip 0.3 1.02
    "k=$k  guard $g   no-guard $u"
  } else {
    "k=$k  guard $g"
  }
}

& g++ -O3 -DNDEBUG -std=gnu++17 "-I$Src" "$PSScriptRoot\caps.cpp" -o "$d\n20\caps.exe" -static
if ($LASTEXITCODE -ne 0) { throw "caps.exe build failed" }
Copy-Item "$d\n20\caps.exe" "$d\n0\caps.exe"; Copy-Item "$d\n20\caps.exe" "$d\n40\caps.exe"
foreach ($k in 0, 20, 40) { $c = & "$d\n$k\caps.exe"; "k=$k $c" }
