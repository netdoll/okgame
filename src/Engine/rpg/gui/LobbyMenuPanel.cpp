#include "stdafx.h"
#include "LobbyMenuPanel.h"
#include "GUIManager.h"
#include "Puzzle/BobsGame.h"
#include "Engine/nd/ND.h"
#include "main.h"

LobbyMenuPanel::LobbyMenuPanel()
{
}

LobbyMenuPanel::~LobbyMenuPanel()
{
}

void LobbyMenuPanel::init()
{
}

void LobbyMenuPanel::onRoomListReceived(const std::vector<LobbyRoom>& rooms)
{
    std::lock_guard<std::mutex> lock(m_roomMutex);
    m_rooms = rooms;
    m_roomsUpdated = true;
}

void LobbyMenuPanel::update()
{
    MenuPanel::update();

    if (getIsActivated() == true)
    {
        if (getIsScrollingDown() == false)
        {
            if (menu == nullptr)
            {
                menu = new BobMenu(getEngine(), "Multiplayer Lobby");
                menu->add("Create Room");
                menu->addInfo("--- Rooms ---");
                menu->add("Back");
                
                Main::networkManager->setRoomListCallback([this](const std::vector<LobbyRoom>& rooms){
                    this->onRoomListReceived(rooms);
                });
                Main::networkManager->connect("http://localhost:6065");
                Main::networkManager->listRooms();
                m_lastRefreshTime = System::getTicks();
            }

            // Periodic refresh
            if (System::getTicks() - m_lastRefreshTime > 5000) {
                Main::networkManager->listRooms();
                m_lastRefreshTime = System::getTicks();
            }

            {
                std::lock_guard<std::mutex> lock(m_roomMutex);
                if (m_roomsUpdated) {
                    menu->clear();
                    menu->add("Create Room");
                    menu->addInfo("--- Rooms ---");
                    for (const auto& room : m_rooms) {
                        menu->add(room.name + " (" + std::to_string(room.players) + "/" + std::to_string(room.maxPlayers) + ")", room.id);
                    }
                    menu->add("Back");
                    m_roomsUpdated = false;
                }
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
                    if (menu->isSelectedID("Create Room", clicked, mx, my))
                    {
                        Main::networkManager->createRoom("C++ Room");
                    }
                    else if (menu->isSelectedID("Back", clicked, mx, my))
                    {
                        setActivated(false);
                    }
                    else {
                        // Check if it's a room ID
                        string id = menu->getSelectedMenuItem()->id;
                        if (id != "" && id != "Create Room" && id != "Back") {
                            Main::networkManager->joinRoom(id);
                        }
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

void LobbyMenuPanel::render()
{
    MenuPanel::render();
}
