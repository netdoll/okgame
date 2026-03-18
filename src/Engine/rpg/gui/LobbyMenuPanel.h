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
    void startNetworkGame(long long seed);

    std::vector<LobbyRoom> m_rooms;
    std::vector<std::string> m_chatMessages;
    std::mutex m_chatMutex;
    std::mutex m_roomMutex;
    bool m_roomsUpdated = false;
    bool m_chatUpdated = false;
    
    long long m_lastRefreshTime = 0;
};

#endif
