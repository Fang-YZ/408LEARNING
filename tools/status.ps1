# ---------------------------------------------------------------------------
# status.ps1 -- learn408 学习状态总览（教练/学员共用入口）
#
# 用法（在 E:\learn408 下）：
#   powershell -ExecutionPolicy Bypass -File .\tools\status.ps1
#
# 它做四件事：
#   1. 对比「课次」与「已就绪材料」，报出每课材料齐不齐（笔记/示例/参考答案）
#   2. 扫描 homework\N01..N05\ 收件箱，报出已交/未交
#   3. 打印「现在该做什么」（当前阻塞点 + 一条命令验证）
#   4. 打印档案里的每日节奏
#
# 只读脚本：不编译、不启动服务、不改任何文件。输出用英文标签 + 中文说明。
# ---------------------------------------------------------------------------
$ErrorActionPreference = 'Continue'

$ROOT = 'E:\learn408'

function Line { param([string]$c = '-', [int]$n = 72) Write-Host ($c * $n) -ForegroundColor DarkGray }
function Head { param([string]$t) Write-Host ''; Line '='; Write-Host "  $t" -ForegroundColor White; Line '=' }

Head "learn408 学习状态总览   $(Get-Date -Format 'yyyy-MM-dd HH:mm')"

# --- 1. 档案摘录 -----------------------------------------------------------
Write-Host ''
Write-Host '  学员档案' -ForegroundColor Cyan
$catalog = Join-Path $ROOT '课程总目录.md'
if (Test-Path $catalog) {
    $c = Get-Content $catalog -Raw -Encoding UTF8
    if ($c -match '每日可投入[：:]\s*([0-9]+[^（(\r\n]{0,12}h)') { Write-Host ("    每日可投入: " + $Matches[1].Trim()) }
    if ($c -match '建档日期：([0-9\-]+)') { Write-Host ("    建档日期  : " + $Matches[1]) }
}
Write-Host ("    工作目录  : $ROOT")

# --- 2. B 线课次 vs 材料就绪度 --------------------------------------------
Head 'B 线（计算机网络）课次 · 材料就绪度'

$lessons = @(
    @{ Id = 'N1'; Dir = 'N01_intro_metrics';  Title = '概述(上)+性能指标';       Grade = 'grade_net_delay.ps1' }
    @{ Id = 'N2'; Dir = 'N02_layered_arch';   Title = '概述(下)+分层/OSI';       Grade = 'check-n2.ps1' }
    @{ Id = 'N3'; Dir = 'N03_physical_layer'; Title = '物理层(1)+通信基础';      Grade = 'check-n3.ps1' }
    @{ Id = 'N4'; Dir = 'N04_transmission_media'; Title = '物理层(2)+介质/设备'; Grade = 'check-n4.ps1' }
    @{ Id = 'N5'; Dir = 'N05_data_link_basics';   Title = '数据链路层(1)';       Grade = 'check-n5.ps1' }
)

