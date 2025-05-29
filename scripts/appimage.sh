#!/usr/bin/env bash
set -euo pipefail

TARGET="eidolon"

version_file="engine/include/version.h"
VERSION_MAJOR="$(sed -n 's/#define CASTOR_VERSION_MAJOR \([0-9]*\)/\1/p' "$version_file")"
VERSION_MINOR="$(sed -n 's/#define CASTOR_VERSION_MINOR \([0-9]*\)/\1/p' "$version_file")"
VERSION_PATCH="$(sed -n 's/#define CASTOR_VERSION_PATCH \([0-9]*\)/\1/p' "$version_file")"
VERSION="${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"

APPDIR="AppDir"

echo "Creating AppImage for version $VERSION..."

# Ensure binary exists
if [[ ! -f "demo/${TARGET}" ]]; then
  echo "Error: demo/${TARGET} binary not found. Please build the game first."
  exit 1
fi

# Prepare AppDir
rm -rf "$APPDIR"
mkdir -p "$APPDIR/usr/bin"
mkdir -p "$APPDIR/usr/lib"

# Copy necessary files
cp "demo/${TARGET}" "$APPDIR/usr/bin/"
cp -r assets levels "$APPDIR/"
cp -r deps/lib "$APPDIR/usr/"
bash ./scripts/fetch_deps.sh $APPDIR
sudo cp /usr/lib/libSDL3.so* "$APPDIR/usr/lib"

# Create AppRun
cat > "$APPDIR/AppRun" <<EOF
#!/bin/bash
HERE=\$(dirname "\$(readlink -f "\$0")")
cd \$HERE
export LD_LIBRARY_PATH="\$HERE/usr/lib:\$LD_LIBRARY_PATH"
export SDL_AUDIODRIVER=alsa
exec "\$HERE/usr/bin/${TARGET}" "\$@"
EOF
chmod +x "$APPDIR/AppRun"

# Create .desktop file
cat > "$APPDIR/${TARGET}.desktop" <<EOF
[Desktop Entry]
Name=Eidolon
Exec=${TARGET}
Icon=${TARGET}
Type=Application
Categories=Game;
EOF

ICON_PATH="${APPDIR}/${TARGET}.png"
# Try to copy the icon first
if ! cp "$TARGET.png" "$ICON_PATH" 2>/dev/null; then
  # Fallback logic if copy fails
  if command -v magick &> /dev/null; then
    magick -size 256x256 xc:purple "$ICON_PATH"
  else
    echo "Warning: 'magick' not found. Creating empty icon."
    touch "$ICON_PATH"
  fi
fi

# Build AppImage
NO_STRIP=true ./tools/linuxdeploy-x86_64.AppImage \
    --appdir AppDir \
    --executable AppDir/usr/bin/eidolon \
    --icon-file AppDir/eidolon.png \
    --desktop-file AppDir/eidolon.desktop \
    --output appimage
