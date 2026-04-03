#include "stdafx.h"
#include "LuaManager.h"

lua_State* LuaManager::L = nullptr;

// Helper function for logging from Lua
static int l_log_info(lua_State* L) {
    const char* msg = luaL_checkstring(L, 1);
    Main::log.info(string("[Lua] ") + msg);
    return 0;
}

static int l_log_error(lua_State* L) {
    const char* msg = luaL_checkstring(L, 1);
    Main::log.error(string("[Lua] ") + msg);
    return 0;
}

// Binding functions
static int l_get_score(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game()) {
        lua_pushnumber(L, (double)Main::gameEngine->getPlayer1Game()->score);
        return 1;
    }
    lua_pushnumber(L, 0);
    return 1;
}

static int l_get_level(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game()) {
        lua_pushinteger(L, Main::gameEngine->getPlayer1Game()->currentLevel);
        return 1;
    }
    lua_pushinteger(L, 0);
    return 1;
}

static int l_send_garbage(lua_State* L) {
    int amount = (int)luaL_checkinteger(L, 1);
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game()) {
        Main::gameEngine->getPlayer1Game()->queueVSGarbageToSend(amount);
    }
    return 0;
}

static int l_receive_garbage(lua_State* L) {
    int amount = (int)luaL_checkinteger(L, 1);
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game()) {
        Main::gameEngine->getPlayer1Game()->gotVSGarbageFromOtherPlayer(amount);
    }
    return 0;
}

// Grid functions
static int l_get_grid_width(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game() && Main::gameEngine->getPlayer1Game()->grid) {
        lua_pushinteger(L, Main::gameEngine->getPlayer1Game()->grid->getWidth());
        return 1;
    }
    return 0;
}

static int l_get_grid_height(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game() && Main::gameEngine->getPlayer1Game()->grid) {
        lua_pushinteger(L, Main::gameEngine->getPlayer1Game()->grid->getHeight());
        return 1;
    }
    return 0;
}

static int l_get_tile(lua_State* L) {
    int x = (int)luaL_checkinteger(L, 1);
    int y = (int)luaL_checkinteger(L, 2);

    if (Main::gameEngine && Main::gameEngine->getPlayer1Game() && Main::gameEngine->getPlayer1Game()->grid) {
        shared_ptr<Block> b = Main::gameEngine->getPlayer1Game()->grid->get(x, y);
        if (b && b->blockType) {
            lua_pushstring(L, b->blockType->name.c_str());
            return 1;
        }
    }
    lua_pushnil(L);
    return 1;
}

static int l_set_tile(lua_State* L) {
    int x = (int)luaL_checkinteger(L, 1);
    int y = (int)luaL_checkinteger(L, 2);
    const char* typeName = luaL_checkstring(L, 3);

    if (Main::gameEngine && Main::gameEngine->getPlayer1Game()) {
        shared_ptr<GameLogic> g = Main::gameEngine->getPlayer1Game();
        shared_ptr<Grid> grid = g->grid;
        if (grid) {
            shared_ptr<BlockType> bt = g->currentGameType->getBlockTypeByName(typeName);
            if (bt) {
                shared_ptr<Block> b = make_shared<Block>(g.get(), grid.get(), nullptr, bt);
                grid->add(x, y, b);
                return 0;
            } else {
                Main::log.error(string("Lua setTile error: BlockType not found: ") + typeName);
            }
        }
    }
    return 0;
}

static int l_shake_screen(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game() && Main::gameEngine->getPlayer1Game()->grid) {
        Main::gameEngine->getPlayer1Game()->grid->shakeSmall();
    }
    return 0;
}

static int l_wiggle_screen(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game() && Main::gameEngine->getPlayer1Game()->grid) {
        Main::gameEngine->getPlayer1Game()->grid->wigglePlayingField();
    }
    return 0;
}

// Piece functions
static int l_get_piece_info(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game()) {
        shared_ptr<Piece> p = Main::gameEngine->getPlayer1Game()->currentPiece;
        if (p) {
            lua_newtable(L);
            lua_pushstring(L, "x");
            lua_pushinteger(L, p->xGrid);
            lua_settable(L, -3);
            lua_pushstring(L, "y");
            lua_pushinteger(L, p->yGrid);
            lua_settable(L, -3);
            lua_pushstring(L, "rotation");
            lua_pushinteger(L, p->currentRotation);
            lua_settable(L, -3);
            lua_pushstring(L, "type");
            lua_pushstring(L, p->pieceType->name.c_str());
            lua_settable(L, -3);
            return 1;
        }
    }
    return 0;
}

static int l_get_hold_piece(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game()) {
        shared_ptr<Piece> p = Main::gameEngine->getPlayer1Game()->holdPiece;
        if (p) {
            lua_pushstring(L, p->pieceType->name.c_str());
            return 1;
        }
    }
    lua_pushnil(L);
    return 1;
}

