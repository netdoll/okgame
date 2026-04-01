#ifndef LOBBYMENUPANEL_H
#define LOBBYMENUPANEL_H

#include "stdafx.h"
#include "MenuPanel.h"
#include "Engine/network/NetworkManager.h"

class LobbyMenuPanel : public MenuPanel {
public:
    LobbyMenuPanel();
    virtual ~LobbyMenuPanel();

    virtual void init() override;
    virtual void update() override;
    virtual void render() override;

private:
    void onRoomListReceived(const std::vector<LobbyRoom>& rooms);
    void onChatMessageReceived(Poco::Dynamic::Var data);
    void rebuildMenu();
    void startNetworkGame(long long seed, const std::string& gameMode, int startLevel);
    std::string getSelectedGameMode() const;
    std::string getRoomSummary(const LobbyRoom& room) const;
    std::string getPasswordStars() const;

    std::vector<LobbyRoom> m_rooms;
    std::vector<std::string> m_chatMessages;
    BobMenu* menu = nullptr;
    std::mutex m_chatMutex;
    std::mutex m_roomMutex;
    bool m_roomsUpdated = false;
    bool m_chatUpdated = false;
    bool m_textStarted = false;
    bool m_connected = false;
    bool m_joinedRoom = false;
    bool m_privateRoom = false;
    int m_lastMouseX = 0;
    int m_lastMouseY = 0;
    int m_startLevel = 1;
    int m_gameModeIndex = 0;
    std::string m_roomName = "New Room";
    std::string m_roomPassword;
    std::string m_joinedRoomName;
    std::string m_statusMessage = "Edit the room options, create a room, or join one from the list below.";
     
    long long m_lastRefreshTime = 0;
};

#endif
