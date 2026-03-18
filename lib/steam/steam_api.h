#ifndef STEAM_API_H
#define STEAM_API_H

#include <stdint.h>
#include <string>

// Basic types
typedef uint64_t uint64;
typedef uint32_t uint32;

class CSteamID {
public:
    CSteamID() : m_steamid(0) {}
    CSteamID(uint64 id) : m_steamid(id) {}
    uint64 ConvertToUint64() const { return m_steamid; }
private:
    uint64 m_steamid;
};

// ISteamUser
class ISteamUser {
public:
    virtual CSteamID GetSteamID() = 0;
};

// ISteamFriends
class ISteamFriends {
public:
    virtual const char *GetPersonaName() = 0;
};

// ISteamUserStats
class ISteamUserStats {
public:
    virtual bool SetAchievement(const char *pchName) = 0;
    virtual bool SetStat(const char *pchName, int32_t nData) = 0;
    virtual bool SetStat(const char *pchName, float fData) = 0;
    virtual bool StoreStats() = 0;
};

// Global accessors
ISteamUser *SteamUser();
ISteamFriends *SteamFriends();
ISteamUserStats *SteamUserStats();

// Core API
bool SteamAPI_Init();
void SteamAPI_Shutdown();
void SteamAPI_RunCallbacks();
bool SteamAPI_IsSteamRunning();

#endif // STEAM_API_H
