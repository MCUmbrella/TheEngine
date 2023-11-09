//
// Created by mcumbrella on 2023/11/7.
//

#include "Music.h"
#include "SoundManager.h"

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
    path = newPath;
    mixMusic = SoundManager::hasMixMusic(path) ? SoundManager::getMixMusic(path)
                                               : SoundManager::loadMixMusic(path);
    return this;
}

void Music::play()
{
    SoundManager::playMusic(this);
}

void Music::pause()
{
    SoundManager::pauseMusic();
}

void Music::resume()
{
    SoundManager::resumeMusic();
}

void Music::stop()
{
    SoundManager::stopMusic();
}

string Music::toString()
{
    char a[32]{};
    sprintf(a, "%p", mixMusic);
    return "Music \"" + name + "\" (file=\"" + path + "\", &=" + a + ")";
}
