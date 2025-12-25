# Universal LLM Instructions

This file contains universal instructions for all AI models (Claude, Gemini, GPT, Copilot, etc.) working on this project.

## Core Principles
1.  **Autonomy**: Proceed with tasks autonomously where possible.
2.  **Documentation**: Keep documentation up to date. Update `CHANGELOG.md` and `VERSION.md` on every significant change.
3.  **Versioning**: 
    *   Increment the version number in `VERSION.md` and `version.txt` for every build/release.
    *   Update `CHANGELOG.md` with a new entry.
    *   Ensure commit messages reference the version bump.
4.  **Code Quality**: Write clean, maintainable code. Use `std::shared_ptr` for memory management in C++.
5.  **Project Structure**: Respect the existing directory structure.
    *   `src/`: Source code.
    *   `lib/`: External libraries and submodules.
    *   `data/`: Game assets.
    *   `cmake/`: CMake configuration files.

## Task Protocol
1.  **Analyze**: Understand the task and the codebase.
2.  **Plan**: Create a todo list or roadmap.
3.  **Execute**: Implement the changes.
4.  **Verify**: Check for errors (compilation, logic).
5.  **Document**: Update documentation and changelogs.
6.  **Commit**: (Simulated) Commit changes with a descriptive message.

## Specific Instructions
*   **Submodules**: When updating submodules, ensure `SUBMODULES.md` is updated.
*   **Visualizer**: The visualizer integration is a high priority. Refer to `AGENTS.md` for details.
*   **Refactoring**: Continue refactoring to modern C++ standards.

## Agent Personas
*   **Copilot**: Expert AI programming assistant.
*   **Claude**: Analytical and detailed researcher/coder.
*   **Gemini**: Creative and robust problem solver.
*   **GPT**: Versatile generalist coder.

---
*This file is referenced by CLAUDE.md, GEMINI.md, GPT.md, and copilot-instructions.md.*
