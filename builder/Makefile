include ../common.mk
CFLAGS = -Wall -Iinclude -I../deps/include/raylib
LDFLAGS = $(DEPS_LIB)/libraylib.a $(RAYLIB_LDFLAGS) $(SANITIZERS)

SRC_DIR = src
BUILD_DIR = build
TARGET = lvl

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
all: check_raylib $(BUILD_DIR) $(TARGET) ## Build the project
	@echo "Build complete."

.PHONY: check_raylib
check_raylib:
	@test -f $(RAYLIB_A) || { echo >&2 "[ERRO] Run 'make deps' from the repo root first."; exit 1; }

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ_FILES)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: help
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

.PHONY: all clean help check_raylib
