#!/usr/bin/env bash

build_builder() {
    cd builder || exit 1
    make clean
    make compile_commands.json
    make all
    cd .. || exit 1
}

build_builder
mv ./builder/lvl .
