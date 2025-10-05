# Build main.exe (interpreter)
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptDir "..")
Push-Location $repoRoot

# Where to put build artifacts
$buildDir = Join-Path $repoRoot "build"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }

if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Error "g++ not found in PATH. Install MinGW or add g++ to PATH."
    Pop-Location
    exit 1
}

Write-Host "Compiling main.exe into $buildDir..."
$out = Join-Path $buildDir "main.exe"
g++ -std=c++17 -I. src\main.cpp src\executor\executor.cpp src\lexer\lexer.cpp src\parser\parser.cpp src\interpreter\interpreter.cpp src\utility\utility.cpp -o "$out"
if ($LASTEXITCODE -ne 0) { Write-Error "Build failed (main.exe)"; Pop-Location; exit $LASTEXITCODE }
Write-Host "Built $out"
Pop-Location
