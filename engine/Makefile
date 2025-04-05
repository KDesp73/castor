# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Iinclude -fPIC -Ideps/include
LDFLAGS = -Ldeps/lib -lSDL2 -lSDL2_ttf -lSDL2_image -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Version info
version_file = include/version.h
VERSION_MAJOR = $(shell sed -n -e 's/\#define CASTOR_VERSION_MAJOR \([0-9]*\)/\1/p' $(version_file))
VERSION_MINOR = $(shell sed -n -e 's/\#define CASTOR_VERSION_MINOR \([0-9]*\)/\1/p' $(version_file))
VERSION_PATCH = $(shell sed -n -e 's/\#define CASTOR_VERSION_PATCH \([0-9]*\)/\1/p' $(version_file))
VERSION = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

LIBRARY_NAME = castor
SO_NAME = lib$(LIBRARY_NAME).so.$(VERSION)
A_NAME = lib$(LIBRARY_NAME).a.$(VERSION)

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

# Default target
.DEFAULT_GOAL := help

# Total source file count
TOTAL_FILES := $(words $(SRC_FILES))

# Counter to track progress
counter = 0

# Targets

.PHONY: all
all: $(BUILD_DIR) static shared ## Build the project
	@echo "Build complete."

$(BUILD_DIR): ## Create the build directory if it doesn't exist
	@echo "[INFO] Creating build directory"
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/ui
	mkdir -p $(BUILD_DIR)/asset
	mkdir -p $(BUILD_DIR)/core
	mkdir -p $(BUILD_DIR)/world/entity
	mkdir -p $(BUILD_DIR)/player

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c ## Compile source files with progress
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

DEPS_DIR := ../deps/lib
SO_FULL_PATH := $(DEPS_DIR)/$(SO_NAME)
A_FULL_PATH  := $(DEPS_DIR)/$(A_NAME)

.PHONY: shared
shared: $(BUILD_DIR) $(OBJ_FILES) ## Build shared library
	@echo "[INFO] Building shared library: $(SO_NAME)"
	@$(CC) -shared $(CFLAGS) -o $(SO_NAME) $(OBJ_FILES)
	@mkdir -p $(DEPS_DIR)
	@mv -f $(SO_NAME) $(SO_FULL_PATH)
	@ln -sf $(notdir $(SO_FULL_PATH)) $(DEPS_DIR)/lib$(LIBRARY_NAME).so

.PHONY: static
static: $(BUILD_DIR) $(OBJ_FILES) ## Build static library
	@echo "[INFO] Building static library: $(A_NAME)"
	@$(AR) rcs $(A_NAME) $(OBJ_FILES)
	@mkdir -p $(DEPS_DIR)
	@mv -f $(A_NAME) $(A_FULL_PATH)
	@ln -sf $(notdir $(A_FULL_PATH)) $(DEPS_DIR)/lib$(LIBRARY_NAME).a

.PHONY: clean
clean: ## Remove all build files and the executable
	@echo "[INFO] Cleaning up build directory and executable."
	rm -rf $(BUILD_DIR)

.PHONY: dist
dist: $(SRC_FILES) ## Create a tarball of the project
	@echo "[INFO] Creating a tarball for version $(VERSION)"
	./scripts/dist.sh

.PHONY: compile_commands.json
compile_commands.json: $(SRC_FILES) ## Generate compile_commands.json
	@echo "[INFO] Generating compile_commands.json"
	bear -- make all

PREFIX = /usr/local

.PHONY: install
install: all ## Install libraries and headers
	@echo "[INFO] Installing castor using" $(PREFIX) "as the prefix"
	mkdir -p $(PREFIX)/include/castor
	cp -r ./include/* $(PREFIX)/include/castor
	cp ./libcastor.* $(PREFIX)/lib

.PHONY: uninstall
uninstall: ## Uninstall castor from the machine
	@echo "[INFO] Uninstalling castor"
	rm -rf $(PREFIX)/include/castor
	rm -f $(PREFIX)/lib/libcastor.*

## Show this help message
.PHONY: help
help: ## Show this help message
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
