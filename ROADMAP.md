# Project Roadmap

## Status: Active Development
**Current Version:** 0.1.3

## In Progress
*   [ ] **Submodule Updates**
    *   Update all submodules to latest upstream.
    *   Fix broken submodule references (e.g., `lib/CLove`).

## Completed Features
*   [x] **Refactoring**
    *   [x] `std::shared_ptr` migration for `BobsGame` textures.
*   [x] **Documentation**
    *   [x] Centralized Versioning (`VERSION.md`).
    *   [x] Universal LLM Instructions (`LLM_INSTRUCTIONS.md`).
    *   [x] Submodule Dashboard (`SUBMODULES.md`).
*   [x] Basic Engine Architecture (SDL2 based)
*   [x] Submodule Integration (Extensive library support)
*   [x] Initial Visualizer Submodules Added
*   [x] **Visualizer Integration**
    *   [x] Build System Update (`CMakeLists.txt` for `lib/projectm`)
    *   [x] Render Loop Integration (`BobsGame::render`)
    *   [x] Audio Input Feed (`AudioManager` -> `projectM::pcm_add`)
    *   [x] Preset Loading & Texture Paths

## Planned Features (Backlog)
*   **Game Modes**
    *   More game mode features, modes, and goals.
    *   Custom themes, sounds, and music.
*   **Multiplayer**
    *   Better multiplayer lobby with chat.
    *   Matchmaking by region or rank.
    *   Server verification.
    *   Spectator mode, replays, challenge replay.
*   **Platform Support**
    *   Android, iOS, and Browser support.
*   **Tournament Features**
    *   Ladders, regional and global.

## Maintenance
*   [ ] Update all submodules to latest upstream.
*   [ ] Merge feature branches.
*   [ ] Continuous Integration/Deployment setup.
