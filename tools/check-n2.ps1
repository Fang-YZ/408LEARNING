# ---------------------------------------------------------------------------
# check-n2.ps1 -- B-line N2 mechanical pre-grader (network architecture)
#
# What it checks (only things a machine can judge):
#   0. environment / previous stray processes
#   1. compile the student files with -Wall -Wextra, zero warnings required
#   2. concept example ex3_encapsulation_demo runs and prints the known values
#   3.真机 end-to-end: probe connects to the STUDENT's echo server, sends known
#      payloads, compares the reply byte for byte, and prints a HEX dump so the
#      coach can see exactly where a wrong byte sits
#
# Usage (from E:\learn408):
#   powershell -ExecutionPolicy Bypass -File .\tools\check-n2.ps1
#
# Expected student layout (see networks/N02_layered_arch/notes.md section 六):
#   homeworknet/N02/echo_server.c        (Q3: the ex1 skeleton, working)
#   homeworknet/N02/encap_demo.c         (Q2: the ex3 concept demo, working)
#   solutions/N02_layered_arch/ex3_upper_server.c (Q4 reference, coach copy)
#
# Output is ASCII English on purpose (Windows console + Chinese = mojibake).
# ---------------------------------------------------------------------------
$ErrorActionPreference = 'Continue'

$ROOT   = 'E:\learn408'
$SRC    = Join-Path $ROOT 'networks\N02_layered_arch\examples'
$HW     = Join-Path $ROOT 'homeworknet\N02'
$LOGDIR = Join-Path $ROOT 'grade\logs'
if (-not (Test-Path $LOGDIR)) { New-Item -ItemType Directory -Path $LOGDIR -Force | Out-Null }
$REPORT = Join-Path $LOGDIR 'N02-precheck-report.txt'

$script:pass = 0
$script:fail = 0
$script:lines = New-Object System.Collections.ArrayList

function Check([string]$id, [bool]$ok, [string]$detail) {
    $tag = if ($ok) { '[ PASS ]' } else { '[ FAIL ]' }
    Write-Host ("$tag $id  $detail") -ForegroundColor $(if ($ok) { 'Green' } else { 'Red' })
    if ($ok) { $script:pass++ } else { $script:fail++ }
    [void]$script:lines.Add("$tag $id  $detail")
}
function Info([string]$m) { Write-Host ("        $m") -ForegroundColor DarkGray; [void]$script:lines.Add("        $m") }
function Section([string]$t) {
    Write-Host ''
    Write-Host ('=' * 64) -ForegroundColor DarkGray
    Write-Host ("  $t") -ForegroundColor White
    Write-Host ('=' * 64) -ForegroundColor DarkGray
    [void]$script:lines.Add(''); [void]$script:lines.Add("== $t ==")
}

Section '0. ENVIRONMENT'
$gcc = Get-Command gcc -ErrorAction SilentlyContinue
Check 'ENV-1' ($null -ne $gcc) ("gcc: " + $(if ($gcc) { $gcc.Source } else { 'NOT FOUND' }))
Get-Process ex1_echo_server, ex1_upper_server, _n2_probe -ErrorAction SilentlyContinue |
    Stop-Process -Force -ErrorAction SilentlyContinue

if (-not (Test-Path $HW)) { New-Item -ItemType Directory -Path $HW -Force | Out-Null }

Section '1. CONCEPT EXAMPLE (ex3) -- known values'
$ex3 = Join-Path $SRC 'ex3_encapsulation_demo.c'
if (-not (Test-Path $ex3)) {
    Check 'CONCEPT-0' $false "missing source: $ex3"
} else {
    $o  = & gcc -Wall -Wextra -O2 -o (Join-Path $SRC 'ex3_encapsulation_demo.exe') $ex3 2>&1
    Check 'CONCEPT-1' ($LASTEXITCODE -eq 0 -and -not $o) 'ex3 compiles warning-free'
    foreach ($l in $o) { Info ([string]$l) }

    $out = (& (Join-Path $SRC 'ex3_encapsulation_demo.exe') 2>&1) -join "`n"
    # payload "GET /index.html HTTP/1.1" = 24 bytes -> frame 24+20+20+14+4 = 82 bytes = 656 bits
    $expect = @(
        @('CONCEPT-2', 'payload length            : 24 bytes',            'payload length 24'),
        @('CONCEPT-3', 'transport      segment                [TCP:20][DATA:24]', 'segment = TCP20 + DATA24'),
        @('CONCEPT-4', '[FRAME:14][IP:20][TCP:20][DATA:24][FCS:4]',       'frame header+trailer layout'),
        @('CONCEPT-5', 'physical       bit                    82 bytes -> 656 bits', '82 bytes / 656 bits'),
        @('CONCEPT-6', 'efficiency     : 24 / 82 = 29.27%',                'efficiency 29.27%'),
        @('CONCEPT-7', 'If the payload were 1 byte, how many bytes go on the wire?  ... 59', '1-byte payload -> 59 bytes')
    )
    foreach ($e in $expect) { Check $e[0] ($out -match [regex]::Escape($e[1])) $e[2] }
}

