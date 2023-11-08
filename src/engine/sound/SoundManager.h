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

    /*================================================
     *                    MANAGER
     ================================================*/

    /**
     * Initialize the sound manager.
     * Should be called only once.
     */
    static void init();

    /**
     * Shut down the sound manager.
     * Should be called only once and after calling init()
     */
    static void shutdown();

    /**
     * Unload all unused SDL Mixer resources.
     * If a loaded resource has no Sound or Music object
     * with pointer pointed to it, then it's considered
     * unused and will be unloaded.
     */
    static void gc();

    /*================================================
     *                     SOUND
     ================================================*/

    /**
     * Load a sound resource from file.
     * NOTICE: whether the file is successfully loaded or
     * not, its path will always be added to the list.
     * @param path The path of the file.
     * @return The pointer to the loaded resource's Mix_Chunk
     * object, or nullptr if SDL has failed to load it.
     */
    static Mix_Chunk* loadMixChunk(const string& path);

    /**
     * Get a loaded sound resource by its path.
     * @param path The path of the loaded sound file.
     * @return The pointer to the sound's Mix_Chunk object.
     * @throw EngineException if the file is not loaded.
     */
    static Mix_Chunk* getMixChunk(const string& path);

    /**
     * Unloaded a loaded sound resource by its path.
     * @param path The path of the loaded sound file.
     * @throw EngineException if the file is not loaded.
     */
    static void unloadMixChunk(const string& path);

    /**
     * Check if a sound file is loaded.
     * @param path The path of the file.
     */
    static bool hasMixChunk(const string& path);

    /**
     * Register a Sound object.
     * @param name The name of the sound.
     * @param path The path of the file used by the sound.
     * @return The pointer to the newly registered sound.
     * @throw EngineException if a sound with the specified
     * name already exists.
     */
    static Sound* addSound(const string& name, const string& path);

    /**
     * Get a Sound object by its name.
     * @throw EngineException if the sound with the specified
     * name was not found.
     */
    static Sound* getSound(const string& name);

    /**
     * Unregister a Sound object by its name.
     * @throw EngineException if the sound with the specified
     * name was not found.
     */
    static void removeSound(const string& name);

    /**
     * Check if a sound object with the specified name exists.
     */
    static bool hasSound(const string& name);

    /**
     * Play a Sound object.
     * If the internal pointer of the sound is nullptr, this
     * function will not do anything.
     * @param sound The pointer to the Sound object.
     */
    static void playSound(const Sound* sound);

    /**
     * Play a sound by its name.
     * @throw EngineException if the sound with the specified
     * name was not found.
     */
    static void playSound(const string& name);

    /*================================================
     *                     MUSIC
     ================================================*/
};

#endif //THEENGINE_SOUNDMANAGER_H
