# ---------------------------------------------------------------------------
# check-lesson-01.ps1 -- Lesson 1 mechanical pre-grader (coach tool)
# Run from anywhere:  powershell -ExecutionPolicy Bypass -File E:\learn408\grade\check-lesson-01.ps1
# ---------------------------------------------------------------------------
$ErrorActionPreference = 'Continue'
$OutputEncoding = [System.Text.Encoding]::UTF8
try { [Console]::OutputEncoding = [System.Text.Encoding]::UTF8 } catch { }

$ROOT    = Split-Path -Parent $PSScriptRoot          # E:\learn408
$CODE    = Join-Path $ROOT 'code\lesson-01'
$HW      = Join-Path $ROOT 'homework'
$REPORT  = Join-Path $ROOT 'homework\lesson-01-预检报告.txt'
$CARD    = Join-Path $HW 'lesson-01-答题卡.txt'

$script:pass = 0
$script:fail = 0
$script:checks = New-Object System.Collections.ArrayList

function Write-Section([string]$t) {
    Write-Host ''
    Write-Host ('=' * 64) -ForegroundColor DarkGray
    Write-Host ("  $t") -ForegroundColor White
    Write-Host ('=' * 64) -ForegroundColor DarkGray
}
function Check([string]$id, [bool]$ok, [string]$detail) {
    $tag = if ($ok) { '[ PASS ]' } else { '[ FAIL ]' }
    $color = if ($ok) { 'Green' } else { 'Red' }
    Write-Host ("$tag $id  $detail") -ForegroundColor $color
    if ($ok) { $script:pass++ } else { $script:fail++ }
    [void]$script:checks.Add(("$tag $id  $detail"))
}
function Info([string]$m) { Write-Host ("        $m") -ForegroundColor DarkGray }

# ---------------------------------------------------------------------------
Write-Section '0. ENVIRONMENT'
# ---------------------------------------------------------------------------
$gcc = Get-Command gcc -ErrorAction SilentlyContinue
Check 'ENV-1' ($null -ne $gcc) ("gcc found: " + $(if ($gcc) { $gcc.Source } else { 'NOT FOUND' }))
if ($gcc) { Info ((& gcc --version 2>&1 | Select-Object -First 1) -join '') }

Get-Process server_echo, server_echo_upper, _hw_probe -ErrorAction SilentlyContinue |
    Stop-Process -Force -ErrorAction SilentlyContinue

# ---------------------------------------------------------------------------
Write-Section '1. ANSWER CARD PRE-CHECK'
# ---------------------------------------------------------------------------
if (-not (Test-Path $CARD)) {
    Check 'CARD-1' $false "answer card not found: $CARD"
} else {
    $cardLines = Get-Content $CARD
    $blank = @()
    for ($i = 0; $i -lt $cardLines.Count; $i++) {
        $t = ($cardLines[$i] -replace '\s', '')
        if ($t -match '^（粘贴在这里）$' -or $t -match '^（有就贴，没有写"无"）$') {
            $blank += "line $($i + 1)"
        }
    }
    Check 'CARD-1' $true ("answer card exists: {0} lines" -f $cardLines.Count)
    Check 'CARD-2' ($blank.Count -eq 0) ("unfilled fields: " + $(if ($blank.Count) { $blank.Count.ToString() + ' -> ' + ($blank -join ', ') } else { 'none' }))
    if ($blank.Count -gt 0) { Info 'fill every （粘贴在这里） field, then re-run this script' }

    # did the student fill in the expected n column?  look for the 5 answer rows
    $rows = @(Select-String -Path $CARD -Pattern '^\s*[1-5]\s*\|' -AllMatches)
    Check 'CARD-3' ($rows.Count -ge 5) ("Q2 answer rows detected: $($rows.Count) (need >= 5)")
    foreach ($r in $rows) { Info ("card line {0}: {1}" -f $r.LineNumber, $r.Line.Trim()) }
}

# ---------------------------------------------------------------------------
Write-Section '2. COMPILE STUDENT CODE (must be warning-free)'
# ---------------------------------------------------------------------------
Set-Location $CODE -ErrorAction SilentlyContinue

$srvOut = & gcc -Wall -O2 -o server_echo.exe server_echo.c -lws2_32 2>&1
Check 'BUILD-1' ($LASTEXITCODE -eq 0 -and -not $srvOut) ("server_echo.c -> exit $LASTEXITCODE, " + $(if ($srvOut) { 'WARNINGS/ERRORS' } else { 'clean' }))
foreach ($l in $srvOut) { Info ([string]$l) }

$cliSrc = Get-Content (Join-Path $CODE 'client_echo.c') -Raw
$todoLeft = ([regex]::Matches($cliSrc, '---- TODO')).Count
Check 'BUILD-2' ($todoLeft -eq 0) ("client_echo.c remaining TODO blocks: $todoLeft (must be 0)")

$cliOut = & gcc -Wall -O2 -o client_echo.exe client_echo.c -lws2_32 2>&1
Check 'BUILD-3' ($LASTEXITCODE -eq 0 -and -not $cliOut) ("client_echo.c -> exit $LASTEXITCODE, " + $(if ($cliOut) { 'WARNINGS/ERRORS' } else { 'clean' }))
foreach ($l in $cliOut) { Info ([string]$l) }

