#!/usr/bin/env bash

MAKEFILE="Makefile"
QUIET=false
SILENT=false
VERSION="1.1.0"
OS="$(uname -s)"

DEPENDENCIES=(
    "make"
    "pkg-config"
)

if [[ "$OS" == "Darwin" ]]; then
    DEPENDENCIES+=("clang")
else
    DEPENDENCIES+=("gcc")
fi

GREEN="\e[32m"
RED="\e[31m"
RESET="\e[0m"

show_help() {
    echo "Usage: $0 [-m <Makefile>] [-q] [-s] [-h] [-v]"
    exit 0
}

while getopts "m:qshv" opt; do
    case $opt in
        m) MAKEFILE="$OPTARG" ;;
        q) QUIET=true ;;
        s) SILENT=true ;;
        h) show_help ;;
        v) echo "Dependency Checker v$VERSION" && exit 0 ;;
        *) show_help ;;
    esac
done

print_msg() {
    if ! $QUIET && ! $SILENT; then
        echo -e "$1"
    fi
}

print_error() {
    if ! $QUIET; then
        echo -e "${RED}$1${RESET}" >&2
    fi
}

check_executable() {
    if command -v "$1" &>/dev/null; then
        print_msg "${GREEN}✔ Executable '$1' found${RESET}"
    else
        print_error "✖ Executable '$1' NOT found"
        exit 1
    fi
}

check_sdl_pkgconfig() {
    if pkg-config --exists sdl2 SDL2_image SDL2_ttf SDL2_mixer; then
        print_msg "${GREEN}✔ SDL2 libraries found via pkg-config${RESET}"
    else
        print_error "✖ SDL2 dev packages not found (install via apt or Homebrew)"
        exit 1
    fi
}

check_shared_library_linux() {
    local lib="$1"
    if ldconfig -p 2>/dev/null | grep -q "$lib"; then
        print_msg "${GREEN}✔ Shared library '$lib' found (system-wide)${RESET}"
        return
    fi
    print_error "✖ Shared library '$lib' NOT found"
    exit 1
}

for dep in "${DEPENDENCIES[@]}"; do
    check_executable "$dep"
done

check_sdl_pkgconfig

if [[ "$OS" != "Darwin" && -f "$MAKEFILE" ]]; then
    print_msg "Detecting shared libraries from $MAKEFILE (Linux only)..."
    grep -oP '(?<=-l)[a-zA-Z0-9_]+' "$MAKEFILE" 2>/dev/null | while read -r lib; do
        check_shared_library_linux "lib${lib}.so"
    done
fi

print_msg "${GREEN}All dependency checks passed.${RESET}"
