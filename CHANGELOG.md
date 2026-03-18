# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.9.0] - 2026-03-17

### Added
- Complete API parity in Java `NetworkManager` including `listRooms`, `createRoom`, `joinRoom`, `sendFrame`, and customizable `on` event listeners.
- Implemented `GameStateData` POJOs in Java `GameLogic` for robust JSON serialization via Gson.
- Java `Grid` and `GameLogic` now support full `getState()` and `applyState()` operations matching the Web fork.
- `BobColor` utility in Java now supports `toInt()` and `fromInt(int)` static parsers for compact color serialization.

### Fixed
- Re-added `NetworkManager` constructor and `removeListener` cleanup in `GameLogic` to prevent memory leaks during state transitions.
- Adjusted `Block` initialization in `Grid.applyState` to match correct constructor signatures.
- Cleaned up obsolete test directories in `bobsgameonlinejava` that referenced missing/refactored internal packages.

### Changed
- Bumped version to 0.9.0.
- `NetworkManager` now maintains a HashMap of listeners internally, allowing event subscriptions before socket connection is fully established.

## [0.8.0] - 2026-03-17

### Added
- Real-time opponent board rendering in Web fork (`isOpponent` config in `PuzzleRenderer`).
- High-frequency "Frame Synchronization" in Web multiplayer via `getState`/`applyState`.
- Web `NetworkManager` now extends `EventEmitter` for robust internal event handling.
- Integrated `BobColor` serialization into `Grid` state for accurate cross-client board visualization.

### Fixed
- Multiple TypeScript type errors in `Grid.ts`, `Block.ts`, and `NetworkManager.ts`.
- Resolved `EventEmitter` override conflicts in Web networking layer.
- Corrected `Block` instantiation in `applyState` to use proper `grid` and `blockType` references.

### Changed
- Bumped version to 0.8.0.
- `PuzzleScene` layout updated to side-by-side boards in multiplayer mode.

## [0.7.0] - 2026-03-17

### Added
- Mock business logic in Java `LoginScreen` and `CreateNewAccount` for immediate state transitions.
- Fully simulated Steamworks SDK linking with local stubs (`lib/steam`) so `okgame` can compile with `HAVE_STEAMWORKS=1`.
- Web Lobby transition logic: `LobbyScene` now correctly waits for a `gameStart` server event before pushing the `PuzzleScene`.
- Full room lifecycle in Node.js server (creates room, waits for max players, then emits `gameStart` with synced seed).

### Changed
- Bumped version to 0.7.0.
- `NetworkManager` connect method safely ignores redundant connection attempts, allowing seamless `LobbyScene` to `PuzzleScene` transitions.

## [0.6.0] - 2026-03-17

### Added
- Completed Java UI migration from legacy TWL to modern LibGDX Scene2D.
- Ported all core menus (`LoginScreen`, `StuffMenu`, `GameStore`, `PlayerEditMenu`, etc.) to `Scene2DPanel` architecture.
- Fully implemented interactive `LobbyScene` in Web fork with dynamic room list and room creation.
- Unified UI lifecycle management in Java `Engine` class using `Stage` and `Skin`.
- Added compatibility stubs for legacy Java menus to ensure stable build during migration.

### Fixed
- Resolved multiple TypeScript errors in `bobsgameweb` related to scene management and networking.
- Fixed `StateManager` type reference in Web fork by exporting `StateManagerClass`.
- Corrected various layout and theme issues in the Java fork.

### Changed
- Bumped version to 0.6.0.
- Modernized base `Scene` class in Web fork to store configuration and manager references.

## [0.5.2] - 2026-03-17

### Added
- Integrated LibGDX `Stage` and `Skin` into the base `Engine` class for modern UI support in Java.
- Created `Scene2DPanel` base class in Java to replace legacy `MenuPanel` with LibGDX `Scene2D`.
- Modernized `CreateNewAccount` screen in Java using `TextField`, `Label`, and `TextButton`.
- Enhanced Node.js `socket.io` server with room creation, joining, and listing logic.
- Added `LobbyScene.ts` shell to Web fork for multiplayer room management.

### Fixed
- Fixed UI layout consistency in Java fork by centralizing `Skin` management in `GLUtils`.

### Changed
- Bumped version to 0.5.2.

## [0.5.1] - 2026-03-17

### Added
- `SteamManager` in C++ fork (stubbed implementation for Steamworks SDK).
- `NetworkManager` in Java fork using `socket.io-client-java` for unified multiplayer.
- `GameLogicListener` interface in Java to match TypeScript `EventEmitter` patterns.

### Fixed
- Fixed missing `SpriteManager` in `BobsGameStadium.java`.
- Restored missing `GLUtils.java` drawing methods for broad codebase compatibility.
- Fixed `SDL_SetWindowFullscreen` parameter change in SDL3.

### Changed
- Bumped version to 0.5.1.
- Updated `BobsGame.java` to support optional network initialization.

## [0.5.0] - 2026-03-17

### Added
- Advanced Java Renderer with Sprite support in LibGDX.
- Automatic batch/shape state switching in `GLUtils.java` for optimized Java rendering.
- `NetworkManager` in TypeScript fork using `socket.io-client`.
- Simple WebSocket server in `bobsgameweb/server` for multiplayer testing.
- `garbageSent` event in `GameLogic` for multiplayer synchronization.

### Fixed
- Fixed 10+ TypeScript compilation errors in `bobsgameweb`.
- Corrected SDL3 Gamepad button/axis constants in `ControlsManager.cpp`.
- Fixed performance-heavy state switching in Java renderer.

### Changed
- Bumped version to 0.5.0.
- Modernized `LibretroGame.cpp` to use `SDL_AudioStream` for better audio quality and compatibility.

## [0.4.0] - 2026-03-16

### Added
- `GameManager` interface in Java fork to unify `BobsGame` and `BobsGameStadium` engines.
- SDL3 support in C++ fork (modernized `main.cpp` and `ControlsManager`).
- Parity fields in Java `BlockType` to match TypeScript and C++ expectations.

### Fixed
- All compilation errors in `bobsgameonlinejava`.
- Redundant enum definitions in Java fork (unified into `GameType.java`).
- SDL3 event and gamepad constant naming in C++ fork.

### Changed
- Bumped version to 0.4.0.
- Modernized `ControlsManager` to use `SDL_Gamepad` API.

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