# student's homework server #4
$upperC = Join-Path $HW 'server_echo_upper.c'
if (-not (Test-Path $upperC)) {
    Check 'BUILD-4' $false "Q4 source missing: $upperC"
    $hasUpper = $false
} else {
    $UpperOut = & gcc -Wall -O2 -o (Join-Path $HW 'server_echo_upper.exe') $upperC -lws2_32 2>&1
    $hasUpper = ($LASTEXITCODE -eq 0 -and -not $UpperOut)
    Check 'BUILD-4' $hasUpper ("Q4 server_echo_upper.c -> exit $LASTEXITCODE, " + $(if ($UpperOut) { 'WARNINGS/ERRORS' } else { 'clean' }))
    foreach ($l in $UpperOut) { Info ([string]$l) }
}

# ---------------------------------------------------------------------------
Write-Section '3. END-TO-END REGRESSION AGAINST *YOUR* SERVER (real sockets)'
# ---------------------------------------------------------------------------
$probeC = @'
#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char **argv)
{
    WSADATA wsa; SOCKET sd; struct sockaddr_in a; char buf[8192];
    int total = 0; DWORD tv = 4000; char *hex; int i;
    int port = atoi(argv[1]);
    WSAStartup(MAKEWORD(2, 2), &wsa);
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
    memset(&a, 0, sizeof(a));
    a.sin_family = AF_INET; a.sin_port = htons((unsigned short)port);
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    if (connect(sd, (struct sockaddr *)&a, sizeof(a)) != 0) { printf("ERR connect %d\n", WSAGetLastError()); return 1; }
    send(sd, argv[2], (int)strlen(argv[2]), 0);
    for (;;) {
        char ch; int r = recv(sd, &ch, 1, 0);
        if (r <= 0) { printf("ERR recv %d\n", WSAGetLastError()); return 2; }
        if (ch == '\n') break;
        if (ch != '\r' && total < 8000) buf[total++] = ch;
    }
    buf[total] = 0;
    printf("%s\n", buf);
    fflush(stdout);
    hex = getenv("HW_PROBE_HEX");
    if (hex && *hex == '1') {
        printf("HEX ");
        for (i = 0; i < total; i++) printf("%02X ", (unsigned char)buf[i]);
        printf("\n");
    }
    closesocket(sd); WSACleanup();
    return 0;
}
'@
$probeSrc = Join-Path $CODE '_hw_probe.c'
$probeExe = Join-Path $CODE '_hw_probe.exe'
Set-Content -Path $probeSrc -Value $probeC -Encoding ASCII
$probeBuild = & gcc -Wall -O2 -o $probeExe $probeSrc -lws2_32 2>&1
Check 'PROBE-1' ($LASTEXITCODE -eq 0) ("probe harness built: exit $LASTEXITCODE")

$cases  = @('tcp', 'TCP', 'a', 'network 408', '')
$gnums  = @(4, 4, 2, 12, 1)
$gtext  = @('tcp', 'TCP', 'a', 'network 408', '')

function Wait-Port([int]$port, [int]$ms) {
    $sw = [System.Diagnostics.Stopwatch]::StartNew()
    while ($sw.ElapsedMilliseconds -lt $ms) {
        $c = New-Object System.Net.Sockets.TcpClient
        try { $c.Connect('127.0.0.1', $port); $c.Close(); return $true } catch { }
        Start-Sleep -Milliseconds 200
    }
    return $false
}

