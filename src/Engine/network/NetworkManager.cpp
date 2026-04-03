#include "stdafx.h"
#include "NetworkManager.h"
#include "Puzzle/GameLogic.h"
#include "Poco/Net/NetException.h"
#include "Poco/JSON/Array.h"
#include "Poco/Dynamic/Var.h"
#include <iostream>

NetworkManager::NetworkManager() {}

NetworkManager::~NetworkManager() {
    disconnect();
}

void NetworkManager::connect(const std::string& url) {
    if (_ws) return;

    // Simple URL parsing for ws://host:port
    size_t start = url.find("://");
    std::string path = "/socket.io/?EIO=4&transport=websocket";
    std::string hostStr = url.substr(start + 3);
    size_t colon = hostStr.find(":");
    
    _host = hostStr.substr(0, colon);
    _port = std::stoi(hostStr.substr(colon + 1));

    _stopThread = false;
    _thread = std::thread(&NetworkManager::threadLoop, this);
}

void NetworkManager::disconnect() {
    _stopThread = true;
    if (_thread.joinable()) _thread.join();
    if (_ws) {
        delete _ws;
        _ws = nullptr;
    }
}

void NetworkManager::threadLoop() {
    try {
        Poco::Net::HTTPClientSession session(_host, _port);
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/socket.io/?EIO=4&transport=websocket", Poco::Net::HTTPMessage::HTTP_1_1);
        Poco::Net::HTTPResponse response;

        _ws = new Poco::Net::WebSocket(session, request, response);
        
        char buffer[4096];
        while (!_stopThread) {
            int flags = 0;
            int n = _ws->receiveFrame(buffer, sizeof(buffer), flags);
            if (n > 0) {
                std::string msg(buffer, n);
                // Socket.io EIO=4 protocol: 
                // 0: open, 1: close, 2: ping, 3: pong, 4: message
                if (msg[0] == '2') { // Ping
                    std::string pong = "3";
                    _ws->sendFrame(pong.c_str(), pong.length(), Poco::Net::WebSocket::FRAME_TEXT);
                } else if (msg[0] == '4' && msg[1] == '2') { // Message (4) + Event (2)
                    std::string json = msg.substr(2);
                    std::lock_guard<std::mutex> lock(_msgMutex);
                    _incomingMessages.push(json);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } catch (std::exception& e) {
        std::cerr << "NetworkManager Thread Error: " << e.what() << std::endl;
    }
}

void NetworkManager::update() {
    std::queue<std::string> localMsgs;
    {
        std::lock_guard<std::mutex> lock(_msgMutex);
        std::swap(localMsgs, _incomingMessages);
    }

    while (!localMsgs.empty()) {
        handleMessage(localMsgs.front());
        localMsgs.pop();
    }
}

void NetworkManager::handleMessage(const std::string& json) {
    try {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();
        
        std::string eventName = arr->get(0).toString();
        Poco::Dynamic::Var data = (arr->size() > 1) ? arr->get(1) : Poco::Dynamic::Var();

        bool handledByCallback = _callbacks.count(eventName) > 0;
        if (handledByCallback) {
            _callbacks[eventName](data);
        }

        if (eventName == "roomList") {
            if (roomListCallback) {
                std::vector<LobbyRoom> rooms;
                Poco::JSON::Array::Ptr roomsArr = data.extract<Poco::JSON::Array::Ptr>();
                for (size_t i = 0; i < roomsArr->size(); ++i) {
                    Poco::JSON::Object::Ptr rObj = roomsArr->getObject(i);
                    std::string gameMode = "marathon";
                    if (rObj->has("gameMode")) gameMode = rObj->getValue<std::string>("gameMode");

                    int startLevel = 1;
                    if (rObj->has("startLevel")) startLevel = rObj->getValue<int>("startLevel");

                    bool isTournament = false;
                    if (rObj->has("isTournament")) isTournament = rObj->getValue<bool>("isTournament");

                    std::string state = "LOBBY";
                    if (rObj->has("state")) state = rObj->getValue<std::string>("state");

                    rooms.push_back({
                        rObj->getValue<std::string>("id"),
                        rObj->getValue<std::string>("name"),
                        rObj->getValue<int>("players"),
                        rObj->getValue<int>("maxPlayers"),
                        rObj->getValue<bool>("hasPassword"),
                        gameMode,
                        startLevel,
                        isTournament,
                        state
                    });
                }
                roomListCallback(rooms);
            }
        } else if (eventName == "garbage" && _game) {
            _game->gotVSGarbageFromOtherPlayer(data.convert<int>());
        } else if (eventName == "opponentFrame" && _opponentGame && handledByCallback == false) {
            Poco::JSON::Object::Ptr stateObj = nullptr;
            if (data.type() == typeid(Poco::JSON::Object::Ptr)) {
                Poco::JSON::Object::Ptr payloadObj = data.extract<Poco::JSON::Object::Ptr>();
                if (payloadObj->has("state")) {
                    Poco::Dynamic::Var stateVar = payloadObj->get("state");
                    if (stateVar.type() == typeid(std::string)) {
                        Poco::JSON::Parser frameParser;
                        stateObj = frameParser.parse(stateVar.convert<std::string>()).extract<Poco::JSON::Object::Ptr>();
                    } else if (stateVar.type() == typeid(Poco::JSON::Object::Ptr)) {
                        stateObj = stateVar.extract<Poco::JSON::Object::Ptr>();
                    }
                } else {
                    // Fallback for older packet versions
                    stateObj = payloadObj;
                }
            } else if (data.type() == typeid(std::string)) {
                // Raw string fallback
                Poco::JSON::Parser frameParser;
                stateObj = frameParser.parse(data.convert<std::string>()).extract<Poco::JSON::Object::Ptr>();
            }

            if (stateObj) {
                _opponentGame->applyState(stateObj);
            }
        }
    } catch (...) {}
}

void NetworkManager::sendEvent(const std::string& name, const Poco::JSON::Object::Ptr& data) {
    if (!_ws) return;
    
    Poco::JSON::Array arr;
    arr.add(name);
    arr.add(data);
    
    std::stringstream ss;
    arr.stringify(ss);
    
    std::string msg = "42" + ss.str(); // 4 (Message) + 2 (Event)
    std::lock_guard<std::mutex> lock(_sendMutex);
    _ws->sendFrame(msg.c_str(), msg.length(), Poco::Net::WebSocket::FRAME_TEXT);
}

void NetworkManager::listRooms() {
    if (!_ws) return;
    std::string msg = "42[\"listRooms\"]";
    _ws->sendFrame(msg.c_str(), msg.length(), Poco::Net::WebSocket::FRAME_TEXT);
}

void NetworkManager::createRoom(const std::string& name, bool isPrivate, const std::string& password, const std::string& gameMode, int startLevel) {
    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
    obj->set("name", name);
    obj->set("isPrivate", isPrivate);
    obj->set("password", password);
    obj->set("gameMode", gameMode);
    obj->set("startLevel", startLevel);
    sendEvent("createRoom", obj);
}

void NetworkManager::joinRoom(const std::string& id, const std::string& password, bool spectator) {
    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
    obj->set("id", id);
    obj->set("password", password);
    if (spectator) obj->set("spectator", true);
    sendEvent("joinRoom", obj);
}

void NetworkManager::sendFrame(const std::string& stateJson) {
    if (!_ws) return;
    std::string msg = "42[\"frame\",\"" + stateJson + "\"]"; // Manual string concat for performance
    _ws->sendFrame(msg.c_str(), msg.length(), Poco::Net::WebSocket::FRAME_TEXT);
}

void NetworkManager::sendChat(const std::string& message, const std::string& name) {
    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
    obj->set("message", message);
    obj->set("name", name);
    sendEvent("chatMessage", obj);
}

void NetworkManager::reportScore(const std::string& mode, const std::string& name, long score, int lines, int time) {
    Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
    obj->set("mode", mode);
    obj->set("name", name);
    obj->set("score", static_cast<long long>(score));
    obj->set("lines", lines);
    obj->set("time", time);
    sendEvent("reportScore", obj);
}

void NetworkManager::setGame(GameLogic* game) { _game = game; }
void NetworkManager::setOpponentGame(GameLogic* opponentGame) { _opponentGame = opponentGame; }
