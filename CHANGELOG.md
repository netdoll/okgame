# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.3.0] - 2026-03-08
### Added
- Synchronized Puzzle logic across C++, Java, and TypeScript forks.
- Ported `GameLogic`, `Grid`, `Block`, `Piece`, and `GameType` properties and methods to Java and TS.
- Implemented full chain detection logic using `GameType` rules in all forks.
- Added 25+ standard easing functions to `Easing` class.

### Changed
- Modernized Java version to Java 21 and LibGDX 1.12.1.
- Consolidated Java source tree into `src/main/java`.
- Upgraded C++ version to SDL3 (In Progress).
- Refactored `Piece` architecture to use static inner classes for `Rotation` and `BlockOffset`.

## [0.2.0] - 2026-03-07
### Changed
- Initial consolidation of Java fork.
- Modernized Networking to Netty 4.
- Added LibGDX dependency foundation.

### Added
- GitHub Actions CI/CD pipeline (.github/workflows/build.yml)
- Windows build with VS2022, Ninja, MSVC
- Linux build with GCC, Ninja, ccache
- Submodule and build caching for faster CI runs
- Concurrency control to cancel stale builds

## [0.1.7] - 2025-01-11
### Added
- Comprehensive AGENTS.md with full project documentation (architecture, conventions, code map).
- Subdirectory AGENTS.md files for src/Engine/, src/Puzzle/, src/Utility/.
- Complete SUBMODULES.md dashboard documenting all 89 submodules with versions.
- Created .github/copilot-instructions.md for VS Code Copilot integration.

### Changed
- Rewrote CLAUDE.md, GEMINI.md, GPT.md to reference LLM_INSTRUCTIONS.md as master.
- Standardized LLM instruction files with role-specific focus areas.

## [0.1.6] - 2025-12-27
### Changed
- Updated all submodules to latest versions (merged upstream changes).
- Fixed `lib/SDL`, `lib/SDL_image`, `lib/SDL_mixer`, `lib/SDL_ttf`, `lib/SDL_net` to track `SDL2` branch to avoid conflicts with SDL3.
- Updated `SUBMODULES.md` with detailed version and date information.
- Merged feature branches into main.

## [0.1.5] - 2025-12-27
### Fixed
- Refactored `EnginePart::getCurrentMap` and `Entity::getCurrentMap` to return `std::shared_ptr<Map>`.
- Updated `BobEvent`, `Cameraman`, `RandomCharacter` to match the new `getCurrentMap` signature.
- Updated `RamioGuy::render` and `ScreenSprite::render` to use `std::shared_ptr<BobTexture>`.
- Fixed `Gwen` `GetModuleFileNameW` error by using `GetModuleFileNameA`.
- Fixed `Map.h` include path for `CTPL`.

## [0.1.4] - 2025-12-27
### Fixed
- Fixed build error in `ProjectM.hpp` (missing include `Renderer/RenderContext.hpp`).
- Fixed build error in `Entity.h` (covariant return type mismatch).
- Fixed `ControlsManager.h` syntax error.
- Fixed `BobSprite.h` include path.

## [0.1.3] - 2025-12-25
### Changed
- Refactored `BobsGame` static texture members to use `std::shared_ptr`.
- Updated `Grid.cpp` to use `.get()` for texture rendering calls.
- Centralized versioning to `VERSION.md`.
- Updated `version.php` to read from `VERSION.md`.
- Consolidated LLM instructions into `LLM_INSTRUCTIONS.md`.
- Updated `ROADMAP.md` and `SUBMODULES.md`.

## [0.1.2] - 2025-12-25
### Added
- Implemented preset loading for `projectM` visualizer in `BobsGame::init()`.
- Completed visualizer integration (Build, Render, Input, Preset Loading).

## [0.1.1] - 2025-12-25
### Added
- Integrated `projectM` build configuration in `CMakeLists.txt`.
- Updated `AGENTS.md` to reference `LLM_INSTRUCTIONS.md`.

## [0.1.0] - 2025-12-25
### Added
- Initial changelog creation.
- Added `VERSION.md` and `version.txt` for centralized versioning.
- Added `LLM_INSTRUCTIONS.md` for unified AI agent instructions.
- Added `SUBMODULES.md` dashboard for submodule management.
- Added `ROADMAP.md` to track project progress.

### Changed
- Consolidated LLM instructions into `LLM_INSTRUCTIONS.md`.
- Updated `AGENTS.md` to reference the central instructions.
