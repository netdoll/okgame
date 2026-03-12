#pragma once

#include "src/Utility/oktypes.h"
#include "GameType.h"
#include <memory>
#include <string>
#include <vector>

class BobsGame;

class CustomGameEditor
{
public:
    CustomGameEditor(BobsGame* bobsGame);
    virtual ~CustomGameEditor();

    void init();
    void update();
    void render();

    bool isShowing = false;

private:
    BobsGame* bobsGame;
    shared_ptr<GameType> currentGameType;

    // UI State variables
    char nameBuffer[256];
    int selectedGameMode;
    int gridWidth;
    int gridHeight;
    int gravityTicks;
    int lockDelayTicks;
    int chainAmount;
    bool holdPieceEnabled;
    bool nextPieceEnabled;
    bool hardDropPunch;

    int selectedBlockTypeIndex = -1;
    int selectedPieceTypeIndex = -1;

    void loadFromGameType();
    void saveToGameType();

    void renderSettingsTab();
    void renderBlocksTab();
    void renderPiecesTab();
    void renderMainButtons();
};