static int l_get_next_pieces(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer1Game()) {
        auto nextQueue = Main::gameEngine->getPlayer1Game()->nextPieces;
        lua_newtable(L);
        for (size_t i = 0; i < nextQueue.size(); i++) {
            lua_pushinteger(L, i + 1);
            lua_pushstring(L, nextQueue.get(i)->pieceType->name.c_str());
            lua_settable(L, -3);
        }
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

// RPG / Map functions
static int l_get_current_map_name(lua_State* L) {
    if (Main::mapManager && Main::mapManager->currentMap && Main::mapManager->currentMap->mapData) {
        lua_pushstring(L, Main::mapManager->currentMap->mapData->name.c_str());
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int l_get_player_x(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer()) {
        lua_pushnumber(L, Main::gameEngine->getPlayer()->x());
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

static int l_get_player_y(lua_State* L) {
    if (Main::gameEngine && Main::gameEngine->getPlayer()) {
        lua_pushnumber(L, Main::gameEngine->getPlayer()->y());
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

// Audio functions
static int l_play_sound(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    float vol = (float)luaL_optnumber(L, 2, 1.0);
    float pitch = (float)luaL_optnumber(L, 3, 1.0);
    if (Main::getAudioManager()) {
        Main::getAudioManager()->playSound(name, vol, pitch);
    }
    return 0;
}

static int l_play_music(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    float vol = (float)luaL_optnumber(L, 2, 1.0);
    if (Main::getAudioManager()) {
        Main::getAudioManager()->playMusic(name, vol, 1.0f, true);
    }
    return 0;
}

static int l_stop_music(lua_State* L) {
    if (Main::getAudioManager()) {
        Main::getAudioManager()->stopAllMusic();
    }
    return 0;
}

// Sprite functions
static int l_create_screen_sprite(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    const char* spriteName = luaL_checkstring(L, 2);
    float x = (float)luaL_checknumber(L, 3);
    float y = (float)luaL_checknumber(L, 4);

    if (Main::getSpriteManager()) {
        shared_ptr<ScreenSprite> s = make_shared<ScreenSprite>(Main::getEngine(), name, spriteName);
        s->setX(x);
        s->setY(y);
        Main::getSpriteManager()->screenSpriteList.add(s);
        // We could return a handle or ID here if needed
    }
    return 0;
}

void LuaManager::registerEngineBindings() {
    lua_register(L, "log", l_log_info);
    lua_register(L, "logError", l_log_error);
    lua_register(L, "getScore", l_get_score);
    lua_register(L, "getLevel", l_get_level);
    lua_register(L, "sendGarbage", l_send_garbage);
    lua_register(L, "receiveGarbage", l_receive_garbage);
    lua_register(L, "getGridWidth", l_get_grid_width);
    lua_register(L, "getGridHeight", l_get_grid_height);
    lua_register(L, "getTile", l_get_tile);
    lua_register(L, "setTile", l_set_tile);
    lua_register(L, "shakeScreen", l_shake_screen);
    lua_register(L, "wiggleScreen", l_wiggle_screen);
    lua_register(L, "getPieceInfo", l_get_piece_info);
    lua_register(L, "getHoldPiece", l_get_hold_piece);
    lua_register(L, "getNextPieces", l_get_next_pieces);
    lua_register(L, "getCurrentMapName", l_get_current_map_name);
    lua_register(L, "getPlayerX", l_get_player_x);
    lua_register(L, "getPlayerY", l_get_player_y);
    
    lua_register(L, "playSound", l_play_sound);
    lua_register(L, "playMusic", l_play_music);
    lua_register(L, "stopMusic", l_stop_music);
    lua_register(L, "createScreenSprite", l_create_screen_sprite);
}

void LuaManager::init() {
    L = luaL_newstate();
    luaL_openlibs(L);

    registerEngineBindings();

    Main::log.info("Lua initialized successfully");
    
    // Execute initialization script
    executeFile("data/scripts/init.lua");
}

void LuaManager::shutdown() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

void LuaManager::update() {
    if (!L) return;
    
    // Call onUpdate global function if it exists
    lua_getglobal(L, "onUpdate");
    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != 0) {
            Main::log.error(string("Lua onUpdate error: ") + lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    } else {
        lua_pop(L, 1);
    }
}

void LuaManager::executeString(const string& code) {
    if (!L) return;
    if (luaL_dostring(L, code.c_str())) {
        Main::log.error(string("Lua error: ") + lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void LuaManager::executeFile(const string& filename) {
    if (!L) return;
    string fullPath = Main::getPath() + filename;
    if (luaL_dofile(L, fullPath.c_str())) {
        Main::log.error(string("Lua error in file ") + filename + ": " + lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}
