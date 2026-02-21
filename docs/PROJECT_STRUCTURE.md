# Project Structure & Submodules

## Directory Structure

*   `src/`: Main source code directory.
    *   `Engine/`: Core engine logic (Entity, Map, Network, GUI).
        *   `nd/`: "ND" device emulation (including `LibretroGame` frontend).
    *   `Puzzle/`: Game-specific logic (BobsGame puzzle mechanics).
    *   `Utility/`: Helper classes (Audio, GLUtils, System).
    *   `main.cpp`: Entry point.
*   `lib/`: External libraries and submodules.
*   `resources/`: Game assets (ProjectM presets, etc).
*   `data/`: Runtime data.
*   `docs/`: Project documentation.

## Submodules Dashboard

The following external libraries are linked as git submodules in `lib/`.

| Submodule | Path | URL | Description |
| :--- | :--- | :--- | :--- |
| **Bobcoin** | `lib/bobcoin` | `https://github.com/robertpelloni/bobcoin` | In-game economy token (Solana/Monero hybrid) |
| **ProjectM** | `lib/projectm` | `https://github.com/robertpelloni/projectm` | Music Visualizer Engine |
| **MilkDrop3** | `lib/MilkDrop3` | `https://github.com/robertpelloni/MilkDrop3` | MilkDrop3 Implementation |
| **Gambatte** | `lib/gambatte-libretro` | `https://github.com/libretro/gambatte-libretro` | GameBoy Color Emulator Core |
| **Libretro Common** | `lib/libretro-common` | `https://github.com/libretro/libretro-common` | Shared Libretro utilities |
| **GWEN** | `lib/GWEN` | `https://github.com/robertpelloni/GWEN` | GUI Library |
| **Poco** | `lib/poco` | `https://github.com/pocoproject/poco` | Network & Utility Library |
| **SDL** | `lib/SDL` | `https://github.com/libsdl-org/SDL` | Simple DirectMedia Layer |
| **SDL_image** | `lib/SDL_image` | `https://github.com/libsdl-org/SDL_image` | Image loading |
| **SDL_mixer** | `lib/SDL_mixer` | `https://github.com/libsdl-org/SDL_mixer` | Audio mixing |
| **SDL_net** | `lib/SDL_net` | `https://github.com/libsdl-org/SDL_net` | Networking |
| **NanoVG** | `lib/nanovg` | `https://github.com/robertpelloni/nanovg` | Vector Graphics |
| **CTPL** | `lib/CTPL` | `https://github.com/vit-vit/CTPL` | Thread Pool |

*(Note: This list is a subset of the extensive submodule list found in `.gitmodules`. See that file for the full list of ~60 dependencies including various presets and codecs.)*

## Dependency Management
To update all submodules to their latest committed state:
```bash
git submodule update --init --recursive
```
To fetch the latest upstream changes for all submodules:
```bash
git submodule foreach git pull origin master
```
