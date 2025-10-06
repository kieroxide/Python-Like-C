
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptDir "..")
Push-Location $repoRoot

# output folder
$buildDir = Join-Path $repoRoot "build"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir -Force | Out-Null }

if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Error "g++ not found in PATH. Install MinGW or add g++ to PATH."
    Pop-Location
    exit 1
}

# Build run_tests.exe
Write-Host "Compiling run_tests.exe into $buildDir"
$out = Join-Path $buildDir "run_tests.exe"
g++ -std=c++17 -I. -DRUN_TESTS src/Executor/executor.cpp src/Lexer/Lexer.cpp src/Parser/Parser.cpp src/Interpreter/Interpreter.cpp src/Scope/Scope.cpp src/Utility/utility.cpp tests/src/runTests.cpp -o "$out"
if ($LASTEXITCODE -ne 0) { Write-Error "Build failed (run_tests.exe)"; Pop-Location; exit $LASTEXITCODE }
Write-Host "Built $out"
Pop-Location
