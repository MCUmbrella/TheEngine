//
// Created by mcumbrella on 2023/11/7.
//

#include "Sound.h"
#include "SoundManager.h"

Sound::Sound(const string& name, const string& path) : name(name), path(path)
{
    mixChunk = SoundManager::hasMixChunk(path) ? SoundManager::getMixChunk(path)
                                               : SoundManager::loadMixChunk(path);
}

string Sound::getName()
{
    return name;
}

string Sound::getPath()
{
    return path;
}

Sound* Sound::reassign(const string& newPath)
{
    path = newPath;
    mixChunk = SoundManager::hasMixChunk(path) ? SoundManager::getMixChunk(path)
                                               : SoundManager::loadMixChunk(path);
    return this;
}

PlayingSound Sound::play()
{
    return SoundManager::playSound(this);
}

string Sound::toString()
{
    char a[32]{};
    sprintf(a, "%p", mixChunk);
    return "Sound \"" + name + "\" (file=\"" + path + "\", &=" + a + ")";
}
