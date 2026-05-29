# Compiler and flags
include ../common.mk
CFLAGS = -Wall -Werror -Iinclude -I../engine/include -I../deps/include
CFLAGS += $(SDL2_CFLAGS)
LDFLAGS = $(DEPS_LIB)/libcastor.a -lm $(SDL2_LIBS) $(SANITIZERS)
ifeq ($(OS),macos)
  LDFLAGS += -Wl,-rpath,@executable_path/../deps/lib
endif

SRC_DIR = src
BUILD_DIR = build
TARGET = eidolon

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
all: check_tools $(BUILD_DIR) $(TARGET) ## Build the project
	@echo "Build complete."

.PHONY: check_tools
check_tools: ## Check if necessary tools are available
	@command -v $(CC) >/dev/null 2>&1 || { echo >&2 "[ERRO] $(CC) is not installed."; exit 1; }
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "[ERRO] pkg-config is not installed."; exit 1; }
	@test -n "$(SDL2_LIBS)" || { echo >&2 "[ERRO] SDL2 not found. Install via Homebrew or apt and set PKG_CONFIG_PATH."; exit 1; }
	@test -f $(DEPS_LIB)/libcastor.a || { echo >&2 "[ERRO] Build the engine first: make -C ../engine static"; exit 1; }

$(BUILD_DIR):
	@echo "[INFO] Creating build directory"
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ_FILES)
	@echo "[INFO] Building the project"
	@$(CC) -o $@ $^ $(LDFLAGS)
	@echo "[INFO] Executable created: $(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: install
install: all
	cp $(TARGET) /usr/local/bin/$(TARGET)

.PHONY: uninstall
uninstall:
	rm -f /usr/local/bin/$(TARGET)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: compile_commands.json
compile_commands.json: $(SRC_FILES)
	bear -- make all

.PHONY: help
help:
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

.PHONY: verbose
verbose: CFLAGS += -DVERBOSE
verbose: all

.PHONY: all clean install uninstall compile_commands.json help check_tools verbose
