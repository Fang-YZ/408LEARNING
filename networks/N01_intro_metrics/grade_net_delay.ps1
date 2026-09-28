# grade_net_delay.ps1 — Auto-grader for N1 H4 (net_delay.c)
# Usage:  powershell -File grade_net_delay.ps1 [-Path homework/N01/net_delay.c]
# Defaults to homework/N01/net_delay.c when the optional path is omitted.
# Checks: 1) gcc -Wall -Wextra zero-warning compile
#         2) MAIN + B1..B4 test sets, key numeric lines compared to expected
# Output is ASCII English on purpose.
param([string]$Path = "")

$keys = @('send_delay','propagation_delay','total_delay',
          'delay_bandwidth_product','rtt_propagation')

if ($Path -eq "") { $Path = "E:\learn408\homework\N01\net_delay.c" }
$full = Join-Path (Get-Location) $Path
if (-not (Test-Path $Path)) {
    Write-Output "NO_SUBMISSION_FILE: $Path"
    exit 2
}

$exe = Join-Path $env:TEMP "net_delay_grade.exe"

# ---- compile -------------------------------------------------------------
$errs = & gcc -Wall -Wextra $Path -o $exe 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Output "COMPILE_FAILED"
    $errs | ForEach-Object { Write-Output $_ }
    exit 1
}
if ($errs) { Write-Output "WARNINGS_FOUND"; $errs | ForEach-Object { Write-Output $_ } }
else       { Write-Output "compile: zero warnings (-Wall -Wextra)" }

# ---- expected key lines (numeric part after '=') --------------------------
$E = @{}
$E['MAIN'] = @('0.100000000 s','0.005000000 s','0.105000000 s',
               '500000.000000 bit','0.010000000 s')
$E['B1']   = @('0.000000010 s','0.005000000 s','0.005000010 s',
               '500000.000000 bit','0.010000000 s')
$E['B2']   = @('0.100000000 s','0.000000000 s','0.100000000 s',
               '0.000000 bit','0.000000000 s')
$E['B3']   = @('1.000000000 s','0.000000000 s','1.000000000 s',
               '0.000000 bit','0.000000000 s')
$E['B4']   = @('0.100000000 s','1.000000000 s','1.100000000 s',
               '100000000.000000 bit','2.000000000 s')

$tests = @{
    'MAIN' = '10000000 100000000 1000000 200000000'
    'B1'   = '1 100000000 1000000 200000000'
    'B2'   = '10000000 100000000 0 200000000'
    'B3'   = '1 1 0 200000000'
    'B4'   = '10000000 100000000 200000000 200000000'
}

$passCount = 0
foreach ($name in @('MAIN','B1','B2','B3','B4')) {
    $out = $tests[$name] | & $exe
    $got = @()
    foreach ($line in $out) {
        $first = ($line -split ' ')[0]
        if ($keys -contains $first) {
            if ($line -match '=\s*(.+)$') { $got += $matches[1].Trim() }
            else                          { $got += '' }
        }
    }
    if (($got -join '|') -eq ($E[$name] -join '|')) {
        Write-Output "PASS $name"
        $passCount++
    }
    else {
        Write-Output "FAIL $name"
        Write-Output "  expected: $($E[$name] -join ' | ')"
        Write-Output "  got     : $($got -join ' | ')"
    }
}
Write-Output "RESULT: $passCount/5 sets passed"
exit 0
