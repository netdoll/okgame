#include "stdafx.h"
#include "SteamManager.h"

void SteamManager::init() {
#ifdef HAVE_STEAMWORKS
    // Replace with your real App ID if you have one, or use a steam_appid.txt file
    // if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) { 
    //     exit(1);
    // }

    if (!SteamAPI_Init()) {
        Main::log.error("SteamAPI_Init() failed");
        return;
    }
    Main::log.info("Steam initialized successfully");
#else
    //Main::log.info("Steam support not compiled in");
#endif
}

void SteamManager::update() {
#ifdef HAVE_STEAMWORKS
    SteamAPI_RunCallbacks();
#endif
}

void SteamManager::shutdown() {
#ifdef HAVE_STEAMWORKS
    SteamAPI_Shutdown();
#endif
}

void SteamManager::setAchievement(const string& id) {
#ifdef HAVE_STEAMWORKS
    SteamUserStats()->SetAchievement(id.c_str());
    SteamUserStats()->StoreStats();
#endif
}

void SteamManager::setStat(const string& id, int value) {
#ifdef HAVE_STEAMWORKS
    SteamUserStats()->SetStat(id.c_str(), value);
    SteamUserStats()->StoreStats();
#endif
}

void SteamManager::setStat(const string& id, float value) {
#ifdef HAVE_STEAMWORKS
    SteamUserStats()->SetStat(id.c_str(), value);
    SteamUserStats()->StoreStats();
#endif
}

bool SteamManager::isSteamRunning() {
#ifdef HAVE_STEAMWORKS
    return SteamAPI_IsSteamRunning();
#else
    return false;
#endif
}

uint64_t SteamManager::getSteamID() {
#ifdef HAVE_STEAMWORKS
    if (SteamUser()) {
        return SteamUser()->GetSteamID().ConvertToUint64();
    }
#endif
    return 0;
}

string SteamManager::getPersonaName() {
#ifdef HAVE_STEAMWORKS
    if (SteamFriends()) {
        return string(SteamFriends()->GetPersonaName());
    }
#endif
    return "Player";
}
