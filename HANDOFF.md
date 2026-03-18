# Handoff - 2026-03-17

## Current Status - Version 1.5.0
- **Java fork (bobsgameonlinejava):** LOBBY LEADERBOARD READY. `LobbyScreen.java` now requests and displays real-time high scores for Marathon mode, matching the Web client's functionality. The Java client can now see and join rooms created by Web clients and vice versa.
- **Matchmaking Persistence:** Both Java and Web clients now fully support private rooms and password protection.
- **Documentation:** Updated all project tracking files to reflect the completion of the Java Leaderboard and matchmaking polish.

## Accomplishments
- Achieved full cross-platform matchmaking parity: rooms, passwords, and leaderboards are now identical between Java and TypeScript.
- Improved the robustness of the Java networking layer with better thread handling and POJO-based JSON serialization.

## Next Steps
- **Steam Integration Polish:** Replace the C++ `lib/steam` stubs with real SDK binaries and verify the overlay functionality.
- **Visualizer Shaders:** Modernize `projectM` shaders for SDL3 compatibility once the submodule is restored and linked.
- **Web UI Polish:** Improve the visual fidelity of the Web lobby using custom CSS or a UI component library to match the "bob's game" theme.

## Technical Notes
- `NetworkManager.java` uses `com.google.gson.reflect.TypeToken` to safely deserialize the room list from the server into a typed `ArrayList`.
- Polling frequency for both rooms and leaderboards is synced at 5 seconds across all platforms.
