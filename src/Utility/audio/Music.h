//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------

#pragma once
#include "bobtypes.h"
#include "src/Engine/EnginePart.h"

class Logger;

class Music : public EnginePart
{
public:
    static Logger log;

    Music(Engine* g, const string& filename);
    virtual ~Music();

    void play(int loops = -1, int fadeInMs = 0);
    void stop(int fadeOutMs = 0);
    void pause();
    void resume();

    void setVolume(float volume); // 0.0f to 1.0f
    float getVolume();

    bool isPlaying();
    bool isPaused();

    string getName();

private:
    string filename;
    Mix_Music* music = nullptr;
    float volume = 1.0f;
};
