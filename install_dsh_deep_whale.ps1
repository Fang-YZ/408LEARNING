# install_dsh_deep_whale.ps1
# Installs the dsh-deep-whale whale-girl skins into the DeepSeek Harness "web" profile.
# Usage (run in any PowerShell window):
#   powershell -ExecutionPolicy Bypass -File "E:\learn408\install_dsh_deep_whale.ps1"
$ErrorActionPreference = 'Stop'

$binDir = 'C:\Users\LENOVO\AppData\Local\npm-cache\_npx\1e7f6d9597241db0\node_modules\.bin'
$dsh = Join-Path $binDir 'dsh.cmd'

if (-not (Test-Path $dsh)) {
    Write-Host "[ERROR] dsh CLI not found at:" -ForegroundColor Red
    Write-Host "  $dsh" -ForegroundColor Red
    Write-Host "Locate your dsh.cmd next to the running harness, update this script, then retry." -ForegroundColor Red
    exit 1
}
Write-Host ("Using dsh: " + $dsh) -ForegroundColor Cyan

$specs = @(
    'github:Small-tailqwq/dsh-deep-whale#path:/skin-manager',
    'github:Small-tailqwq/dsh-deep-whale#path:/maid-atelier',
    'github:Small-tailqwq/dsh-deep-whale#path:/orca-link'
)

foreach ($spec in $specs) {
    Write-Host ""
    Write-Host (">>> dsh plugin --profile web add " + $spec) -ForegroundColor Yellow
    & $dsh plugin --profile web add $spec
    if ($LASTEXITCODE -ne 0) {
        Write-Host ("[ERROR] install failed for: " + $spec) -ForegroundColor Red
        Write-Host "Possible causes: no internet to github.com, or GitHub fetch blocked on this network." -ForegroundColor Red
        Write-Host "Copy the error above and send it back." -ForegroundColor Red
        exit $LASTEXITCODE
    }
}

Write-Host ""
Write-Host "=== Verify: installed plugins in profile web ===" -ForegroundColor Cyan
& $dsh plugin --profile web list

Write-Host ""
Write-Host "Next steps:" -ForegroundColor Green
Write-Host "  1. Fully quit and restart DeepSeek Harness once (first-time package install needs a restart)."
Write-Host "  2. Open Settings -> Skin Management, click Switch on 'maid-atelier' or 'orca-link'."
Write-Host "  3. Refresh the page. Done."
