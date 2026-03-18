# Handoff - 2026-03-17

## Current Status - Version 1.1.0
- **Node.js Server:** LEADERBOARDS IMPLEMENTED. The server now features persistent leaderboards across game modes (Marathon, Sprint, Ultra), backed by a `leaderboards.json` file. It receives cross-platform telemetry via `reportScore` and serves live top-10 lists.
- **Web fork (bobsgameweb):** LOBBY POLISH. `LobbyScene.ts` fetches and displays the live Marathon leaderboard alongside active rooms. The puzzle scene successfully dispatches final stats upon game over.
- **C++ fork (okgame):** AUDIO POLISH COMPLETE. Realigned the `AudioManager` visualizer callback hooks with the new `SDL3_mixer` signature (`MIX_PostMixCallback`) and correctly handled the translation of float streams to int16 format.

## Accomplishments
- Implemented robust and persistent high-score tracking, a critical component of multiplayer persistence.
- Visually integrated global stats into the Web multiplayer lobby.
- Addressed low-level audio integration friction in the modernized C++ fork.
- Handled edge cases with JSON payload parsing in the Node.js server to ensure compatibility with Java's Gson serialization.

## Next Steps
- **Steam Integration Polish:** This is the only remaining major task. Since the SDK is stubbed, we'll need the proprietary Steamworks binaries placed into `okgame/lib/steam` to complete and verify the in-game overlay and achievements.
- **Visualizer Shaders:** Modernize `projectM` shaders for SDL3 compatibility and link them into the updated graphics pipeline.
- **Client Settings:** Build out the Settings UI in the Web and Java forks to allow users to set their display names (currently hardcoded as "WebPlayer" and "JavaPlayer" in score reports).

## Technical Notes
- Server gracefully handles both Object payloads (from Web) and stringified JSON payloads (from Java) via a safe `JSON.parse` try/catch block.
- Leaderboards are restricted to the Top 10 entries per mode and are saved to disk synchronously on every update to ensure no data loss.
