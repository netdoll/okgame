# Project Roadmap

## Status: Active Development
**Current Version:** 0.1.0

## In Progress
*   [ ] **Refactoring**
    *   Continue `std::shared_ptr` migration.

## Completed Features
*   [x] Basic Engine Architecture (SDL2 based)
*   [x] Submodule Integration (Extensive library support)
*   [x] Initial Visualizer Submodules Added
*   [x] Documentation Structure (LLM Instructions, Changelog, Versioning)
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
