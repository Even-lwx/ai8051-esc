$ErrorActionPreference = "Stop"

$maxFilterSum = 393210
$checkedPairs = 0L

for ($filterSum = 0; $filterSum -le $maxFilterSum; $filterSum++) {
    $quotient = [uint32][Math]::Floor($filterSum / 36.0)
    $remainder = $filterSum - ($quotient * 36)

    if (($remainder -lt 0) -or ($remainder -ge 36)) {
        throw "除以 36 的参考结果错误: filter_sum=$filterSum quotient=$quotient remainder=$remainder"
    }

    $lowerBoundary = [int][Math]::Floor($filterSum / 12.0)
    $upperBoundary = [int][Math]::Floor($filterSum / 4.0)
    $candidates = @(
        0,
        65535,
        ($lowerBoundary - 1),
        $lowerBoundary,
        ($lowerBoundary + 1),
        ($upperBoundary - 1),
        $upperBoundary,
        ($upperBoundary + 1)
    )

    foreach ($temp in $candidates) {
        if (($temp -lt 0) -or ($temp -gt 65535)) {
            continue
        }

        $oldLower = $temp -gt [Math]::Floor($filterSum / 12.0)
        $newLower = ($temp * 12) -gt $filterSum
        $oldUpper = $temp -lt [Math]::Floor($filterSum / 4.0)
        $newUpper = (($temp * 4) + 3) -lt $filterSum

        if (($oldLower -ne $newLower) -or ($oldUpper -ne $newUpper)) {
            throw "换相边界不等价: filter_sum=$filterSum temp=$temp"
        }

        $checkedPairs++
    }
}

Write-Output "PASS: filter_sum=0..$maxFilterSum, boundary pairs=$checkedPairs"
