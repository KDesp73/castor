#!/bin/bash

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <path-to-binary-or-directory>"
    exit 1
fi

TARGET="$1"

copy_deps() {
    local file="$1"
    local dir
    dir="$(dirname "$file")"

    echo "Processing $file"

    if [[ "$(uname -s)" == "Darwin" ]]; then
        otool -L "$file" | awk '/^\t\// { print $1 }' | while read -r dep; do
            if [ -f "$dep" ]; then
                echo "Copying $dep to $dir"
                cp -u "$dep" "$dir/"
            else
                echo "Skipping missing dependency: $dep"
            fi
        done
    else
        ldd "$file" | awk '/=>/ { print $3 }' | while read -r dep; do
            if [ -f "$dep" ]; then
                echo "Copying $dep to $dir"
                cp -u "$dep" "$dir/"
            else
                echo "Skipping missing dependency: $dep"
            fi
        done
    fi
}

if [ -f "$TARGET" ]; then
    copy_deps "$TARGET"
elif [ -d "$TARGET" ]; then
    find "$TARGET" -type f \( -name "*.so" -o -name "*.dylib" -o -perm -111 \) | while read -r bin; do
        copy_deps "$bin"
    done
else
    echo "Error: '$TARGET' is not a valid file or directory."
    exit 1
fi
