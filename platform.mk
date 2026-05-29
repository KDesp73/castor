# Platform detection and shared flags for Castor subprojects.

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  OS := macos
  CC ?= clang
else
  OS := linux
  CC ?= gcc
endif

PKG_CONFIG ?= pkg-config
SDL2_CFLAGS := $(shell $(PKG_CONFIG) --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer 2>/dev/null)
SDL2_LIBS   := $(shell $(PKG_CONFIG) --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer 2>/dev/null)

ifeq ($(SDL2_LIBS),)
  ifeq ($(OS),macos)
    BREW_PREFIX := $(shell brew --prefix 2>/dev/null)
    ifneq ($(BREW_PREFIX),)
      SDL2_CFLAGS := -I$(BREW_PREFIX)/include/SDL2 -I$(BREW_PREFIX)/include -D_THREAD_SAFE
      SDL2_LIBS := -L$(BREW_PREFIX)/lib -lSDL2_mixer -lSDL2_ttf -lSDL2_image -lSDL2
    endif
  endif
endif

ifeq ($(OS),macos)
  RAYLIB_LDFLAGS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm
  SHARED_LIB_FLAGS := -dynamiclib -install_name @rpath/libcastor.$(VERSION).dylib
  DYLIB_NAME = libcastor.$(VERSION).dylib
  DYLIB_SONAME = libcastor.dylib
  SHARED_LIB_EXT := dylib
else
  RAYLIB_LDFLAGS := -lGL -lX11 -lpthread -ldl -lrt -lm
  SHARED_LIB_FLAGS := -shared
  DYLIB_NAME = libcastor.so.$(VERSION)
  DYLIB_SONAME = libcastor.so
  SHARED_LIB_EXT := so
endif

ifeq ($(type),RELEASE)
  SANITIZERS :=
else
  SANITIZERS := # -fsanitize=address,undefined
endif
