#include "stdafx.h"
#include "CustomGameEditor.h"
#include "BobsGame.h"
#include "GameType.h"
#include "BlockType.h"
#include "PieceType.h"
#include "src/Utility/System.h"

// ImGui headers would normally be included here
// #include "imgui.h"

CustomGameEditor::CustomGameEditor(BobsGame* b) : bobsGame(b)
{
    init();
}

CustomGameEditor::~CustomGameEditor()
{
}

void CustomGameEditor::init()
{
    currentGameType = make_shared<GameType>();
    loadFromGameType();
}

void CustomGameEditor::update()
{
    // Update logic independent of rendering
}

void CustomGameEditor::render()
{
    if (!isShowing) return;

    // ImGui scaffold
    /*
    ImGui::Begin("Custom Game Editor", &isShowing);
    
    if (ImGui::BeginTabBar("EditorTabs"))
    {
        if (ImGui::BeginTabItem("Settings"))
        {
            renderSettingsTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Blocks"))
        {
            renderBlocksTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Pieces"))
        {
            renderPiecesTab();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    renderMainButtons();
    
    ImGui::End();
    */
}

void CustomGameEditor::loadFromGameType()
{
    if (currentGameType)
    {
        strncpy(nameBuffer, currentGameType->name.c_str(), sizeof(nameBuffer));
        nameBuffer[sizeof(nameBuffer) - 1] = '\0';
        
        selectedGameMode = currentGameType->gameMode;
        gridWidth = currentGameType->gridWidth;
        gridHeight = currentGameType->gridHeight;
        gravityTicks = currentGameType->gravityRule_ticksToMoveDownBlocksOverBlankSpaces;
        lockDelayTicks = currentGameType->maxLockDelayTicks;
        chainAmount = currentGameType->chainRule_AmountPerChain;
        holdPieceEnabled = currentGameType->holdPieceEnabled;
        nextPieceEnabled = currentGameType->nextPieceEnabled;
        hardDropPunch = currentGameType->hardDropPunchThroughToLowestValidGridPosition;
    }
}

void CustomGameEditor::saveToGameType()
{
    if (currentGameType)
    {
        currentGameType->name = string(nameBuffer);
        currentGameType->gameMode = selectedGameMode;
        currentGameType->gridWidth = gridWidth;
        currentGameType->gridHeight = gridHeight;
        currentGameType->gravityRule_ticksToMoveDownBlocksOverBlankSpaces = gravityTicks;
        currentGameType->maxLockDelayTicks = lockDelayTicks;
        currentGameType->chainRule_AmountPerChain = chainAmount;
        currentGameType->holdPieceEnabled = holdPieceEnabled;
        currentGameType->nextPieceEnabled = nextPieceEnabled;
        currentGameType->hardDropPunchThroughToLowestValidGridPosition = hardDropPunch;
    }
}

void CustomGameEditor::renderSettingsTab()
{
    /*
    ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer));
    
    const char* modes[] = { "Drop", "Stack" };
    ImGui::Combo("Mode", &selectedGameMode, modes, IM_ARRAYSIZE(modes));
    
    ImGui::InputInt("Grid Width", &gridWidth);
    ImGui::InputInt("Grid Height", &gridHeight);
    ImGui::InputInt("Gravity Ticks", &gravityTicks);
    ImGui::InputInt("Lock Delay", &lockDelayTicks);
    ImGui::InputInt("Chain Amount", &chainAmount);
    
    ImGui::Checkbox("Hold Piece", &holdPieceEnabled);
    ImGui::Checkbox("Next Piece", &nextPieceEnabled);
    ImGui::Checkbox("Hard Drop Punch", &hardDropPunch);
    */
}

void CustomGameEditor::renderBlocksTab()
{
    /*
    ImGui::BeginChild("BlockList", ImVec2(150, 0), true);
    for (int i = 0; i < currentGameType->blockTypes.size(); i++)
    {
        string label = currentGameType->blockTypes[i]->name + "##" + to_string(i);
        if (ImGui::Selectable(label.c_str(), selectedBlockTypeIndex == i))
            selectedBlockTypeIndex = i;
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    ImGui::BeginChild("BlockEditor");
    if (selectedBlockTypeIndex >= 0 && selectedBlockTypeIndex < currentGameType->blockTypes.size())
    {
        auto bt = currentGameType->blockTypes[selectedBlockTypeIndex];
        // ImGui editor fields for block type properties
    }
    ImGui::EndChild();
    */
}

void CustomGameEditor::renderPiecesTab()
{
    /*
    ImGui::BeginChild("PieceList", ImVec2(150, 0), true);
    for (int i = 0; i < currentGameType->pieceTypes.size(); i++)
    {
        string label = currentGameType->pieceTypes[i]->name + "##" + to_string(i);
        if (ImGui::Selectable(label.c_str(), selectedPieceTypeIndex == i))
            selectedPieceTypeIndex = i;
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    ImGui::BeginChild("PieceEditor");
    if (selectedPieceTypeIndex >= 0 && selectedPieceTypeIndex < currentGameType->pieceTypes.size())
    {
        auto pt = currentGameType->pieceTypes[selectedPieceTypeIndex];
        // ImGui editor fields for piece type properties
    }
    ImGui::EndChild();
    */
}

void CustomGameEditor::renderMainButtons()
{
    /*
    ImGui::Separator();
    if (ImGui::Button("Save"))
    {
        saveToGameType();
        // save to file logic
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        // load from file logic
    }
    ImGui::SameLine();
    if (ImGui::Button("New"))
    {
        currentGameType = make_shared<GameType>();
        loadFromGameType();
    }
    */
}
