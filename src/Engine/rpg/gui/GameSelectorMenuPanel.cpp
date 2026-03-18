#include "stdafx.h"
#include "GameSelectorMenuPanel.h"
#include "GUIManager.h"
#include "LobbyMenuPanel.h"
#include "Puzzle/BobsGame.h"
#include "Engine/nd/ND.h"

GameSelectorMenuPanel::GameSelectorMenuPanel()
{
}

GameSelectorMenuPanel::~GameSelectorMenuPanel()
{
}

void GameSelectorMenuPanel::init()
{
}

void GameSelectorMenuPanel::update()
{
    MenuPanel::update();

    if (getIsActivated() == true)
    {
        if (getIsScrollingDown() == false)
        {
            if (menu == nullptr)
            {
                menu = new BobMenu(getEngine(), "Select Mode");
                menu->add("Single Player");
                menu->add("Multiplayer");
                menu->add("Back");
            }

            if (getIsScrolledUp())
            {
                int mx = getControlsManager()->getMouseX();
                int my = getControlsManager()->getMouseY();

                if (getControlsManager()->miniGame_UP_Pressed()) menu->up();
                if (getControlsManager()->miniGame_DOWN_Pressed()) menu->down();

                bool confirm = getControlsManager()->miniGame_CONFIRM_Pressed();
                bool clicked = getControlsManager()->mouse_LEFTBUTTON_Pressed();

                if (confirm || clicked)
                {
                    if (menu->isSelectedID("Single Player", clicked, mx, my))
                    {
                        setActivated(false);
                        
                        BobsGame* bobsGame = new BobsGame();
                        bobsGame->init();
                        getND()->setGame(bobsGame);
                        getND()->setActivated(true);
                    }
                    else if (menu->isSelectedID("Multiplayer", clicked, mx, my))
                    {
                        setActivated(false);
                        getGUIManager()->openLobbyMenu();
                    }
                    else if (menu->isSelectedID("Back", clicked, mx, my))
                    {
                        setActivated(false);
                    }
                }
            }
        }
    }

    if (getIsActivated() == false && getIsScrollingDown() == false)
    {
        if (menu != nullptr)
        {
            delete menu;
            menu = nullptr;
        }
    }
}
