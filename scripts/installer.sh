#!/usr/bin/env bash
# Installs prebuilt libraries from deps/lib into the system library path (Linux).
# On macOS, prefer running binaries from the repo root or set DYLD_LIBRARY_PATH to deps/lib.

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEST="${INSTALL_DEST:-/usr/local/lib}"

if [[ "$(uname -s)" == "Darwin" ]]; then
    echo "macOS: system-wide install is not supported by this script."
    echo "Run games from the repository root, or export:"
    echo "  export DYLD_LIBRARY_PATH=\"${ROOT}/deps/lib:\${DYLD_LIBRARY_PATH:-}\""
    exit 0
fi

echo "Installing libraries from ${ROOT}/deps/lib to ${DEST}"
sudo mkdir -p "${DEST}"
sudo cp -f "${ROOT}"/deps/lib/* "${DEST}/"
