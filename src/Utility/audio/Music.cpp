#include "stdafx.h"
#include "Music.h"
#include <SDL_mixer.h>

Logger Music::log = Logger("Music");

Music::Music(Engine* g, const string& filename) : EnginePart(g)
{
    this->filename = filename;
    string path = Main::getPath() + filename;
    music = Mix_LoadMUS(path.c_str());
    if (!music)
    {
        log.error("Failed to load music: " + filename + " Error: " + string(Mix_GetError()));
    }
}

Music::~Music()
{
    if (music)
    {
        Mix_FreeMusic(music);
        music = nullptr;
    }
}

void Music::play(int loops, int fadeInMs)
{
    if (!music) return;

    if (fadeInMs > 0)
    {
        if (Mix_FadeInMusic(music, loops, fadeInMs) == -1)
        {
            log.error("Mix_FadeInMusic: " + string(Mix_GetError()));
        }
    }
    else
    {
        if (Mix_PlayMusic(music, loops) == -1)
        {
            log.error("Mix_PlayMusic: " + string(Mix_GetError()));
        }
    }
}

void Music::stop(int fadeOutMs)
{
    if (fadeOutMs > 0)
    {
        Mix_FadeOutMusic(fadeOutMs);
    }
    else
    {
        Mix_HaltMusic();
    }
}

void Music::pause()
{
    Mix_PauseMusic();
}

void Music::resume()
{
    Mix_ResumeMusic();
}

void Music::setVolume(float v)
{
    this->volume = v;
    Mix_VolumeMusic((int)(v * MIX_MAX_VOLUME));
}

float Music::getVolume()
{
    return volume;
}

bool Music::isPlaying()
{
    return Mix_PlayingMusic();
}

bool Music::isPaused()
{
    return Mix_PausedMusic();
}

string Music::getName()
{
    return filename;
}