Section '2. TCP SKELETON EXAMPLE (ex1/ex2) -- student copies'
$hwServer = Join-Path $HW 'echo_server.c'
$hwDemo   = Join-Path $HW 'encap_demo.c'

# Q3: the student is expected to have the working echo server saved as echo_server.c.
# Fall back to the classroom copy so the socket regression can still run and be graded.
$useFallback = $false
if (-not (Test-Path $hwServer)) {
    Check 'COMPILE-Q3' $false "homeworknet/N02/echo_server.c not submitted -- grading the classroom copy instead"
    $hwServer = Join-Path $SRC 'ex1_echo_server.c'
    $useFallback = $true
} else {
    $o = & gcc -Wall -Wextra -O2 -o (Join-Path $HW 'echo_server.exe') $hwServer -lws2_32 2>&1
    Check 'COMPILE-Q3' ($LASTEXITCODE -eq 0 -and -not $o) ("student echo_server.c -> " + $(if ($o) { 'WARNINGS/ERRORS' } else { 'zero warnings' }))
    foreach ($l in $o) { Info ([string]$l) }
}

if ($useFallback) {
    $o = & gcc -Wall -Wextra -O2 -o (Join-Path $SRC 'ex1_echo_server.exe') $hwServer -lws2_32 2>&1
    Check 'COMPILE-Q3b' ($LASTEXITCODE -eq 0 -and -not $o) 'classroom echo_server.c compiles warning-free'
    $srvExe = Join-Path $SRC 'ex1_echo_server.exe'
} else {
    $srvExe = Join-Path $HW 'echo_server.exe'
}

if (Test-Path $hwDemo) {
    $o = & gcc -Wall -Wextra -O2 -o (Join-Path $HW 'encap_demo.exe') $hwDemo 2>&1
    Check 'COMPILE-Q2' ($LASTEXITCODE -eq 0 -and -not $o) 'student encap_demo.c compiles warning-free'
} else {
    Check 'COMPILE-Q2' $false 'homeworknet/N02/encap_demo.c not submitted yet'
}

# the client is a classroom example: compile it for the end-to-end check
$cliSrc = Join-Path $SRC 'ex2_echo_client.c'
$cliExe = Join-Path $SRC 'ex2_echo_client.exe'
$o = & gcc -Wall -Wextra -O2 -o $cliExe $cliSrc -lws2_32 2>&1
Check 'COMPILE-CLI' ($LASTEXITCODE -eq 0 -and -not $o) 'classroom client compiles warning-free'

Section '3. END-TO-END REGRESSION (real sockets, known payloads)'

$probeC = @'
#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char **argv)
{
    WSADATA wsa; SOCKET sd; struct sockaddr_in a; char buf[8192];
    int total = 0; DWORD tv = 4000; int i, port = atoi(argv[1]);
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
    printf("%s\nHEXLEN %d\nHEX ", buf, total);
    for (i = 0; i < total; i++) printf("%02X ", (unsigned char)buf[i]);
    printf("\n");
    fflush(stdout);
    closesocket(sd); WSACleanup();
    return 0;
}
'@
$probeSrc = Join-Path $env:TEMP '_n2_probe.c'
$probeExe = Join-Path $env:TEMP '_n2_probe.exe'
Set-Content -Path $probeSrc -Value $probeC -Encoding ASCII
$o = & gcc -Wall -Wextra -O2 -o $probeExe $probeSrc -lws2_32 2>&1
Check 'PROBE-1' ($LASTEXITCODE -eq 0) 'probe harness built'

function Wait-Port([int]$port, [int]$ms) {
    $sw = [System.Diagnostics.Stopwatch]::StartNew()
    while ($sw.ElapsedMilliseconds -lt $ms) {
        $c = New-Object System.Net.Sockets.TcpClient
        try { $c.Connect('127.0.0.1', $port); $c.Close(); return $true } catch { }
        Start-Sleep -Milliseconds 200
    }
    return $false
}

