# Handoff - 2026-03-17

## Current Status - Version 1.4.0
- **Node.js Server:** ENHANCED ROOM LOGIC. The server now supports creating private rooms and assigning passwords to rooms.
- **Web fork (bobsgameweb):** LOBBY UI POLISH. The `LobbyScene.ts` UI now features HTML inputs for setting room names, passwords, and private visibility. It also prompts the user for a password if the room they are trying to join requires one.
- **Java fork (bobsgameonlinejava):** LOBBY UI POLISH. The `LobbyScreen.java` UI has been updated to use `TextField` and `CheckBox` to mimic the advanced room creation and joining features implemented in the Web fork.

## Accomplishments
- Successfully unified the matchmaking experience, allowing users on both Java and Web to create public or private rooms and protect them with passwords.
- Increased server robustness by allowing strings or JSON objects in the `createRoom` and `joinRoom` packet events to support different client network libraries.

## Next Steps
- **Steam Integration Polish:** This task remains blocked until the proprietary Steamworks SDK binaries are placed into `okgame/lib/steam`. Once available, the C++ client needs to be tested with the active Steam overlay.
- **Visualizer Shaders:** The `projectM` submodules and dependencies appear to be commented out or missing from the current `CMakeLists.txt` build path. Once restored, the shaders need to be audited and updated for `SDL3` compatibility.

## Technical Notes
- Private rooms are intentionally stripped from the `roomList` broadcast payload in the Node.js server. Users must know the exact room ID and password to join a private room.
- `NetworkManager` in Java was updated to support overloaded `createRoom` and `joinRoom` methods that take complex Option objects and emit them as JSON strings.
