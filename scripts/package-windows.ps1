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

$dllSources = @()
$dllSources += Get-ChildItem -Path $exe.DirectoryName -Filter "sfml-*.dll" -ErrorAction SilentlyContinue
if ($SfmlBin -and (Test-Path $SfmlBin)) {
    $dllSources += Get-ChildItem -Path $SfmlBin -Filter "sfml-*.dll"
    $openal = Join-Path $SfmlBin "openal32.dll"
    if (Test-Path $openal) {
        Copy-Item $openal $stage
    }
}
foreach ($dll in ($dllSources | Sort-Object FullName -Unique)) {
    if ($dll.Name -match "-d-") {
        continue
    }
    Copy-Item $dll.FullName $stage
}

$vsEditions = Get-ChildItem "${env:ProgramFiles}\Microsoft Visual Studio\2022" -Directory -ErrorAction SilentlyContinue
foreach ($edition in $vsEditions) {
    $msvc = Join-Path $edition.FullName "VC\Redist\MSVC"
    if (-not (Test-Path $msvc)) {
        continue
    }
    $ver = Get-ChildItem $msvc -Directory | Sort-Object Name -Descending | Select-Object -First 1
    $crt = Get-ChildItem (Join-Path $ver.FullName "x64") -Directory -Filter "Microsoft.VC*.CRT" -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($crt) {
        Copy-Item (Join-Path $crt.FullName "*.dll") $stage
        break
    }
}

New-Item -ItemType Directory -Force -Path $DistDir | Out-Null
$zip = Join-Path $DistDir "2DGameSFML-windows-x64.zip"
if (Test-Path $zip) {
    Remove-Item -Force $zip
}
Compress-Archive -Path (Join-Path $stage "*") -DestinationPath $zip
Write-Host "Windows package: $zip"
