#!/usr/bin/env bash

build() {
    cd "$1" || exit 1
    make clean
    make compile_commands.json
    make all
    cd .. || exit 1
    mv "$1/$2" .
}

case "$1" in
    builder) build "builder" "lvl" ;;
    demo) build "demo" "game" ;;
    launcher) build "launcher" "launch" ;;
    *) 
        build "builder" "lvl"
        build "demo" "game"
        build "launcher" "launch"
        ;;
esac
