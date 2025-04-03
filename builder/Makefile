# Detect OS
OS := $(shell uname -s 2>/dev/null || echo Windows)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinclude -Ilib/raylib/include
LDFLAGS = -lm

ifeq ($(OS), Windows)
    EXE = .exe
	LDFLAGS += -Llib/raylib/lib -l:raylib.lib -lopengl32 -lgdi32 -lwinmm
    RM = del /Q /F
    MKDIR = mkdir
else
    EXE =
    LDFLAGS += -Llib/raylib/lib -l:libraylib.a
    RM = rm -rf
    MKDIR = mkdir -p
endif

# Directories
SRC_DIR = src
BUILD_DIR = build
DIST_DIR = dist

TARGET = lvl$(EXE)

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
	SANITIZERS =
	CFLAGS  += -DDEBUG -ggdb $(SANITIZERS)
	LDFLAGS += $(SANITIZERS)
endif

# Source and object files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

.DEFAULT_GOAL := all

.PHONY: all
all: $(BUILD_DIR) $(TARGET) ## Build the project
	@echo "Build complete."

$(BUILD_DIR):
	@echo "[INFO] Creating build directory"
	$(MKDIR) $(BUILD_DIR)

$(TARGET): $(OBJ_FILES)
	@echo "[INFO] Building the project"
	@$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: install
install: all
ifeq ($(OS), Windows)
	@echo "[INFO] Installing $(TARGET) to C:\Program Files\$(TARGET)"
	$(MKDIR) "C:\Program Files\$(TARGET)"
	cp $(TARGET).exe "C:\Program Files\$(TARGET)"
else
	@echo "[INFO] Installing $(TARGET) to /usr/bin/"
	cp $(TARGET) /usr/bin/$(TARGET)
endif

.PHONY: uninstall
uninstall:
ifeq ($(OS), Windows)
	@echo "[INFO] Uninstalling $(TARGET)"
	$(RM) "C:\Program Files\$(TARGET)\$(TARGET).exe"
else
	@echo "[INFO] Uninstalling $(TARGET)"
	rm -f /usr/bin/$(TARGET)
endif

.PHONY: clean
clean:
	@echo "[INFO] Cleaning up build directory."
	$(RM) $(BUILD_DIR) $(TARGET)
