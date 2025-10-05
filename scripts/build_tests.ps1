# Build run_tests.exe (in-process test runner)
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptDir "..")
Push-Location $repoRoot

# output folder
$buildDir = Join-Path $repoRoot "build"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }

if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Error "g++ not found in PATH. Install MinGW or add g++ to PATH."
    Pop-Location
    exit 1
}

Write-Host "Compiling run_tests.exe into $buildDir (defines RUN_TESTS)..."
$out = Join-Path $buildDir "run_tests.exe"
g++ -std=c++17 -I. -DRUN_TESTS src\executor\executor.cpp src\lexer\lexer.cpp src\parser\parser.cpp src\interpreter\interpreter.cpp src\utility\utility.cpp tests\src\runTests.cpp -o "$out"
if ($LASTEXITCODE -ne 0) { Write-Error "Build failed (run_tests.exe)"; Pop-Location; exit $LASTEXITCODE }
Write-Host "Built $out"
Pop-Location
