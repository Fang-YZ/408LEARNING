<#
 * check.ps1 - batch tester for learn408 homework
 *
 * Usage (from E:\learn408):
 *   .\tools\check.ps1 -Exe .\hw1_leap.exe -Tests .\tools\tests\hw1_leap.txt
 *
 * If PowerShell blocks script execution, use:
 *   pwsh -ExecutionPolicy Bypass -File .\tools\check.ps1 -Exe .\hw1_leap.exe -Tests .\tools\tests\hw1_leap.txt
 *
 * Test file format (one case per line):
 *   INPUT => EXPECTED OUTPUT
 *   - a line starting with '#' is a comment
 *   - write \n inside INPUT to feed several input lines
 *   - write \n inside EXPECTED for multi-line expected output (e.g. triangle)
 *   - EXPECTED is matched as a substring of the program output,
 *     so printf prompts like "Enter a year: " are ignored
 #>
param(
    [Parameter(Mandatory = $true)][string]$Exe,
    [Parameter(Mandatory = $true)][string]$Tests
)

if (-not (Test-Path $Exe))
{
    Write-Host "ERROR: exe not found: $Exe" -ForegroundColor Red
    exit 2
}

if (-not (Test-Path $Tests))
{
    Write-Host "ERROR: test file not found: $Tests" -ForegroundColor Red
    exit 2
}

$exePath = (Resolve-Path $Exe).Path
$passCount = 0
$failCount = 0
$skipCount = 0

Get-Content $Tests | ForEach-Object {
    $line = $_.Trim()

    if ($line -eq '' -or $line.StartsWith('#'))
    {
        return
    }

    $parts = $line -split '\s*=>\s*', 2

    if ($parts.Count -lt 2)
    {
        Write-Host ("SKIP  (bad format) {0}" -f $line) -ForegroundColor Yellow
        $skipCount++
        return
    }

    $inputRaw = $parts[0].Trim()
    $inputLines = $inputRaw -split '\\n'
    $expected = ($parts[1].Trim() -replace '\\n', ' ' -replace '\s+', ' ')

    $actual = ($inputLines | & $exePath 2>&1 | Out-String)
    $actualNorm = ($actual -replace '\s+', ' ').Trim()

    if ($actualNorm.Contains($expected))
    {
        $passCount++
        Write-Host ("PASS  {0}" -f $inputRaw) -ForegroundColor Green
    }
    else
    {
        $failCount++
        Write-Host ("FAIL  {0}" -f $inputRaw) -ForegroundColor Red
        Write-Host ("      expected > {0}" -f $expected) -ForegroundColor DarkGray
        Write-Host ("      actual   > {0}" -f $actualNorm) -ForegroundColor DarkGray
    }
}

Write-Host ""
Write-Host ("Result: {0} passed, {1} failed, {2} skipped" -f $passCount, $failCount, $skipCount)

if ($failCount -gt 0)
{
    exit 1
}

exit 0
