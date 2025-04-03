OS := $(shell uname -s)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinclude -Ideps/include -I../include
LDFLAGS = -Ldeps/lib -L.. -l:libcastor.a -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm

ifeq ($(OS), Windows_NT)
    EXE = .exe
    RM = del /Q /F
    MKDIR = mkdir
    COPY = copy
    TARGET_DIR = bin
    LDFLAGS += -lmingw32 -mwindows
else
    EXE =
    RM = rm -rf
    MKDIR = mkdir -p
    COPY = cp
    TARGET_DIR = /usr/bin
endif

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DIST_DIR = dist

# Target and version info
TARGET = eidolon$(EXE)
version_file = include/version.h
VERSION_MAJOR = $(shell sed -n -e 's/\#define VERSION_MAJOR \([0-9]*\)/\1/p' $(version_file))
VERSION_MINOR = $(shell sed -n -e 's/\#define VERSION_MINOR \([0-9]*\)/\1/p' $(version_file))
VERSION_PATCH = $(shell sed -n -e 's/\#define VERSION_PATCH \([0-9]*\)/\1/p' $(version_file))
VERSION = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

# Build type
ifeq ($(type), RELEASE)
    CFLAGS += -O3
else
    SANITIZERS = # -fsanitize=address,undefined
    CFLAGS  += -DDEBUG -ggdb
    CFLAGS  += $(SANITIZERS)
    LDFLAGS += $(SANITIZERS)
endif

# Source and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Default target
.DEFAULT_GOAL := all

.PHONY: all
all: $(BUILD_DIR) $(TARGET)
	@echo "Build complete."

$(BUILD_DIR):
	@echo "[INFO] Creating build directory"
	$(MKDIR) $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ_FILES)
	@echo "[INFO] Building $(TARGET)"
	@$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: install
install: all
ifeq ($(OS), Windows_NT)
	@echo "[INFO] Installing $(TARGET) to $(TARGET_DIR)"
	$(MKDIR) $(TARGET_DIR)
	$(COPY) $(TARGET) $(TARGET_DIR)/$(TARGET)
else
	@echo "[INFO] Installing $(TARGET) to /usr/bin/"
	$(COPY) $(TARGET) /usr/bin/$(TARGET)
endif

.PHONY: uninstall
uninstall:
ifeq ($(OS), Windows_NT)
	@echo "[INFO] Uninstalling $(TARGET)"
	$(RM) $(TARGET_DIR)/$(TARGET)
else
	@echo "[INFO] Uninstalling $(TARGET)"
	$(RM) /usr/bin/$(TARGET)
endif

.PHONY: clean
clean:
	@echo "[INFO] Cleaning up build directory and executable."
	$(RM) $(BUILD_DIR) $(TARGET)

.PHONY: help
help:
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
