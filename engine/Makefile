# Compiler and flags
include ../common.mk
CFLAGS = -Wall -Werror -Iinclude -I../deps/include -fPIC
CFLAGS += $(SDL2_CFLAGS)
LDFLAGS = $(SANITIZERS)

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
A_NAME = lib$(LIBRARY_NAME).a.$(VERSION)

ifeq ($(type), RELEASE)
	CFLAGS += -O3
else
	CFLAGS += -DDEBUG -ggdb $(SANITIZERS)
endif

SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

.DEFAULT_GOAL := help

TOTAL_FILES := $(words $(SRC_FILES))
counter = 0

.PHONY: all
all: $(BUILD_DIR) static shared ## Build the project
	@echo "Build complete."

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c ## Compile source files with progress
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

DYLIB_FULL_PATH := $(DEPS_LIB)/$(DYLIB_NAME)
A_FULL_PATH := $(DEPS_LIB)/$(A_NAME)

.PHONY: shared
shared: $(BUILD_DIR) $(OBJ_FILES) ## Build shared library
	@echo "[INFO] Building shared library: $(DYLIB_NAME)"
	@$(CC) $(SHARED_LIB_FLAGS) $(CFLAGS) -o $(DYLIB_NAME) $(OBJ_FILES) $(SDL2_LIBS)
	@rm -f $(DEPS_LIB)/lib$(LIBRARY_NAME).$(SHARED_LIB_EXT)*
	@mkdir -p $(DEPS_LIB)
	@mv -f $(DYLIB_NAME) $(DYLIB_FULL_PATH)
	@ln -sf $(notdir $(DYLIB_FULL_PATH)) $(DEPS_LIB)/$(DYLIB_SONAME)

.PHONY: static
static: $(BUILD_DIR) $(OBJ_FILES) ## Build static library
	@echo "[INFO] Building static library: $(A_NAME)"
	@$(AR) rcs $(A_NAME) $(OBJ_FILES)
	@rm -f $(DEPS_LIB)/lib$(LIBRARY_NAME).a*
ifeq ($(OS),macos)
	@rm -f $(DEPS_LIB)/lib$(LIBRARY_NAME).so $(DEPS_LIB)/lib$(LIBRARY_NAME).so.*
endif
	@mkdir -p $(DEPS_LIB)
	@mv -f $(A_NAME) $(A_FULL_PATH)
	@ln -sf $(notdir $(A_FULL_PATH)) $(DEPS_LIB)/lib$(LIBRARY_NAME).a

.PHONY: clean
clean: ## Remove all build files and the executable
	@echo "[INFO] Cleaning up build directory and executable."
	rm -rf $(BUILD_DIR)

.PHONY: dist
dist: $(SRC_FILES) ## Create a tarball of the project
	@echo "[INFO] Creating a tarball for version $(VERSION)"
	../scripts/dist.sh

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
	cp $(DEPS_LIB)/libcastor.* $(PREFIX)/lib

.PHONY: uninstall
uninstall: ## Uninstall castor from the machine
	@echo "[INFO] Uninstalling castor"
	rm -rf $(PREFIX)/include/castor
	rm -f $(PREFIX)/lib/libcastor.*

.PHONY: help
help: ## Show this help message
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

$(BUILD_DIR): ## Create the build directory if it doesn't exist
	@echo "[INFO] Creating build directory"
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/ui
	mkdir -p $(BUILD_DIR)/asset
	mkdir -p $(BUILD_DIR)/core
	mkdir -p $(BUILD_DIR)/world/entity
	mkdir -p $(BUILD_DIR)/player
	mkdir -p $(BUILD_DIR)/renderer
	mkdir -p $(BUILD_DIR)/tests

# Tests
TEST_SRC := $(wildcard tests/*.c)
TEST_OBJ := $(patsubst tests/%.c,$(BUILD_DIR)/tests/%.o,$(TEST_SRC))
TEST_RUNNER := $(BUILD_DIR)/tests/runner

.PHONY: test
test: static $(TEST_RUNNER) ## Run engine unit tests
	@$(TEST_RUNNER)

$(BUILD_DIR)/tests/%.o: tests/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TEST_RUNNER): $(TEST_OBJ) $(DEPS_LIB)/libcastor.a
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $(TEST_OBJ) $(DEPS_LIB)/libcastor.a $(SDL2_LIBS) -lm
