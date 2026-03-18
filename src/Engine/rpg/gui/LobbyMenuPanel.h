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
    void startNetworkGame(long long seed);

    std::vector<LobbyRoom> m_rooms;
    std::mutex m_roomMutex;
    bool m_roomsUpdated = false;
    
    long long m_lastRefreshTime = 0;
};

#endif
