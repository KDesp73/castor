# Castor Monorepo

**Castor** is a modular, pseudo-3D game engine written in C, purpose-built for creating retro-style first-person experiences reminiscent of *Doom* and *Wolfenstein 3D*. It handles rendering, AI, audio, UI, and asset management from scratch — no external game engine required.

This monorepo contains the full engine source code, supporting tools, and a playable demo that showcases Castor’s capabilities.

---

## 🧱 Engine Overview

Located in: `engine/`

Features:
- 🧭 **Raycasting renderer** for walls, floors, ceilings, and sprites
- 👾 **Enemy AI** with patrol, chase, and attack states
- 🧰 **UI toolkit**: buttons, sliders, toasts
- 🔊 **Audio system** powered by SDL_mixer
- 🧃 **Asset loader** using `.list` files for textures and sprites
- 🧼 **Utility library** with memory-safe strings, file I/O, and math

### Built with:
- **SDL2**, SDL_image, SDL_ttf, SDL_mixer

---

## 📁 Repository Structure

```
.
├── engine/         # Castor engine source code
├── demo/           # Game demo built with Castor
├── builder/        # CLI level builder tool
├── assets/         # Shared textures, sounds, fonts
├── deps/           # Third-party libraries (SDL, Raylib, etc.)
├── dist/           # Packaged binaries for distribution
├── docs/           # Developer notes and architecture
└── README.md       # This file
```

---

## 🛠️ Building

### Linux

```bash
sudo apt install build-essential pkg-config libsdl2-dev libsdl2-image-dev \
  libsdl2-ttf-dev libsdl2-mixer-dev
make deps   # builds Raylib into deps/lib
make all
```

### macOS

```bash
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer pkg-config
export PKG_CONFIG_PATH="$(brew --prefix)/lib/pkgconfig"
make deps
make all
```

Run the demo from the **repository root** so relative `assets/` and `levels/` paths resolve:

```bash
cd demo && ./eidolon
```

Set `BUILD_TOOLS=0` to build only the engine and demo (skip builder/launcher).

### Engine only

```bash
cd engine
make static
```

---

## 🧪 Demo Game

Located in: `demo/`

A sample game built entirely using Castor to demonstrate:
- 2.5D rendering
- Combat system
- Enemy AI
- UI and inventory prototyping

```bash
cd demo
make all
./eidolon
```

---

## 🔧 Builder Tool

Located in: `builder/`

This CLI tool:
- Creates new level files
- Edits existing level files

```bash
cd builder
make all
./lvl -r 16 -c 16 --scale 50 -o 1.lvl
```

---

## 📦 Asset Pipeline

Assets are organized via `.list` files for reproducibility and automation.

Example `textures.list`:

```
wall_brick.png
wall_mossy.png
floor_tile.png
```

- Assets are index-based for faster lookup
- Sounds support WAV/OGG
- Fonts auto-scaled at runtime
- Sprite metadata includes animation info

<!-- --- -->
<!---->
<!-- ## 📄 Documentation -->
<!---->
<!-- - [`docs/Architecture.md`](docs/Architecture.md): Engine internals -->
<!-- - [`docs/UI.md`](docs/UI.md): Using the UI layer -->
<!-- - [`CHANGELOG.md`](CHANGELOG.md): Feature log -->

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).
