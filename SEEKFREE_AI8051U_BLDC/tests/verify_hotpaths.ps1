$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $PSScriptRoot
$outputRoot = Join-Path $projectRoot "project/mdk/out_file"
$buildLog = Get-Content -Raw -LiteralPath (Join-Path $outputRoot "SEEKFREE.build_log.htm")
$motorListing = Get-Content -Raw -LiteralPath (Join-Path $outputRoot "motor_control.lst")
$mduListing = Get-Content -Raw -LiteralPath (Join-Path $outputRoot "mdu32_div.lst")
$inputSource = Get-Content -Raw -LiteralPath (Join-Path $projectRoot "bldc/signal_input.c")

if ($buildLog -notmatch '0 Error\(s\), 0 Warning\(s\)') {
    throw "全量构建不是 0 Error(s), 0 Warning(s)"
}

$comparatorMatch = [regex]::Match(
    $motorListing,
    'FUNCTION comparator_isr \(BEGIN\)(?<body>[\s\S]*?)FUNCTION comparator_isr \(END\)'
)
if (-not $comparatorMatch.Success) {
    throw "未在 LST 中找到 comparator_isr 汇编区间"
}

$forbiddenArithmetic = '\?C\?(LIMUL|ULIDIV|ULDIV|SLDIV|SIDIV)'
if ($comparatorMatch.Groups['body'].Value -match $forbiddenArithmetic) {
    throw "comparator_isr 仍调用通用软件乘除法"
}
if ($comparatorMatch.Groups['body'].Value -match 'bldc_mdu32_div_u32') {
    throw "comparator_isr 不得直接访问 MDU32"
}

$timer0Match = [regex]::Match(
    $motorListing,
    'FUNCTION TM0_Isr \(BEGIN\)(?<body>[\s\S]*?)FUNCTION TM0_Isr \(END\)'
)
if (-not $timer0Match.Success) {
    throw "未在 LST 中找到 TM0_Isr 汇编区间"
}
if ($timer0Match.Groups['body'].Value -match $forbiddenArithmetic) {
    throw "TM0_Isr 仍调用通用软件乘除法"
}

$mduCalls = [regex]::Matches($timer0Match.Groups['body'].Value, 'LCALL\s+bldc_mdu32_div_u32').Count
if ($mduCalls -ne 2) {
    throw "TM0_Isr 应只有缓存初始化和正常换相两个互斥 MDU32 调用点，实际为 $mduCalls"
}

if ($mduListing -notmatch 'MOV\s+DMAIR,#04H') {
    throw "MDU32 汇编封装缺少无符号 32 位除法指令"
}
if ($inputSource -match 'PWMB_SR1\s*=') {
    throw "PWMB ISR 不得通过写 PWMB_SR1 清除捕获标志"
}
if ($inputSource -match 'PWMB_IER\s*=\s*0x07') {
    throw "PWMB 不得重新启用 UIF 中断"
}

Write-Output "PASS: build, comparator ISR, Timer0 ISR, MDU32 wrapper, PWMB flag handling"
