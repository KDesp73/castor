#!/usr/bin/env bash

# Default Makefile
MAKEFILE="Makefile"

# Define dependencies manually (or leave empty for auto-detection)
DEPENDENCIES=()

# Parse command-line options
while getopts "m:" opt; do
    case $opt in
        m) MAKEFILE="$OPTARG" ;;
        *) echo "Usage: $0 [-m <Makefile>]" && exit 1 ;;
    esac
done

# Function to check if an executable exists
check_executable() {
    if command -v "$1" &>/dev/null; then
        echo "✔ Executable '$1' found"
    else
        echo "✖ Executable '$1' NOT found"
    fi
}

# Function to search for a shared library in $LD_LIBRARY_PATH
check_shared_library() {
    local lib="$1"
    IFS=":" read -ra paths <<< "$LD_LIBRARY_PATH"

    for path in "${paths[@]}"; do
        if [[ -f "$path/$lib" ]]; then
            echo "✔ Shared library '$lib' found in $path"
            return
        fi
    done

    # Fallback: Check system-wide with ldconfig
    if ldconfig -p | grep -q "$lib"; then
        echo "✔ Shared library '$lib' found (system-wide)"
    else
        echo "✖ Shared library '$lib' NOT found"
    fi
}

# Function to check for a static library
check_static_library() {
    local lib="$1"
    IFS=":" read -ra paths <<< "$LD_LIBRARY_PATH"

    for path in "${paths[@]}"; do
        if [[ -f "$path/$lib" ]]; then
            echo "✔ Static library '$lib' found in $path"
            return
        fi
    done

    echo "✖ Static library '$lib' NOT found"
}

# Function to detect dependencies from a given Makefile
detect_makefile_deps() {
    if [[ -f "$MAKEFILE" ]]; then
        echo "Detecting dependencies from $MAKEFILE..."
        grep -oP "(?<=-l)[a-zA-Z0-9_]+" "$MAKEFILE" | while read -r lib; do
            check_shared_library "lib$lib.so"
        done
    else
        echo "✖ Makefile '$MAKEFILE' not found"
        exit 1
    fi
}

# Main check function
check_dependencies() {
    for dep in "${DEPENDENCIES[@]}"; do
        if [[ $dep == *.so ]]; then
            check_shared_library "$dep"
        elif [[ $dep == *.a ]]; then
            check_static_library "$dep"
        else
            check_executable "$dep"
        fi
    done
}

# Run checks
check_dependencies

# Try auto-detecting dependencies if none are manually defined
if [[ ${#DEPENDENCIES[@]} -eq 0 ]]; then
    detect_makefile_deps
fi
