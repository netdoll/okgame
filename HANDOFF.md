# Handoff - 2026-03-17

## Current Status - Version 1.2.0
- **Web fork (bobsgameweb):** SETTINGS UI IMPLEMENTED. Added `SettingsScene.ts` which provides a simple HTML-over-Canvas input field to set the player's name. This name is persisted via `localStorage` and sent to the server during `reportScore` events at the end of a match.
- **Java fork (bobsgameonlinejava):** UNIQUE IDENTITIES. The Java `BobsGame` instance now generates a pseudo-random identifier (`JavaPlayer_XXX`) on startup to ensure distinct leaderboard entries during testing.
- **Documentation:** Updated `CHANGELOG.md` and `ROADMAP.md` to accurately reflect the completion of the Leaderboards and Settings tasks.

## Accomplishments
- Addressed the final immediate UI requirement for the Web and Java forks by allowing distinct player identities in the global leaderboards.
- Maintained a clean build state across all three primary branches (`okgame`, `bobsgameweb`, `bobsgameonlinejava`).

## Next Steps
- **Steam Integration Polish:** This task remains blocked until the proprietary Steamworks SDK binaries are placed into `okgame/lib/steam`. Once available, the C++ client needs to be tested with the active Steam overlay.
- **Visualizer Shaders:** The `projectM` submodules and dependencies appear to be commented out or missing from the current `CMakeLists.txt` build path. Once restored, the shaders need to be audited and updated for `SDL3` compatibility.

## Technical Notes
- Web UI uses `document.createElement('input')` overlaid on the PIXI canvas as a lightweight, accessible way to handle text input without requiring complex web-font rendering. It correctly removes itself in the `destroy()` lifecycle hook.
- The Node.js server gracefully receives these distinct names and updates `leaderboards.json` accordingly.
