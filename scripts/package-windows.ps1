param(
    [string]$BuildDir = "build",
    [string]$DistDir = "dist",
    [string]$SfmlBin = ""
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot

$exe = Get-ChildItem -Path $BuildDir -Recurse -Filter "2DGameSFML.exe" | Select-Object -First 1
if (-not $exe) {
    throw "Could not find 2DGameSFML.exe under $BuildDir"
}

$stage = Join-Path $DistDir "2DGameSFML-windows-x64"
if (Test-Path $stage) {
    Remove-Item -Recurse -Force $stage
}
New-Item -ItemType Directory -Force -Path $stage | Out-Null

Copy-Item $exe.FullName (Join-Path $stage "2DGameSFML.exe")
Copy-Item (Join-Path $root "packaging\README.txt") (Join-Path $stage "README.txt")

$assets = @(
    "Around_The_World.ogg",
    "Hyderabad.ogg",
    "jeremy-soule-sunrise-of-flutes.ogg",
    "marshmello.ogg",
    "music2.ogg",
    "hit.ogg",
    "background.png",
    "ball11.png",
    "music_icon.png",
    "playbutton.png",
    "reset.png",
    "CyrilicOld.ttf"
)
foreach ($asset in $assets) {
    Copy-Item (Join-Path $root "Game1.0\$asset") $stage
}

$neededSfml = @(
    "sfml-graphics-2.dll",
    "sfml-window-2.dll",
    "sfml-system-2.dll",
    "sfml-audio-2.dll",
    "openal32.dll"
)
$searchDirs = @($exe.DirectoryName)
if ($SfmlBin -and (Test-Path $SfmlBin)) {
    $searchDirs += $SfmlBin
}
foreach ($name in $neededSfml) {
    $found = $null
    foreach ($dir in $searchDirs) {
        $candidate = Join-Path $dir $name
        if (Test-Path $candidate) {
            $found = $candidate
            break
        }
    }
    if (-not $found) {
        throw "Missing required DLL: $name"
    }
    Copy-Item $found $stage
}

$crtCopied = $false
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vswhere) {
    $crtDirs = & $vswhere -latest -products * -find "VC\Redist\MSVC\*\x64\Microsoft.VC*.CRT" 2>$null
    foreach ($crt in $crtDirs) {
        if ($crt -and (Test-Path $crt)) {
            Copy-Item (Join-Path $crt "*.dll") $stage
            Write-Host "Copied VC++ runtime from $crt"
            $crtCopied = $true
            break
        }
    }
}
if (-not $crtCopied) {
    $sys = Join-Path $env:SystemRoot "System32"
    $runtime = @(
        "vcruntime140.dll",
        "vcruntime140_1.dll",
        "msvcp140.dll",
        "msvcp140_1.dll",
        "msvcp140_2.dll"
    )
    foreach ($name in $runtime) {
        $src = Join-Path $sys $name
        if (Test-Path $src) {
            Copy-Item $src $stage
            $crtCopied = $true
        }
    }
    if ($crtCopied) {
        Write-Host "Copied VC++ runtime from $sys"
    }
}
if (-not $crtCopied) {
    throw "Could not locate the Visual C++ runtime DLLs"
}

New-Item -ItemType Directory -Force -Path $DistDir | Out-Null
$zip = Join-Path $DistDir "2DGameSFML-windows-x64.zip"
if (Test-Path $zip) {
    Remove-Item -Force $zip
}
Compress-Archive -Path (Join-Path $stage "*") -DestinationPath $zip
Write-Host "Windows package: $zip"
Get-ChildItem $stage | ForEach-Object { Write-Host ("  {0}" -f $_.Name) }
