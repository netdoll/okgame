#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "bobtypes.h"
#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"

class GameLogic;

struct LobbyRoom {
    std::string id;
    std::string name;
    int players;
    int maxPlayers;
    bool hasPassword;
};

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    void connect(const std::string& url);
    void disconnect();
    void update();

    void setGame(GameLogic* game);
    void setOpponentGame(GameLogic* opponentGame);

    void listRooms();
    void createRoom(const std::string& name, bool isPrivate = false, const std::string& password = "");
    void joinRoom(const std::string& id, const std::string& password = "");
    void sendFrame(const std::string& stateJson);
    void reportScore(const std::string& mode, const std::string& name, long score, int lines, int time);

    typedef std::function<void(const std::vector<LobbyRoom>&)> RoomListCallback;
    void setRoomListCallback(RoomListCallback cb) { roomListCallback = cb; }

private:
    void threadLoop();
    void handleMessage(const std::string& msg);
    void sendEvent(const std::string& name, const Poco::JSON::Object::Ptr& data);

    Poco::Net::WebSocket* _ws = nullptr;
    std::thread _thread;
    bool _stopThread = false;
    std::mutex _sendMutex;
    
    std::queue<std::string> _incomingMessages;
    std::mutex _msgMutex;

    GameLogic* _game = nullptr;
    GameLogic* _opponentGame = nullptr;
    RoomListCallback roomListCallback;

    std::string _host;
    int _port;
};

#endif
