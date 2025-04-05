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
- **Raylib** (optional UI elements)
- No scripting language yet — game logic is written in C

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

## 🛠️ Building the Engine

```bash
cd engine
make all
```

- Produces a `build/castor` binary
- Requires a C compiler and SDL2 development headers
- SDL dependencies can be pulled from `deps/` if needed

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
- Converts `.lvl` text files into binary levels
- Packs and indexes assets from `.list` files

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

Currently closed-source for private development and research. For collaboration or licensing inquiries, contact the author.
