<#
    token_report.ps1 -- summarize DSH token usage for one workspace.

    Usage:
        powershell -ExecutionPolicy Bypass -File E:\learn408\tools\token_report.ps1
        powershell -ExecutionPolicy Bypass -File E:\learn408\tools\token_report.ps1 -Workspace E:\learn408

    Data source (read only, nothing is written):
        <DSH_HOME>\storages\session_projcache\sessions\*.json
    DSH_HOME defaults to %USERPROFILE%\.dsh

    Reading the report:
        Input      = prompt tokens that missed the cache (billed full price)
        CacheRead  = prompt tokens served from cache (about 1/10 price)
        Output     = generated tokens (Reasoning is the thinking part of Output)
        Cost       = DSH's own estimate for that session (USD)
#>
param(
    [string]$Workspace = 'E:\learn408'
)

[Console]::OutputEncoding = [System.Text.Encoding]::UTF8

$dshHome = if ($env:DSH_HOME) { $env:DSH_HOME } else { Join-Path $env:USERPROFILE '.dsh' }
$cacheDir = Join-Path $dshHome 'storages\session_projcache\sessions'
if (-not (Test-Path $cacheDir)) {
    Write-Host "No session cache found at: $cacheDir"
    exit 1
}

function Get-Num {
    param([string]$Name, [string]$Text)
    $m = [regex]::Match($Text, '"' + $Name + '"\s*:\s*([0-9.eE+\-]+)')
    if ($m.Success) {
        return [double]$m.Groups[1].Value
    }
    return 0
}

$rows = @()
foreach ($file in Get-ChildItem $cacheDir -File) {
    $text = [System.IO.File]::ReadAllText($file.FullName, [System.Text.Encoding]::UTF8)

    $cwd = [regex]::Match($text, '"cwd"\s*:\s*"([^"]*)"').Groups[1].Value -replace '\\\\', '\'
    if ($cwd -notlike "*$Workspace*") {
        continue
    }

    $title = [regex]::Match($text, '(?s)"title"\s*:\s*\{.*?"val"\s*:\s*"([^"]*)"').Groups[1].Value

    $costAt = $text.IndexOf('"costUsage"')
    $totals = [regex]::Match($text.Substring($costAt, [Math]::Min(6000, $text.Length - $costAt)), '(?s)"totals"\s*:\s*\{(.*?)\}').Groups[1].Value

    $statsAt = $text.IndexOf('"sessionStats"')
    $stats = $text.Substring($statsAt, [Math]::Min(1500, $text.Length - $statsAt))

    $timelineAt = $text.IndexOf('"contextTimeline"')
    $times = [regex]::Matches($text.Substring($timelineAt, [Math]::Min(200000, $text.Length - $timelineAt)), '"time":\s*(\d{13})') |
             ForEach-Object { [double]$_.Groups[1].Value }

    $window = '-'
    if ($times.Count -gt 0) {
        $first = [datetimeoffset]::FromUnixTimeMilliseconds([long]($times | Measure-Object -Minimum).Minimum).ToLocalTime().ToString('MM-dd HH:mm')
        $last = [datetimeoffset]::FromUnixTimeMilliseconds([long]($times | Measure-Object -Maximum).Maximum).ToLocalTime().ToString('MM-dd HH:mm')
        $window = "$first -> $last"
    }

    $rows += [pscustomobject]@{
        Id        = ($file.BaseName -replace '^session-', '').Substring(0, 8)
        Kind      = $(if ($file.BaseName -like 'session-*') { 'main' } else { 'agent' })
        Window    = $window
        Turns     = [int](Get-Num 'turns' $stats)
        Steps     = [int](Get-Num 'steps' $stats)
        Input     = [int](Get-Num 'input' $totals)
        Output    = [int](Get-Num 'output' $totals)
        CacheRead = [int](Get-Num 'cacheRead' $totals)
        Reasoning = [int](Get-Num 'reasoning' $totals)
        Cost      = [math]::Round((Get-Num 'cost' $totals), 5)
        Title     = $title
    }
}

if ($rows.Count -eq 0) {
    Write-Host "No sessions recorded for workspace: $Workspace"
    exit 0
}

Write-Host "=== Sessions for $Workspace (sorted by cost) ==="
$rows | Sort-Object Cost -Descending |
    Format-Table Id, Kind, Window, Turns, Steps, Input, Output, CacheRead, Reasoning, Cost, Title -AutoSize

$sumInput = [int](($rows | Measure-Object Input -Sum).Sum)
$sumOutput = [int](($rows | Measure-Object Output -Sum).Sum)
$sumCache = [int](($rows | Measure-Object CacheRead -Sum).Sum)
$sumReason = [int](($rows | Measure-Object Reasoning -Sum).Sum)
$sumCost = [math]::Round((($rows | Measure-Object Cost -Sum).Sum), 4)
$hitRate = $sumCache / ($sumCache + $sumInput)

Write-Host "=== Totals ==="
Write-Host ("sessions       : {0}" -f $rows.Count)
Write-Host ("prompt tokens  : {0}  (uncached {1} + cacheRead {2})" -f ($sumInput + $sumCache), $sumInput, $sumCache)
Write-Host ("output tokens  : {0}  (reasoning {1})" -f $sumOutput, $sumReason)
Write-Host ("cache hit rate : {0:P1}" -f $hitRate)
Write-Host ("cost estimate  : `${0}  (agents `${1} / main `${2})" -f `
    $sumCost, `
    [math]::Round((($rows | Where-Object Kind -eq 'agent' | Measure-Object Cost -Sum).Sum), 4), `
    [math]::Round((($rows | Where-Object Kind -eq 'main' | Measure-Object Cost -Sum).Sum), 4))

$ledgerPath = Join-Path $dshHome '.dshw-usage.json'
if (Test-Path $ledgerPath) {
    $ledger = [System.IO.File]::ReadAllText($ledgerPath, [System.Text.Encoding]::UTF8) | ConvertFrom-Json
    Write-Host "=== Balance panel (whole account) ==="
    Write-Host ("date {0}  today {1} {2}  balance {3} {2}" -f $ledger.date, $ledger.todayUsage, $ledger.lastCurrency, $ledger.lastBalance)
}
