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
    logInfo << "Performing GC";
    logInfo << "Loaded sounds: " << mixChunks.size();
    erase_if(mixChunks, [&](auto& p) -> bool{
        int uses = 0;
        for(auto& sound : sounds)
            if(sound.second.mixChunk == p.second)
                ++uses;
        if(uses == 0)
        {
            logInfo << "Remove \"" << p.first << "\" (&: " << p.second << ")";
            if(p.second != nullptr)
                Mix_FreeChunk(p.second);
            return true;
        }
        return false;
    });
    logInfo << "GC completed";
    logInfo << "Loaded sounds: " << mixChunks.size();
}

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

Sound* SoundManager::loadSound(const string& name, const string& path)
{
    if(hasSound(name)) // the sound with this name already exists
    {
        if(sounds.at(name).path == path) // same path, return directly
            return &(sounds.at(name));
        else // different path, reassign the file and return
            return sounds.at(name).reassign(path);
    }
    else // create
        return &(sounds.emplace(name, Sound(name, path)).first->second);
}

Sound* SoundManager::getSound(const string& name)
{
    if(!hasSound(name))
        throw EngineException("Sound not found: " + name);
    return &(sounds.at(name));
}

void SoundManager::unloadSound(const string& name)
{
    if(!hasSound(name))
        throw EngineException("Sound not found: " + name);
    sounds.erase(name);
}

bool SoundManager::hasSound(const string& name)
{
    return sounds.contains(name);
}

void SoundManager::playSound(const string& name)
{
    auto sound = getSound(name);
    if(sound->mixChunk == nullptr)
        return;
    Mix_PlayChannel(-1, sound->mixChunk, false);
}
