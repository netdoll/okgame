# Changelog

## [0.8.6] - 2024-05-22

### Added
- **Economy:** Added `lib/bobcoin` submodule. This implements the new in-game currency based on a Solana/Monero hybrid with "Proof-of-Exercise" mining.
- **Documentation:** Created `docs/AGENTS_COMMON.md` to standardize instructions across all AI assistants. Updated `AGENTS.md`, `CLAUDE.md`, etc.
- **Versioning:** Introduced `VERSION.md` as the single source of truth for the project version.

### Changed
- **Documentation:** Updated `docs/PROJECT_STRUCTURE.md` to include `bobcoin`.
- **System:** Updated build system to acknowledge the new submodule (pending CMake integration).

## [0.8.5] - 2024-05-22

### Added
- **Documentation:** Added `PROJECT_STRUCTURE.md` detailing directory layout and submodules.
- **Memory Management:** Replaced raw pointers with `std::shared_ptr` throughout the Engine.
- **ProjectM:** Integrated `libprojectM` for music visualization. Added `projectMPresetSwitchTimer` for autopilot.
- **Libretro:** Added full Libretro frontend support (`LibretroGame`).
  - Supports loading dynamic cores (`.so`/`.dll`).
  - Save States and SRAM support.
  - CRT Shaders and HQ2X upscaling.
  - Analog Input and Rumble support.
- **Build:** Updated `CMakeLists.txt` to include new dependencies.

### Changed
- Refactored `ArrayList`, `HashMap` to use smart pointers.
- Updated `AudioManager` to feed audio data to ProjectM.
- Updated `NDMenu` to support Libretro launching.
