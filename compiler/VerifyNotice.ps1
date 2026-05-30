param(
    [Parameter(Mandatory = $true)]
    [string]$ProjectName
)

$Extensions = @(
    ".cpp", ".c", ".h", ".x"
)

$Errors = @()

Get-ChildItem -Path . -Recurse -File |
    Where-Object { $Extensions -contains $_.Extension.ToLowerInvariant() } |
    ForEach-Object {

        $File = $_

        try {
            $Lines = Get-Content -LiteralPath $File.FullName

            if ($Lines.Count -eq 0) {
                $Errors += [PSCustomObject]@{
                    File   = $File.FullName
                    Issue  = "Empty file"
                    Detail = ""
                }
                return
            }

            # Search entire file for a copyright notice line
	    $NoticeLine = $Lines |
	    Where-Object { $_ -match '^//\s+(.+?)\s+(.+)$' } |
    Select-Object -First 1

if (-not $NoticeLine) {
    $Errors += [PSCustomObject]@{
        File   = $File.FullName
        Issue  = "Missing notice"
        Detail = ""
    }
    return
}

$null = $NoticeLine -match '^//\s+(.+?)\s+(.+)$'

$RecordedProject = $Matches[1]
$RecordedFile    = $Matches[2]

            if ($RecordedProject -ne $ProjectName) {
                $Errors += [PSCustomObject]@{
                    File   = $File.FullName
                    Issue  = "Project name mismatch"
                    Detail = "Expected '$ProjectName', found '$RecordedProject'"
                }
            }

            if ($RecordedFile -ne $File.Name) {
                $Errors += [PSCustomObject]@{
                    File   = $File.FullName
                    Issue  = "Filename mismatch"
                    Detail = "Expected '$($File.Name)', found '$RecordedFile'"
                }
            }
        }
        catch {
            $Errors += [PSCustomObject]@{
                File   = $File.FullName
                Issue  = "Read error"
                Detail = $_.Exception.Message
            }
        }
    }

if ($Errors.Count -eq 0) {
    Write-Host "SUCCESS: All files passed verification." -ForegroundColor Green
}
else {
    Write-Host ""
    Write-Host "Found $($Errors.Count) issue(s):" -ForegroundColor Red
    Write-Host ""

    $Errors | Format-Table -AutoSize

    exit 1
}