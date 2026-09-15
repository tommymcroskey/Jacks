# PowerShell 7. Child-process environment changes only; no installs.
param([ValidateSet('all', 'configure', 'build', 'test')][string]$Action = 'all')
$ErrorActionPreference = 'Stop'
$engRepo = Split-Path $PSScriptRoot -Parent
$engBuild = Join-Path $engRepo 'build/l001'
$engLocator = 'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $engLocator)) {
    throw 'Visual Studio locator missing. Ask engineering to adapt the verified setup.'
}
$engVs = & $engLocator -latest -version '[16.0,17.0)' -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $engVs) { throw 'No installed Visual Studio C++ toolchain found.' }
$engBin = Join-Path $engVs 'Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin'

function Invoke-EngTool([string]$Name, [string[]]$ToolArguments) {
    $engExe = Join-Path $engBin $Name
    if (-not (Test-Path -LiteralPath $engExe)) { throw "Missing tool: $engExe" }
    $engStart = [System.Diagnostics.ProcessStartInfo]::new()
    $engStart.FileName = $engExe
    $engStart.WorkingDirectory = $engRepo
    $engStart.UseShellExecute = $false
    foreach ($engArg in $ToolArguments) { $engStart.ArgumentList.Add($engArg) }
    # MSBuild 2019 fails if inherited environment contains both Path and PATH.
    $engKeys = @($engStart.Environment.Keys | Where-Object { $_ -ieq 'PATH' })
    $engValues = @($engKeys | ForEach-Object { $engStart.Environment[$_] })
    foreach ($engKey in $engKeys) { $engStart.Environment.Remove($engKey) | Out-Null }
    $engStart.Environment['PATH'] = ($engValues | Select-Object -Unique) -join ';'
    $engProcess = [System.Diagnostics.Process]::Start($engStart)
    $engProcess.WaitForExit()
    if ($engProcess.ExitCode -ne 0) { throw "$Name failed (exit $($engProcess.ExitCode))." }
}

if ($Action -in 'all', 'configure') {
    Invoke-EngTool 'cmake.exe' @('-S', $engRepo, '-B', $engBuild, '-G', 'Visual Studio 16 2019', '-A', 'x64', "-DCMAKE_GENERATOR_INSTANCE=$engVs")
}
if ($Action -in 'all', 'build') {
    Invoke-EngTool 'cmake.exe' @('--build', $engBuild, '--config', 'Debug')
}
if ($Action -in 'all', 'test') {
    Invoke-EngTool 'ctest.exe' @('--test-dir', $engBuild, '-C', 'Debug', '--output-on-failure', '--no-tests=error')
}
