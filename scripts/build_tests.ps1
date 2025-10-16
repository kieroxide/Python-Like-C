# Build run_tests.exe (tests)
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = (Resolve-Path (Join-Path $scriptDir "..")).Path
Push-Location $repoRoot

# prepare folders
$buildDir = Join-Path $repoRoot "build"
$objDir = Join-Path $buildDir "obj"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir -Force | Out-Null }
if (-not (Test-Path $objDir)) { New-Item -ItemType Directory -Path $objDir -Force | Out-Null }

# check g++
if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Error "g++ not found in PATH. Install MinGW-w64 / MSYS2 or add g++ to PATH."
    Pop-Location; exit 1
}
$gccVersion = (& g++ -dumpversion).Trim()
try { $major = [int]($gccVersion.Split('.')[0]) } catch { $major = 0 }
if ($major -lt 7) {
    Write-Warning "g++ version $gccVersion looks older than recommended (>=7). Builds may still work, but C++17 features might be missing."
}

Write-Host "Using g++ version $gccVersion"

# compiler flags as arrays
$cxx = "g++"
$cxxflags = @("-std=c++17", "-O2", "-g")
$includeFlags = @("-I.", "-DRUN_TESTS")

# gather source files under src
$srcFiles = Get-ChildItem -Path (Join-Path $repoRoot "src") -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }

# add test runner(s)
$testRunners = Get-ChildItem -Path (Join-Path $repoRoot "tests" "src") -Recurse -Filter *.cpp -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName }
$allSources = @($srcFiles) + @($testRunners)

if ($allSources.Count -eq 0) {
    Write-Error "No source files found for tests"
    Pop-Location; exit 1
}

function Get-ObjPath($srcFull) {
    $rel = $srcFull.Substring($repoRoot.Length).TrimStart('\', '/')
    $san = ($rel -replace '[\\/:]', '_') -replace '[^A-Za-z0-9_.-]', '_'
    return Join-Path $objDir ($san + ".o")
}

# compile
foreach ($src in $allSources) {
    $obj = Get-ObjPath $src
    if (-not (Test-Path $obj) -or (Get-Item $src).LastWriteTime -gt (Get-Item $obj).LastWriteTime) {
        Write-Host "Compiling $src -> $obj"
        & $cxx $cxxflags $includeFlags -c $src -o $obj
        if ($LASTEXITCODE -ne 0) { Write-Error "Failed compiling $src"; Pop-Location; exit $LASTEXITCODE }
    }
}

# link
$out = Join-Path $buildDir "run_tests.exe"
Write-Host "Linking to $out"
$objs = Get-ChildItem $objDir -Filter *.o | ForEach-Object { $_.FullName }
& $cxx $cxxflags $objs -o $out
if ($LASTEXITCODE -ne 0) { Write-Error "Linking failed"; Pop-Location; exit $LASTEXITCODE }

Write-Host "Built $out successfully."
Pop-Location