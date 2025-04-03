# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(OS),Windows_NT)
    PLATFORM = WINDOWS
else
    PLATFORM = UNIX
endif

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Iinclude -fPIC -Ideps/include
LDFLAGS = -Ldeps/lib -lSDL2 -lSDL2_ttf -lSDL2_image -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

LIBRARY_NAME = castor
ifeq ($(PLATFORM),WINDOWS)
    SO_NAME = lib$(LIBRARY_NAME).dll
else
    SO_NAME = lib$(LIBRARY_NAME).so
endif
A_NAME = lib$(LIBRARY_NAME).a

# Version info
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
ifeq ($(PLATFORM),WINDOWS)
    SRC_FILES := $(shell for /r $(SRC_DIR) %i in (*.c) do @echo %i)
else
    SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
endif
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Default target
.DEFAULT_GOAL := help

# Total source file count
TOTAL_FILES := $(words $(SRC_FILES))

# Counter to track progress
counter = 0

# Targets

.PHONY: all
all: $(BUILD_DIR) static shared ## Build the project
	./scripts/build.sh
	@echo "Build complete."

$(BUILD_DIR): ## Create the build directory if it doesn't exist
	@echo "[INFO] Creating build directory"
ifeq ($(PLATFORM),WINDOWS)
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(BUILD_DIR)/ui" mkdir "$(BUILD_DIR)/ui"
	@if not exist "$(BUILD_DIR)/entity" mkdir "$(BUILD_DIR)/entity"
	@if not exist "$(BUILD_DIR)/context" mkdir "$(BUILD_DIR)/context"
else
	mkdir -p $(BUILD_DIR)/ui $(BUILD_DIR)/entity $(BUILD_DIR)/context
endif

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c ## Compile source files with progress
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: shared
shared: $(BUILD_DIR) $(OBJ_FILES) ## Build shared library
	@echo "[INFO] Building shared library: $(SO_NAME)"
ifeq ($(PLATFORM),WINDOWS)
	@$(CC) -shared $(CFLAGS) -o $(SO_NAME) $(OBJ_FILES) -Wl,--out-implib,lib$(LIBRARY_NAME).dll.a
else
	@$(CC) -shared $(CFLAGS) -o $(SO_NAME) $(OBJ_FILES)
endif

.PHONY: static
static: $(BUILD_DIR) $(OBJ_FILES) ## Build static library
	@echo "[INFO] Building static library: $(A_NAME)"
	@$(AR) rcs $(A_NAME) $(OBJ_FILES)

.PHONY: clean
clean: ## Remove all build files and the executable
	@echo "[INFO] Cleaning up build directory and executable."
ifeq ($(PLATFORM),WINDOWS)
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(SO_NAME)" del "$(SO_NAME)"
	@if exist "$(A_NAME)" del "$(A_NAME)"
else
	rm -rf $(BUILD_DIR) $(TARGET) $(SO_NAME) $(A_NAME)
	rm -f eidolon lvl launch temp.lvl
endif

.PHONY: dist
dist: $(SRC_FILES) ## Create a tarball of the project
	@echo "[INFO] Creating a tarball for version $(VERSION)"
ifeq ($(PLATFORM),WINDOWS)
	@echo "Windows distribution is not supported in this Makefile."
else
	./scripts/dist.sh
endif

.PHONY: compile_commands.json
compile_commands.json: $(SRC_FILES) ## Generate compile_commands.json
	@echo "[INFO] Generating compile_commands.json"
	bear -- make all

PREFIX = /usr/local

.PHONY: install
install: all ## Install libraries and headers
	@echo "[INFO] Installing castor using" $(PREFIX) "as the prefix"
ifeq ($(PLATFORM),WINDOWS)
	@echo "Windows installation is not supported in this Makefile."
else
	mkdir -p $(PREFIX)/include/castor
	cp -r ./include/* $(PREFIX)/include/castor
	cp ./libcastor.* $(PREFIX)/lib
endif

.PHONY: uninstall
uninstall: ## Uninstall castor from the machine
	@echo "[INFO] Uninstalling castor"
ifeq ($(PLATFORM),WINDOWS)
	@echo "Windows uninstallation is not supported in this Makefile."
else
	rm -rf $(PREFIX)/include/castor
	rm -f $(PREFIX)/lib/libcastor.*
endif

## Show this help message
.PHONY: help
help: ## Show this help message
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

## Enable verbose output for debugging
.PHONY: verbose
verbose: CFLAGS += -DVERBOSE
verbose: all ## Build the project in verbose mode

# Phony targets to avoid conflicts with file names
.PHONY: all clean distclean install uninstall dist compile_commands.json help verbose
