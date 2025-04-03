# Detect OS
OS := $(shell uname -s 2>/dev/null || echo Windows)
EXT :=
INSTALL_DIR := /usr/bin/
RM := rm -rf
CP := cp
MKDIR := mkdir -p

ifeq ($(OS), Windows)
    EXT := .exe
    INSTALL_DIR := bin\
    RM := del /Q /F
    CP := copy
    MKDIR := mkdir
    LDFLAGS += -lmingw32 -mwindows
endif

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinclude -Ilib/raylib/include
LDFLAGS += -Llib/raylib/lib -l:libraylib.a -lm

ifeq ($(OS), Linux)
    LDFLAGS += -lGL -lpthread -ldl -lrt -lX11
endif

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DIST_DIR = dist

# Target and version info
TARGET = launch$(EXT)
version_file = include/version.h
VERSION_MAJOR = $(shell sed -n -e 's/\#define VERSION_MAJOR \([0-9]*\)/\1/p' $(version_file))
VERSION_MINOR = $(shell sed -n -e 's/\#define VERSION_MINOR \([0-9]*\)/\1/p' $(version_file))
VERSION_PATCH = $(shell sed -n -e 's/\#define VERSION_PATCH \([0-9]*\)/\1/p' $(version_file))
VERSION = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

# Determine the build type
ifeq ($(type), RELEASE)
	CFLAGS += -O3
else
	SANITIZERS = # -fsanitize=address,undefined
	CFLAGS  += -DDEBUG -ggdb
	CFLAGS  += $(SANITIZERS)
	LDFLAGS += $(SANITIZERS)
endif

# Source and object files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

.DEFAULT_GOAL := help

# Targets
.PHONY: all
all: check_tools $(BUILD_DIR) $(TARGET) ## Build the project
	@echo "Build complete."

.PHONY: check_tools
check_tools:
	@command -v gcc >/dev/null 2>&1 || { echo "[ERRO] gcc is not installed."; exit 1; }
	@command -v bear >/dev/null 2>&1 || { echo "[WARN] bear is not installed. Skipping compile_commands.json target."; }

$(BUILD_DIR):
	@echo "[INFO] Creating build directory"
	$(MKDIR) $(BUILD_DIR)

$(TARGET): $(OBJ_FILES)
	@echo "[INFO] Building the project"
	@$(CC) -o $@ $^ $(LDFLAGS)
	@echo "[INFO] Executable created: $(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: install
install: all
	@echo "[INFO] Installing $(TARGET) to $(INSTALL_DIR)"
	$(CP) $(TARGET) $(INSTALL_DIR)

.PHONY: uninstall
uninstall:
	@echo "[INFO] Uninstalling $(TARGET)"
	$(RM) $(INSTALL_DIR)$(TARGET)

.PHONY: clean
clean:
	@echo "[INFO] Cleaning up build directory and executable."
	$(RM) $(BUILD_DIR) $(TARGET)

.PHONY: distclean
distclean: clean
	@echo "[INFO] Performing full clean."
	$(RM) *~ core $(SRC_DIR)/*~ $(DIST_DIR)/*.tar.gz

.PHONY: dist
dist: $(SRC_FILES)
	@echo "[INFO] Creating a tarball for version $(VERSION)"
	$(MKDIR) $(DIST_DIR)
	tar -czvf $(DIST_DIR)/$(TARGET)-$(VERSION).tar.gz $(SRC_DIR) $(INCLUDE_DIR) Makefile README.md

.PHONY: compile_commands.json
compile_commands.json: $(SRC_FILES)
	@echo "[INFO] Generating compile_commands.json"
	bear -- make all

.PHONY: help
help:
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

.PHONY: verbose
verbose: CFLAGS += -DVERBOSE
verbose: all
