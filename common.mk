ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(ROOT_DIR)platform.mk

DEPS_LIB := $(ROOT_DIR)deps/lib
RAYLIB_A := $(DEPS_LIB)/libraylib.a
