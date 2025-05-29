#!/bin/bash

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <path-to-.so-file-or-directory>"
    exit 1
fi

TARGET="$1"

copy_deps() {
    local file="$1"
    local dir
    dir="$(dirname "$file")"

    echo "Processing $file"

    ldd "$file" | awk '/=>/ { print $3 }' | while read -r dep; do
        if [ -f "$dep" ]; then
            echo "Copying $dep to $dir"
            cp -u "$dep" "$dir/"
        else
            echo "Skipping missing dependency: $dep"
        fi
    done
}

if [ -f "$TARGET" ]; then
    # Single .so file
    copy_deps "$TARGET"
elif [ -d "$TARGET" ]; then
    # Directory: find .so files
    find "$TARGET" -type f -name "*.so" | while read -r sofile; do
        copy_deps "$sofile"
    done
else
    echo "Error: '$TARGET' is not a valid file or directory."
    exit 1
fi