foreach ($l in $lessons) {
    $dir = Join-Path $ROOT ("networks\" + $l.Dir)
    if (-not (Test-Path $dir)) {
        Write-Host ("    {0,-3} {1,-22}  未建档" -f $l.Id, $l.Title) -ForegroundColor DarkGray
        continue
    }
    $note  = Test-Path (Join-Path $dir 'notes.md')
    $srcc  = @(Get-ChildItem $dir -Recurse -File -Filter *.c -ErrorAction SilentlyContinue).Count
    $grade = if ($l.Grade) {
                 # N1 的批改器在课次目录里，N2–N4 的在 tools\ 下
                 (Test-Path (Join-Path $dir $l.Grade)) -or (Test-Path (Join-Path $ROOT ('tools\' + $l.Grade)))
             } else { $false }
    $tag   = if ($note -and $srcc -gt 0) { '材料就绪' } elseif ($note) { '仅笔记' } else { '不完整' }
    $color = if ($note -and $srcc -gt 0) { 'Green' } else { 'Yellow' }
    $extra = if ($l.Id -eq 'N1') { if ($grade) { '批改脚本OK' } else { '缺批改脚本' } }
             elseif ($grade)   { '对拍器OK' }
             else              { '缺对拍器' }
    Write-Host ("    {0,-3} {1,-22}  笔记:{2}  示例:{3} 个  {4}  {5}" -f `
        $l.Id, $l.Title, $(if ($note) { '有' } else { '无' }), $srcc, $tag, $extra) -ForegroundColor $color
}

# --- 3. 收件箱扫描 ---------------------------------------------------------
Head '作业收件箱（homework\）'

$found = $false
foreach ($n in @('N01', 'N02', 'N03', 'N04', 'N05')) {
    $hw = Join-Path $ROOT ("homework\" + $n)
    $files = @()
    if (Test-Path $hw) {
        $files = @(Get-ChildItem $hw -Recurse -File -ErrorAction SilentlyContinue |
                   Where-Object { $_.Name -notmatch '^README\.md$' -and $_.Extension -ne '.exe' })
    }
    if ($files.Count -gt 0) {
        $found = $true
        Write-Host ("    $n  " + ($files.Count) + " 个提交件: " + (($files | ForEach-Object { $_.Name }) -join ', ')) -ForegroundColor Green
    } else {
        Write-Host ("    $n  未提交") -ForegroundColor DarkGray
    }
}
if (-not $found) { Write-Host '    （B 线目前没有任何提交件）' -ForegroundColor Yellow }

# --- 4. 现在该做什么 -------------------------------------------------------
Head '现在该做什么'

if (-not (Test-Path (Join-Path $ROOT 'homework\N01'))) {
    Write-Host '    阻塞点：B 线 N1 的 H1–H4 尚未提交 → N2/N3 不能解锁' -ForegroundColor Yellow
    Write-Host ''
    Write-Host '    第 1 步  打开笔记做 H1–H4：' -ForegroundColor Cyan
    Write-Host '             networks\N01_intro_metrics\notes.md' -ForegroundColor White
    Write-Host '    第 2 步  H4 的程序存到这里：' -ForegroundColor Cyan
    Write-Host '             homework\N01\net_delay.c' -ForegroundColor White
    Write-Host '    第 3 步  存盘后跑批改（会打印 PASS/FAIL 明细）：' -ForegroundColor Cyan
    Write-Host '             powershell -ExecutionPolicy Bypass -File .\networks\N01_intro_metrics\grade_net_delay.ps1' -ForegroundColor White
    Write-Host '    第 4 步  把批改输出贴给我，我逐题批改并登记错题' -ForegroundColor Cyan
} else {
    Write-Host '    homework\N01 已有文件 → 我该批改了。' -ForegroundColor Green
    Write-Host '    批改命令：' -ForegroundColor Cyan
    Write-Host '      powershell -ExecutionPolicy Bypass -File .\networks\N01_intro_metrics\grade_net_delay.ps1' -ForegroundColor White
}

Write-Host ''
Write-Host '    其余待交（不阻塞 B 线，但别拖）：' -ForegroundColor DarkGray
Write-Host '      A 线 L5 递归：lessons\L05_recursion\notes.md 的 HW1–HW4' -ForegroundColor DarkGray

# --- 4b. 每日节奏（按 4–5h 制定） -----------------------------------------
Head '每日节奏建议（4–5h）'
Write-Host '    听讲 + 跑示例        60 min      读概念 → 亲手编译运行 → 看现象' -ForegroundColor White
Write-Host '    做作业（手算 + 写码）120–180 min 先手算/手推，再写程序' -ForegroundColor White
Write-Host '    已知答案自测         30 min      跑批改脚本，对不上就定位到具体那一步' -ForegroundColor White
Write-Host '    复盘 + 勾自查清单    30 min      笔记末尾清单逐条打勾，没勾上的回头补' -ForegroundColor White
Write-Host ''
Write-Host '    一条铁律：先手算 → 再跑程序 → 对不上就找哪一步错（别"跑出什么就信什么"）' -ForegroundColor Yellow

# --- 5. 一键自检入口（自动发现，避免计数过期） ----------------------------
Head '一键自检入口（教练侧已自测通过）'
Write-Host '    N1 数值题    networks\N01_intro_metrics\grade_net_delay.ps1' -ForegroundColor White
Get-ChildItem (Join-Path $ROOT 'tools') -Filter 'check-n*.ps1' -File -ErrorAction SilentlyContinue |
    Sort-Object Name | ForEach-Object {
        # 取该脚本对应的 lessons 目录名，便于学员知道它管哪一课
        $tag = switch -Regex ($_.Name) {
            'check-n2' { 'N2 分层/封装  ' }
            'check-n3' { 'N3 信道/交换  ' }
            'check-n4' { 'N4 介质/设备  ' }
            'check-n5' { 'N5 组帧/CRC   ' }
            default    { '              ' }
        }
        Write-Host ("    $tag tools\" + $_.Name) -ForegroundColor White
    }
Write-Host '    A 线通用对拍 tools\check.ps1 -Exe <程序> -Tests <数据文件>' -ForegroundColor White
Write-Host '    （各脚本会打印自己的 PASS/FAIL 计数，避免手抄计数过期）' -ForegroundColor DarkGray

Write-Host ''
Line '-'
Write-Host '  本脚本只读，不修改任何文件。' -ForegroundColor DarkGray
Write-Host ''
