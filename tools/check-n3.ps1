# ---------------------------------------------------------------------------
# check-n3.ps1 -- B-line N3 tester (physical layer: channel capacity & switching)
#
# Why not tools/check.ps1?  That one feeds the program through stdin, but the
# N3 example programs take COMMAND-LINE ARGUMENTS.  This wrapper does the right
# thing: compile with the course standard (-Wall -Wextra, zero warnings) and
# then run each "ARGS => EXPECTED SUBSTRING" case with those arguments.
#
# Usage (from E:\learn408):
#   powershell -ExecutionPolicy Bypass -File .\tools\check-n3.ps1
#
# Test data (one case per line, '#' starts a comment):
#   tools/tests/n3_channel_capacity.txt
#   tools/tests/n3_switch_compare.txt
# ---------------------------------------------------------------------------
$ErrorActionPreference = 'Continue'

$ROOT = 'E:\learn408'
$EX   = Join-Path $ROOT 'networks\N03_physical_layer\examples'

function Invoke-Suite {
    param(
        [string]$Name,
        [string]$Source,
        [string]$Tests,
        [string]$ExtraLib = ''
    )

    Write-Host ''
    Write-Host ('=' * 64) -ForegroundColor DarkGray
    Write-Host ("  $Name") -ForegroundColor White
    Write-Host ('=' * 64) -ForegroundColor DarkGray

    $exe = Join-Path $EX (($Source -replace '\.c$', '') + '.exe')
    $gccArgs = @('-Wall', '-Wextra', '-O2', '-o', $exe, (Join-Path $EX $Source))
    if ($ExtraLib -ne '') { $gccArgs += $ExtraLib }
    $build = & gcc @gccArgs 2>&1
    if ($LASTEXITCODE -ne 0 -or $build) {
        Write-Host ("[ FAIL ] COMPILE  $Source -> " + $(if ($build) { 'warnings/errors' } else { "exit $LASTEXITCODE" })) -ForegroundColor Red
        foreach ($l in $build) { Write-Host ("         " + [string]$l) -ForegroundColor DarkGray }
        return @{ Pass = 0; Fail = 1 }
    }
    Write-Host ("[ PASS ] COMPILE  $Source  (zero warnings, -Wall -Wextra)") -ForegroundColor Green

    if (-not (Test-Path $Tests)) {
        Write-Host ("[ FAIL ] TESTDATA $Tests not found") -ForegroundColor Red
        return @{ Pass = 0; Fail = 1 }
    }

    $pass = 0; $fail = 0
    # read as UTF-8 explicitly: these files carry Chinese comments and may lack a BOM,
    # and Windows PowerShell 5.1 would otherwise decode them as ANSI and drop lines.
    foreach ($line in (Get-Content $Tests -Encoding UTF8)) {
        $t = $line.Trim()
        if ($t -eq '' -or $t.StartsWith('#')) { continue }

        $parts = $t -split '\s*=>\s*', 2
        if ($parts.Count -lt 2) {
            Write-Host ("[ SKIP ] bad format: $t") -ForegroundColor Yellow
            continue
        }
        $argStr   = $parts[0].Trim()
        $expected = ($parts[1].Trim() -replace '\s+', ' ')

        $argList = @($argStr -split '\s+' | Where-Object { $_ -ne '' })
        $actual  = (& $exe @argList 2>&1 | Out-String)
        $norm    = ($actual -replace '\s+', ' ').Trim()

        if ($norm.Contains($expected)) {
            $pass++
            Write-Host ("[ PASS ] args[$argStr]  expect: $expected") -ForegroundColor Green
        } else {
            $fail++
            Write-Host ("[ FAIL ] args[$argStr]") -ForegroundColor Red
            Write-Host ("         expect: $expected") -ForegroundColor DarkGray
            Write-Host ("         actual: $norm") -ForegroundColor DarkGray
        }
    }

    Write-Host ("  --- $Name : $pass passed, $fail failed") -ForegroundColor $(if ($fail -eq 0) { 'Green' } else { 'Yellow' })
    return @{ Pass = $pass; Fail = $fail }
}

$r1 = Invoke-Suite -Name 'N3-A  channel_capacity  (Nyquist + Shannon)' `
                   -Source 'channel_capacity.c' `
                   -Tests  (Join-Path $ROOT 'tools\tests\n3_channel_capacity.txt') `
                   -ExtraLib '-lm'

$r2 = Invoke-Suite -Name 'N3-B  switch_compare  (circuit/message/packet)' `
                   -Source 'switch_compare.c' `
                   -Tests  (Join-Path $ROOT 'tools\tests\n3_switch_compare.txt')

$totalPass = $r1.Pass + $r2.Pass
$totalFail = $r1.Fail + $r2.Fail
Write-Host ''
Write-Host ('=' * 64) -ForegroundColor DarkGray
Write-Host ("  RESULT: PASS $totalPass / FAIL $totalFail") -ForegroundColor $(if ($totalFail -eq 0) { 'Green' } else { 'Yellow' })
if ($totalFail -eq 0) {
    Write-Host '  ALL KNOWN-ANSWER CASES MATCH.' -ForegroundColor Green
} else {
    Write-Host '  Each FAIL line shows the args, the expected value and what came out.' -ForegroundColor Yellow
}
Write-Host ''
if ($totalFail -gt 0) { exit 1 }
exit 0
