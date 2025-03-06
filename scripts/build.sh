#!/usr/bin/env bash

cd builder || exit 1
make all -B
cd ..
