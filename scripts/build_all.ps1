# Build both main.exe and run_tests.exe
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptDir "..")
Push-Location $repoRoot

Write-Host "Building main.exe..."
& .\scripts\build_main.ps1
if ($LASTEXITCODE -ne 0) {
    Write-Error "build_main failed with exit code $LASTEXITCODE"
    Pop-Location
    exit $LASTEXITCODE
}

Write-Host "Building run_tests.exe..."
& .\scripts\build_tests.ps1
if ($LASTEXITCODE -ne 0) {
    Write-Error "build_tests failed with exit code $LASTEXITCODE"
    Pop-Location
    exit $LASTEXITCODE
}

Write-Host "All builds succeeded."
Pop-Location