function Start-TestServer([string]$exePath, [string]$workDir, [int]$port, [string]$stdout, [string]$stderr) {
    $p = Start-Process -FilePath $exePath -ArgumentList $port.ToString() -WorkingDirectory $workDir `
                       -PassThru -RedirectStandardOutput $stdout -RedirectStandardError $stderr
    $up = Wait-Port $port 5000
    return @{ Proc = $p; Up = $up }
}

function Stop-TestServer($srv) {
    if ($srv -and $srv.Proc) { Stop-Process -Id $srv.Proc.Id -Force -ErrorAction SilentlyContinue }
    Get-Process server_echo, server_echo_upper -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
    Start-Sleep -Milliseconds 400
}

# --- 3a. base server on 9000 ---
$srv1 = Start-TestServer (Join-Path $CODE 'server_echo.exe') $CODE 9000 (Join-Path $ROOT 'grade\_srv9000.out') (Join-Path $ROOT 'grade\_srv9000.err')
Check 'SRV-1' $srv1.Up 'server_echo.exe listening on 127.0.0.1:9000'
if ($srv1.Up) {
    for ($i = 0; $i -lt 5; $i++) {
        $raw = (& $probeExe 9000 ($cases[$i] + "`n") 2>&1) -join ''
        $raw = $raw.TrimEnd("`r", "`n")
        $expected = "ACK $($gnums[$i]): $($gtext[$i])"
        $ok = ($raw -eq $expected)
        Check ("E2E-1.{0}" -f ($i + 1)) $ok ("send [{0}] -> got [{1}] | want [{2}]" -f $cases[$i], $raw, $expected)

        if ($i -eq 4 -and $raw -ne $expected) {
            $hx = $env:HW_PROBE_HEX; $env:HW_PROBE_HEX = '1'
            $rawHex = (& $probeExe 9000 "`n" 2>&1) -join "`n"
            $env:HW_PROBE_HEX = $hx
            Info 'HEX DUMP of the empty-line reply (for coach analysis):'
            foreach ($l in ($rawHex -split "`n")) { if ($l.Trim()) { Info ("  " + $l.Trim()) } }
        }
    }

    # does the CLIENT's own SENT report agree with the server's ACK?
    $cliRaw = (& (Join-Path $CODE 'client_echo.exe') 127.0.0.1 9000 tcp 2>&1) -join "`n"
    Write-Host '        --- client_echo.exe output (Q1 evidence) ---' -ForegroundColor DarkGray
    foreach ($l in ($cliRaw -split "`n")) { if ($l.Trim()) { Write-Host ("        " + $l.Trim()) -ForegroundColor DarkGray } }
    $sentN = $null
    if ($cliRaw -match 'SENT\s+(\d+)\s+bytes') { $sentN = [int]$Matches[1] }
    $cliAck = $null
    if ($cliRaw -match 'ACK\s+(\d+):')      { $cliAck = [int]$Matches[1] }
    Check 'E2E-2' ($sentN -eq 4 -and $cliAck -eq 4) ("client SENT=$sentN / server ACK=$cliAck (both must be 4 for 'tcp')")
    $need = @('CONNECTED 127.0.0.1:9000', 'SENT 4 bytes', 'REPLY: ACK 4: tcp', 'CLOSED')
    foreach ($n in $need) { Check ('E2E-3:' + $n) ($cliRaw -match [regex]::Escape($n)) "client printed: $n" }
}
Stop-TestServer $srv1
Write-Host '        --- base server console log ---' -ForegroundColor DarkGray
$log = Join-Path $ROOT 'grade\_srv9000.out'
if (Test-Path $log) { foreach ($l in (Get-Content $log)) { if ($l.Trim()) { Write-Host ("        " + $l.Trim()) -ForegroundColor DarkGray } } }

# --- 3b. student's uppercase server on 9001 ---
if ($hasUpper) {
    $srv2 = Start-TestServer (Join-Path $HW 'server_echo_upper.exe') $HW 9001 (Join-Path $ROOT 'grade\_srv9001.out') (Join-Path $ROOT 'grade\_srv9001.err')
    Check 'SRV-2' $srv2.Up 'server_echo_upper.exe listening on 127.0.0.1:9001'
    if ($srv2.Up) {
        $ucases = @('tcp', 'AbC 123!', 'hello 408')
        $uexp   = @('ACK 4: TCP', 'ACK 9: ABC 123!', 'ACK 10: HELLO 408')
        for ($i = 0; $i -lt 3; $i++) {
            $raw = (& $probeExe 9001 ($ucases[$i] + "`n") 2>&1) -join ''
            $raw = $raw.TrimEnd("`r", "`n")
            Check ("E2E-4.{0}" -f ($i + 1)) ($raw -eq $uexp[$i]) ("send [{0}] -> got [{1}] | want [{2}]" -f $ucases[$i], $raw, $uexp[$i])
        }
    }
    Stop-TestServer $srv2
}

# ---------------------------------------------------------------------------
Write-Section '4. CLEANUP'
# ---------------------------------------------------------------------------
Remove-Item $probeSrc, $probeExe -ErrorAction SilentlyContinue
Get-Process server_echo, server_echo_upper -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
Info 'temporary probe files removed'

# ---------------------------------------------------------------------------
Write-Section 'RESULT'
# ---------------------------------------------------------------------------
$total = $script:pass + $script:fail
Write-Host ("  PASS: {0}    FAIL: {1}    TOTAL: {2}" -f $script:pass, $script:fail, $total) -ForegroundColor $(if ($script:fail -eq 0) { 'Green' } else { 'Yellow' })
Write-Host ''
if ($script:fail -eq 0) {
    Write-Host '  MECHANICAL PRE-CHECK CLEAN.' -ForegroundColor Green
    Write-Host '  Paste this whole output back to the coach, then say: "Lesson 1 homework submitted".' -ForegroundColor Green
} else {
    Write-Host '  Some checks FAILED. Each FAIL line above tells you exactly what to fix,' -ForegroundColor Yellow
    Write-Host '  then re-run this script. Paste the output to the coach if you are stuck.' -ForegroundColor Yellow
}
Write-Host ''

$header = @(
    'Lesson 1 mechanical pre-check report',
    ('generated: ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss')),
    ("PASS {0} / FAIL {1} / TOTAL {2}" -f $script:pass, $script:fail, $total),
    ''
)
$header + $script:checks | Set-Content -Path $REPORT -Encoding UTF8
Write-Host ("  report saved to: {0}" -f $REPORT) -ForegroundColor DarkGray
Write-Host ''
