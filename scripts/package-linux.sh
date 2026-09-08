#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${1:-$ROOT/build}"
DIST_ROOT="${2:-$ROOT/dist}"
VERSION="${3:-1.0.0}"

BIN="$(find "$BUILD_DIR" -type f -name 2DGameSFML -executable | head -n 1)"
if [[ -z "$BIN" ]]; then
    echo "Could not find 2DGameSFML binary under $BUILD_DIR" >&2
    exit 1
fi

STAGE="$DIST_ROOT/2DGameSFML-linux-x86_64"
rm -rf "$STAGE"
mkdir -p "$STAGE/lib"

cp "$BIN" "$STAGE/2DGameSFML"
chmod +x "$STAGE/2DGameSFML"
cp "$ROOT/packaging/README.txt" "$STAGE/README.txt"

ASSETS=(
    Around_The_World.ogg
    Hyderabad.ogg
    jeremy-soule-sunrise-of-flutes.ogg
    marshmello.ogg
    music2.ogg
    hit.ogg
    background.png
    ball11.png
    music_icon.png
    playbutton.png
    reset.png
    CyrilicOld.ttf
)
for asset in "${ASSETS[@]}"; do
    cp "$ROOT/Game1.0/$asset" "$STAGE/"
done

copy_dep() {
    local lib="$1"
    [[ -e "$lib" ]] || return 0
    local name
    name="$(basename "$lib")"
    cp -aL "$lib" "$STAGE/lib/$name"
}

while read -r lib; do
    [[ -z "$lib" || "$lib" == *linux-vdso* || "$lib" == *ld-linux* ]] && continue
    name="$(basename "$lib")"
    case "$name" in
        libc.so*|libm.so*|libpthread.so*|libdl.so*|libresolv.so*|librt.so*|libgcc_s.so*|libstdc++.so*)
            continue
            ;;
        libX*.so*|libGL.so*|libGLX.so*|libGLdispatch.so*|libOpenGL.so*|libxcb*.so*|libbsd.so*|libmd.so*|libdrm.so*|libz.so*|libudev.so*|libgpg-error.so*|libffi.so*|libstdc++.so*)
            continue
            ;;
    esac
    copy_dep "$lib"
done < <(ldd "$STAGE/2DGameSFML" | awk '/=>/ {print $3}')

cat > "$STAGE/2DGameSFML.sh" << 'EOF'
#!/bin/sh
DIR=$(CDPATH= cd -- "$(dirname "$0")" && pwd)
export LD_LIBRARY_PATH="$DIR/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
exec "$DIR/2DGameSFML" "$@"
EOF
chmod +x "$STAGE/2DGameSFML.sh"

if command -v patchelf >/dev/null 2>&1; then
    patchelf --set-rpath '$ORIGIN/lib' "$STAGE/2DGameSFML"
fi

mkdir -p "$DIST_ROOT"
tar -C "$DIST_ROOT" -czf "$DIST_ROOT/2DGameSFML-linux-x86_64.tar.gz" "2DGameSFML-linux-x86_64"

if [[ "${MAKE_APPIMAGE:-1}" == "1" ]]; then
    APPDIR="$DIST_ROOT/AppDir"
    rm -rf "$APPDIR"
    mkdir -p "$APPDIR/usr/bin" "$APPDIR/usr/share/applications" "$APPDIR/usr/share/icons/hicolor/32x32/apps"
    cp "$STAGE/2DGameSFML" "$APPDIR/usr/bin/"
    cp "$STAGE/"*.png "$STAGE/"*.ogg "$STAGE/"*.ttf "$APPDIR/usr/bin/"
    cp "$ROOT/packaging/2dgame-sfml.desktop" "$APPDIR/usr/share/applications/"
    cp "$ROOT/Game1.0/ball11.png" "$APPDIR/usr/share/icons/hicolor/32x32/apps/2dgame-sfml.png"

    LINUXDEPLOY="${LINUXDEPLOY:-}"
    if [[ -z "$LINUXDEPLOY" ]]; then
        LINUXDEPLOY="$DIST_ROOT/linuxdeploy-x86_64.AppImage"
        if [[ ! -x "$LINUXDEPLOY" ]]; then
            curl -L --fail -o "$LINUXDEPLOY" \
                https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
            chmod +x "$LINUXDEPLOY"
        fi
    fi

    export APPIMAGE_EXTRACT_AND_RUN=1
    export LINUXDEPLOY_OUTPUT_VERSION="$VERSION"
    set +e
    (
        cd "$DIST_ROOT"
        "$LINUXDEPLOY" --appdir "$APPDIR" --output appimage
        APPIMAGE_FILE="$(ls -1 2D_Game_SFML*.AppImage 2DGameSFML*.AppImage 2dgame-sfml*.AppImage 2>/dev/null | head -n 1 || true)"
        if [[ -n "${APPIMAGE_FILE:-}" && "$APPIMAGE_FILE" != "2DGameSFML-linux-x86_64.AppImage" ]]; then
            mv "$APPIMAGE_FILE" "2DGameSFML-linux-x86_64.AppImage"
        fi
    )
    appimage_status=$?
    set -e
    if [[ "$appimage_status" -ne 0 || ! -f "$DIST_ROOT/2DGameSFML-linux-x86_64.AppImage" ]]; then
        echo "AppImage was not created; the .tar.gz package is still available." >&2
    fi
fi

echo "Linux packages are in $DIST_ROOT"