$outLog = Join-Path $env:TEMP '_n2_srv.out'
$errLog = Join-Path $env:TEMP '_n2_srv.err'
$p = Start-Process -FilePath $srvExe -ArgumentList '9000' -WorkingDirectory $HW -PassThru `
                   -RedirectStandardOutput $outLog -RedirectStandardError $errLog
$up = Wait-Port 9000 5000
Check 'SRV-1' $up 'echo server listening on 127.0.0.1:9000'

if ($up) {
    # known payload -> known answer (n = payload bytes + 1, the '\n' counted)
    $cases = @(
        @('tcp',          'ACK 4: tcp'),
        @('TCP',          'ACK 4: TCP'),
        @('a',            'ACK 2: a'),
        @('network 408',  'ACK 12: network 408'),
        @('',             'ACK 1: ')
    )
    $i = 0
    foreach ($c in $cases) {
        $i++
        $raw = (& $probeExe 9000 ($c[0] + "`n") 2>&1) -join "`n"
        $reply = ($raw -split "`n")[0].TrimEnd("`r")
        $hexlen = ''
        if ($raw -match 'HEXLEN (\d+)') { $hexlen = $Matches[1] }
        $hexdump = ''
        if ($raw -match 'HEX ([0-9A-F ]+)') { $hexdump = $Matches[1].Trim() }
        $ok = ($reply -eq $c[1])
        Check ("E2E-{0}" -f $i) $ok ("send [{0}] -> [{1}] | want [{2}]" -f $c[0], $reply, $c[1])
        if (-not $ok) {
            Info ("  reply length: {0} bytes" -f $hexlen)
            Info ("  HEX: {0}" -f $hexdump)
            Info  "  tip: 'ACK n' must count the '\n'  ->  empty line is ACK 1, 'tcp' is ACK 4"
        }
    }

    # the classroom client must agree with the server
    if (Test-Path $cliExe) {
        $cliRaw = (& $cliExe 127.0.0.1 9000 tcp 2>&1) -join "`n"
        Info '--- classroom client output (Q3 evidence) ---'
        foreach ($l in ($cliRaw -split "`n")) { if ($l.Trim()) { Info ("  " + $l.Trim()) } }
        $sentN = $null; if ($cliRaw -match 'SENT\s+(\d+)\s+bytes') { $sentN = [int]$Matches[1] }
        $ackN  = $null; if ($cliRaw -match 'ACK\s+(\d+):')          { $ackN  = [int]$Matches[1] }
        Check 'E2E-6' ($sentN -eq 4 -and $ackN -eq 4) ("client SENT=$sentN / server ACK=$ackN (both must be 4)")
        foreach ($need in @('CONNECTED 127.0.0.1:9000', 'SENT 4 bytes', 'REPLY: ACK 4: tcp', 'CLOSED')) {
            Check ('E2E-6:' + $need) ($cliRaw -match [regex]::Escape($need)) "client printed: $need"
        }
    }
}

Stop-Process -Id $p.Id -Force -ErrorAction SilentlyContinue
Start-Sleep -Milliseconds 400
Info '--- server console log ---'
if (Test-Path $outLog) { foreach ($l in (Get-Content $outLog)) { if ($l.Trim()) { Info ("  " + $l.Trim()) } } }

Remove-Item $probeSrc, $probeExe, $outLog, $errLog -ErrorAction SilentlyContinue
Get-Process ex1_echo_server, ex1_upper_server, _n2_probe -ErrorAction SilentlyContinue |
    Stop-Process -Force -ErrorAction SilentlyContinue

Section 'RESULT'
$total = $script:pass + $script:fail
Write-Host ("  PASS: {0}    FAIL: {1}    TOTAL: {2}" -f $script:pass, $script:fail, $total) -ForegroundColor $(if ($script:fail -eq 0) { 'Green' } else { 'Yellow' })
if ($script:fail -eq 0) {
    Write-Host '  MECHANICAL PRE-CHECK CLEAN.' -ForegroundColor Green
} else {
    Write-Host '  Each FAIL line above names the file and the exact value to fix.' -ForegroundColor Yellow
}
Write-Host ''
[void]$script:lines.Add("PASS $($script:pass) / FAIL $($script:fail) / TOTAL $total")
$script:lines | Set-Content -Path $REPORT -Encoding UTF8
Write-Host ("  report saved to: {0}" -f $REPORT) -ForegroundColor DarkGray
Write-Host ''
