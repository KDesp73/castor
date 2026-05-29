#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
VERSION="5.5"
TAG="5.5"
BUILD_ROOT="${ROOT}/.build"
SRC_DIR="${BUILD_ROOT}/raylib-${VERSION}"
DESTDIR="${ROOT}/deps/lib"
RAYLIB_A="${DESTDIR}/libraylib.a"

if [[ -f "${RAYLIB_A}" ]]; then
    echo "[INFO] ${RAYLIB_A} already exists, skipping Raylib build."
    exit 0
fi

mkdir -p "${BUILD_ROOT}" "${DESTDIR}"

if [[ ! -d "${SRC_DIR}" ]]; then
    echo "[INFO] Cloning Raylib ${TAG}..."
    git clone --depth 1 --branch "${TAG}" https://github.com/raysan5/raylib.git "${SRC_DIR}"
fi

echo "[INFO] Building Raylib static library..."
make -C "${SRC_DIR}/src" PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC -j"$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)"

cp -f "${SRC_DIR}/src/libraylib.a" "${RAYLIB_A}"
ln -sf libraylib.a "${DESTDIR}/libraylib.a.5.5"

echo "[INFO] Installed ${RAYLIB_A}"
