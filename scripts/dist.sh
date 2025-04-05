#!/usr/bin/env bash

DIST_DIR=dist
TARGET=eidolon

version_file=engine/include/version.h
VERSION_MAJOR="$(sed -n -e 's/\#define CASTOR_VERSION_MAJOR \([0-9]*\)/\1/p' "$version_file")"
VERSION_MINOR="$(sed -n -e 's/\#define CASTOR_VERSION_MINOR \([0-9]*\)/\1/p' "$version_file")"
VERSION_PATCH="$(sed -n -e 's/\#define CASTOR_VERSION_PATCH \([0-9]*\)/\1/p' "$version_file")"
VERSION="$VERSION_MAJOR.$VERSION_MINOR.$VERSION_PATCH"

PACKAGE_NAME="$TARGET-$VERSION"
PACKAGE_DIR="$DIST_DIR/$PACKAGE_NAME"

echo "Version: $VERSION"

rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR"

cp -r engine/README.md demo/"$TARGET" builder/lvl launcher/launch CHANGELOG.md deps assets levels "$PACKAGE_DIR"

tar -czvf "$DIST_DIR/$PACKAGE_NAME.tar.gz" -C "$DIST_DIR" "$PACKAGE_NAME"

rm -rf "$PACKAGE_DIR"

echo "Archive created: $DIST_DIR/$PACKAGE_NAME.tar.gz"
