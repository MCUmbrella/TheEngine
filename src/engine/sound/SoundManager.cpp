//
// Created by mcumbrella on 2023/11/7.
//

#include "SoundManager.h"
#include "../util/Logger.h"
#include "../exception/EngineException.h"
#include "Music.h"
#include <SDL2/SDL_mixer.h>
#include <list>
#include <unordered_map>

using std::list;
using std::unordered_map;

static unordered_map<string /*file path*/, Mix_Chunk*> mixChunks;
static unordered_map<string, Mix_Music*> mixMusics;
static unordered_map<string /*name*/, Sound> sounds;
static unordered_map<string, Music> musics;

const SoundManager& SoundManager::getInstance()
{
    static SoundManager THE_SOUND_MANAGER;
    return THE_SOUND_MANAGER;
}


/*================================================
 *                    MANAGER
 ================================================*/


void SoundManager::init()
{
    logInfo << "Initializing sound manager";
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        throw EngineException(string("SDL Mixer initialization failed: ") + SDL_GetError());
    Mix_AllocateChannels(64);
    logInfo << "Sound manager initialization completed";
}

void SoundManager::shutdown()
{
    logInfo << "Shutting down sound manager";
    Mix_Quit();
    logInfo << "Sound manager shutted down";
}

void SoundManager::gc()
{
    logInfo << "Performing GC... Loaded sounds: " << mixChunks.size() << ", loaded musics: " << mixMusics.size();
    erase_if(mixChunks, [&](auto& p) -> bool{
        int uses = 0;
        for(auto& sound : sounds)
            if(sound.second.mixChunk == p.second)
                ++uses;
        if(uses == 0)
        {
            logInfo << "Unloaded sound \"" << p.first << "\" (&: " << p.second << ")";
            if(p.second != nullptr)
                Mix_FreeChunk(p.second);
            return true;
        }
        return false;
    });
    erase_if(mixMusics, [&](auto& p) -> bool{
        int uses = 0;
        for(auto& music : musics)
            if(music.second.mixMusic == p.second)
                ++uses;
        if(uses == 0)
        {
            logInfo << "Unloaded music \"" << p.first << "\" (&: " << p.second << ")";
            if(p.second != nullptr)
                Mix_FreeMusic(p.second);
            return true;
        }
        return false;
    });
    logInfo << "GC completed. Remaining loaded sounds: " << mixChunks.size() << ", loaded musics: " << mixMusics.size();
}


/*================================================
 *                     SOUND
 ================================================*/


Mix_Chunk* SoundManager::loadMixChunk(const string& path)
{
    if(hasMixChunk(path))
        return mixChunks[path];
    logInfo << "Loading sound file: " << path;
    Mix_Chunk* p = Mix_LoadWAV(path.c_str());
    if(p == nullptr)
        logError << "Failed to load sound file \"" << path << "\": " << SDL_GetError();
    mixChunks.emplace(path, p);
    return p;
}

Mix_Chunk* SoundManager::getMixChunk(const string& path)
{
    if(hasMixChunk(path))
        return mixChunks[path];
    throw EngineException("Sound file not loaded: " + path);
}

void SoundManager::unloadMixChunk(const string& path)
{
    logInfo << "Unloading sound file: " << path;
    if(hasMixChunk(path))
    {
        if(mixChunks[path] != nullptr)
            Mix_FreeChunk(mixChunks[path]);
        mixChunks.erase(path);
    }
    throw EngineException("Sound file not loaded: " + path);
}

bool SoundManager::hasMixChunk(const string& path)
{
    return mixChunks.contains(path);
}

Sound* SoundManager::addSound(const string& name, const string& path)
{
    if(hasSound(name)) // the sound with this name already exists
        throw EngineException(sounds.at(name).toString() + " already exists");
    else // create
    {
        auto rp = sounds.emplace(name, Sound(name, path));
        auto& ep = rp.first;
        Sound& s = ep->second;
        logInfo << "Added " << s.toString();
        return &s;
    }
}

Sound* SoundManager::getSound(const string& name)
{
    if(!hasSound(name))
        throw EngineException("Sound not found: " + name);
    return &(sounds.at(name));
}

void SoundManager::removeSound(const string& name)
{
    logInfo << "Removing sound: " << name;
    if(!hasSound(name))
        throw EngineException("Sound not found: " + name);
    sounds.erase(name);
}

bool SoundManager::hasSound(const string& name)
{
    return sounds.contains(name);
}

PlayingSound SoundManager::playSound(const Sound* sound)
{
    return sound->mixChunk == nullptr ?
           PlayingSound(nullptr, -1) :
           PlayingSound(sound->mixChunk, Mix_PlayChannel(-1, sound->mixChunk, false));
}

PlayingSound SoundManager::playSound(const string& name)
{
    return playSound(getSound(name));
}


/*================================================
 *                     MUSIC
 ================================================*/


Mix_Music* SoundManager::loadMixMusic(const string& path)
{
    if(hasMixMusic(path))
        return mixMusics[path];
    logInfo << "Loading music file: " << path;
    Mix_Music* p = Mix_LoadMUS(path.c_str());
    if(p == nullptr)
        logError << "Failed to load music file \"" << path << "\": " << SDL_GetError();
    mixMusics.emplace(path, p);
    return p;
}

Mix_Music* SoundManager::getMixMusic(const string& path)
{
    if(hasMixMusic(path))
        return mixMusics[path];
    throw EngineException("Music file not loaded: " + path);
}

void SoundManager::unloadMixMusic(const string& path)
{
    logInfo << "Unloading music file: " << path;
    if(hasMixMusic(path))
    {
        if(mixMusics[path] != nullptr)
            Mix_FreeMusic(mixMusics[path]);
        mixMusics.erase(path);
    }
    throw EngineException("Music file not loaded: " + path);
}

bool SoundManager::hasMixMusic(const string& path)
{
    return mixMusics.contains(path);
}

Music* SoundManager::addMusic(const string& name, const string& path)
{
    if(hasMusic(name))
        throw EngineException(musics.at(name).toString() + " already exists");
    else
    {
        auto rp = musics.emplace(name, Music(name, path));
        auto& ep = rp.first;
        Music& m = ep->second;
        logInfo << "Added " << m.toString();
        return &m;
    }
}

Music* SoundManager::getMusic(const string& name)
{
    if(!hasMusic(name))
        throw EngineException("Music not found: " + name);
    return &(musics.at(name));
}

void SoundManager::removeMusic(const string& name)
{
    logInfo << "Removing music: " << name;
    if(!hasMusic(name))
        throw EngineException("Music not found: " + name);
    musics.erase(name);
}

bool SoundManager::hasMusic(const string& name)
{
    return musics.contains(name);
}

void SoundManager::playMusic(const Music* music)
{
    if(music->mixMusic != nullptr)
        Mix_PlayMusic(music->mixMusic, -1);
}

void SoundManager::playMusic(const string& name)
{
    playMusic(getMusic(name));
}

void SoundManager::pauseMusic()
{
    Mix_PauseMusic();
}

void SoundManager::resumeMusic()
{
    Mix_ResumeMusic();
}

void SoundManager::stopMusic()
{
    Mix_HaltMusic();
}

int SoundManager::getMusicVolume()
{
    return Mix_VolumeMusic(-1);
}

int SoundManager::setMusicVolume(int vol)
{
    return Mix_VolumeMusic(vol);
}
