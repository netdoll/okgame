# Universal Agent Instructions

This file contains common guidelines for all AI agents (Claude, ChatGPT, Gemini, Copilot, etc.) working on the BobsGame monorepo.

## Core Protocols

### 1. Versioning & Changelog
*   **Single Source of Truth:** The project version is stored in `VERSION.md` (root directory).
*   **Version Bump:** Increment the version number in `VERSION.md` for *every* significant build or feature addition.
*   **Changelog:** Update `CHANGELOG.md` with a summary of changes, referencing the new version number.
*   **Commit Messages:** Include the version number in the commit message (e.g., "Bump version to 0.8.6: Add bobcoin submodule").

### 2. Git & Submodule Management
*   **Monorepo Strategy:** This repository acts as a monorepo for several personal projects (e.g., `lib/projectm`, `lib/GWEN`).
*   **Submodule Development:** When modifying code within a submodule (e.g., `lib/projectm`), you **must** commit and push those changes to the submodule's remote repository as well.
*   **Sync:** Regularly run `git submodule update --init --recursive` and merge upstream changes.
*   **Branching:** Use descriptive feature branches (e.g., `feature-bobcoin-integration`). Merge into `main` only when stable.

### 3. Documentation
*   **Dashboard:** Maintain `docs/DASHBOARD.md` to reflect the current status of all submodules and tasks.
*   **Project Structure:** Keep `docs/PROJECT_STRUCTURE.md` updated with the layout of `src/`, `lib/`, and `resources/`.
*   **Roadmap:** Update `docs/ROADMAP.md` with completed and pending features.

### 4. Code Style & Architecture
*   **Language:** C++ (Standard 11/14/17 depending on compiler support).
*   **Memory Management:** Prefer `std::shared_ptr` and `std::weak_ptr` over raw pointers. Use RAII.
*   **Pattern:** The engine uses a component-based entity system. `Engine` manages subsystems (`Audio`, `Input`, `Graphics`).

## Vision & Goals
*   **Ecosystem:** BobsGame is a platform for music, puzzle, and RPG elements.
*   **Bobcoin:** The in-game economy is based on `bobcoin` (Solana/Monero hybrid), emphasizing anonymity, speed, and "mining by dancing" (proof-of-exercise).
*   **Decentralization:** The game client should eventually act as a node for the network.

## Handoff Protocol
*   Before ending a session, update `HANDOFF.md` with a summary of actions, current state, and next steps for the following agent.
