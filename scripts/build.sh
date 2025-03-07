#!/usr/bin/env bash


build() {
    cd "$1" || exit 1
    make clean
    make compile_commands.json
    make all
    cd .. || exit 1
}

build "builder"
build "demo"

mv ./builder/lvl .
mv ./demo/game .
