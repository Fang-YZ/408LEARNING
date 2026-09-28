# ---------------------------------------------------------------------------
# check-n4.ps1 -- B-line N4 tester (transmission media & physical-layer devices)
#
# Compiles media_domain.c with the course standard and replays every
# "ARGS => EXPECTED SUBSTRING" case from tools/tests/n4_media_domain.txt.
#
# Usage (from E:\learn408):
#   powershell -ExecutionPolicy Bypass -File .\tools\check-n4.ps1
# ---------------------------------------------------------------------------
$ErrorActionPreference = 'Continue'

$ROOT  = 'E:\learn408'
$EX    = Join-Path $ROOT 'networks\N04_transmission_media\examples'
$SRC   = Join-Path $EX 'media_domain.c'
$TESTS = Join-Path $ROOT 'tools\tests\n4_media_domain.txt'
$LOGDIR = Join-Path $ROOT 'grade\logs'
if (-not (Test-Path $LOGDIR)) { New-Item -ItemType Directory -Path $LOGDIR -Force | Out-Null }

Write-Host ''
Write-Host ('=' * 64) -ForegroundColor DarkGray
Write-Host '  N4  media_domain  (shared vs dedicated medium, collision/broadcast domains)' -ForegroundColor White
Write-Host ('=' * 64) -ForegroundColor DarkGray

$exe = Join-Path $EX 'media_domain.exe'
$build = & gcc -Wall -Wextra -O2 -o $exe $SRC 2>&1
if ($LASTEXITCODE -ne 0 -or $build) {
    Write-Host '[ FAIL ] COMPILE  media_domain.c  (warnings or errors)' -ForegroundColor Red
    foreach ($l in $build) { Write-Host ('         ' + [string]$l) -ForegroundColor DarkGray }
    exit 1
}
Write-Host '[ PASS ] COMPILE  media_domain.c  (zero warnings, -Wall -Wextra)' -ForegroundColor Green

$pass = 0; $fail = 0; $lines = New-Object System.Collections.ArrayList
# read as UTF-8 explicitly: the data file carries Chinese comments and may lack a BOM,
# and Windows PowerShell 5.1 would otherwise decode it as ANSI and silently drop lines.
foreach ($line in (Get-Content $TESTS -Encoding UTF8)) {
    $t = $line.Trim()
    if ($t -eq '' -or $t.StartsWith('#')) { continue }
    $parts = $t -split '\s*=>\s*', 2
    if ($parts.Count -lt 2) { Write-Host ("[ SKIP ] bad format: $t") -ForegroundColor Yellow; continue }

    $argStr   = $parts[0].Trim()
    $expected = ($parts[1].Trim() -replace '\s+', ' ')
    $argList  = @($argStr -split '\s+' | Where-Object { $_ -ne '' })
    $actual   = (& $exe @argList 2>&1 | Out-String)
    $norm     = ($actual -replace '\s+', ' ').Trim()

    if ($norm.Contains($expected)) {
        $pass++
        Write-Host ("[ PASS ] args[$argStr]  expect: $expected") -ForegroundColor Green
        [void]$lines.Add("PASS args[$argStr] $expected")
    } else {
        $fail++
        Write-Host ("[ FAIL ] args[$argStr]") -ForegroundColor Red
        Write-Host ("         expect: $expected") -ForegroundColor DarkGray
        Write-Host ("         actual: $norm") -ForegroundColor DarkGray
        [void]$lines.Add("FAIL args[$argStr] expect: $expected | actual: $norm")
    }
}

Write-Host ''
Write-Host ("  RESULT: PASS $pass / FAIL $fail") -ForegroundColor $(if ($fail -eq 0) { 'Green' } else { 'Yellow' })
if ($fail -eq 0) { Write-Host '  ALL KNOWN-ANSWER CASES MATCH.' -ForegroundColor Green }
Write-Host ''
$lines | Set-Content -Path (Join-Path $LOGDIR 'N04-precheck-report.txt') -Encoding UTF8
if ($fail -gt 0) { exit 1 }
exit 0
