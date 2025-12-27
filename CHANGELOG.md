# Changelog

## [Unreleased]

### Added
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
