#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include "stdafx.h"

extern "C" {
#include "../../lib/CLove/src/3rdparty/lua/lua.h"
#include "../../lib/CLove/src/3rdparty/lua/lualib.h"
#include "../../lib/CLove/src/3rdparty/lua/lauxlib.h"
}

class BobsGame;
class GameLogic;

class LuaManager {
public:
    static void init();
    static void shutdown();
    static void update();

    static void executeString(const string& code);
    static void executeFile(const string& filename);

    static lua_State* getLuaState() { return L; }

    static void registerEngineBindings();

private:
    static lua_State* L;
};

#endif
