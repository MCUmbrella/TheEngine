//
// Created by mcumbrella on 2023/11/7.
//

#include "Music.h"
#include "SoundManager.h"

static Music* currentMusic = nullptr;
static bool playing = false;

Music::Music(const string& name, const string& path) : name(name), path(path)
{
    mixMusic = SoundManager::hasMixMusic(path) ? SoundManager::getMixMusic(path)
                                               : SoundManager::loadMixMusic(path);
}

string Music::getName()
{
    return name;
}

string Music::getPath()
{
    return path;
}

Music* Music::reassign(const string& newPath)
{
    bool isCurrentPlaying = isCurrent() && isPlaying();
    if(isCurrentPlaying)
        stop();
    path = newPath;
    mixMusic = SoundManager::hasMixMusic(path) ? SoundManager::getMixMusic(path)
                                               : SoundManager::loadMixMusic(path);
    if(isCurrentPlaying)
        play();
    return this;
}

bool Music::isCurrent()
{
    return currentMusic == this;
}

void Music::play()
{
    if(mixMusic != nullptr)
        Mix_PlayMusic(mixMusic, -1);
    else
        Mix_HaltMusic();
    currentMusic = this;
    playing = true;
}

bool Music::isPlaying()
{
    return playing;
}

void Music::pause()
{
    if(currentMusic != nullptr)
    {
        Mix_PauseMusic();
        playing = false;
    }

}

void Music::resume()
{
    if(currentMusic != nullptr)
    {
        Mix_ResumeMusic();
        playing = true;
    }
}

void Music::stop()
{
    Mix_HaltMusic();
    currentMusic = nullptr;
    playing = false;
}

int Music::getVolume()
{
    return Mix_VolumeMusic(-1);
}

int Music::setVolume(const int& vol)
{
    return Mix_VolumeMusic(vol);
}

string Music::toString()
{
    char a[32]{};
    sprintf(a, "%p", mixMusic);
    return "Music \"" + name + "\" (file: \"" + path + "\", &: " + a + ")";
}
