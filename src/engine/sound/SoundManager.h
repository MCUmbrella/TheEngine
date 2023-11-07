//
// Created by mcumbrella on 23-10-16.
//

#ifndef THEENGINE_SOUNDMANAGER_H
#define THEENGINE_SOUNDMANAGER_H

#include <string>
#include <SDL2/SDL_mixer.h>
#include "Sound.h"

using std::string;

class SoundManager
{
private:
    SoundManager() = default;

    ~SoundManager() = default;
public:
    SoundManager(const SoundManager&) = delete;

    SoundManager& operator=(const SoundManager&) = delete;

    const static SoundManager& getInstance();

    static void init();

    static void shutdown();

    static Mix_Chunk* loadMixChunk(const string& path);

    static Mix_Chunk* getMixChunk(const string& path);

    static void unloadMixChunk(const string& path);

    static bool hasMixChunk(const string& path);

    static Sound* loadSound(const string& name, const string& path);

    static Sound* getSound(const string& name);

    static void unloadSound(const string& name);

    static bool hasSound(const string& name);

    static void playSound(const string& name);

    static void gc();
};

#endif //THEENGINE_SOUNDMANAGER_H
