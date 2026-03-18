#include "steam_api.h"
#include <iostream>

class MockSteamUser : public ISteamUser {
public:
    CSteamID GetSteamID() override {
        return CSteamID(1234567890);
    }
};

class MockSteamFriends : public ISteamFriends {
public:
    const char *GetPersonaName() override {
        return "LocalPlayer";
    }
};

class MockSteamUserStats : public ISteamUserStats {
public:
    bool SetAchievement(const char *pchName) override {
        std::cout << "MockSteam: Achievement Unlocked: " << pchName << std::endl;
        return true;
    }
    bool SetStat(const char *pchName, int32_t nData) override {
        std::cout << "MockSteam: Stat " << pchName << " set to " << nData << std::endl;
        return true;
    }
    bool SetStat(const char *pchName, float fData) override {
        std::cout << "MockSteam: Stat " << pchName << " set to " << fData << std::endl;
        return true;
    }
    bool StoreStats() override {
        return true;
    }
};

static MockSteamUser g_MockUser;
static MockSteamFriends g_MockFriends;
static MockSteamUserStats g_MockUserStats;
static bool g_bIsRunning = false;

ISteamUser *SteamUser() { return &g_MockUser; }
ISteamFriends *SteamFriends() { return &g_MockFriends; }
ISteamUserStats *SteamUserStats() { return &g_MockUserStats; }

bool SteamAPI_Init() {
    g_bIsRunning = true;
    return true;
}

void SteamAPI_Shutdown() {
    g_bIsRunning = false;
}

void SteamAPI_RunCallbacks() {
    // No-op for mock
}

bool SteamAPI_IsSteamRunning() {
    return g_bIsRunning;
}
