//
// Created by mcumbrella on 2023/11/7.
//

#ifndef THEENGINE_SOUND_H
#define THEENGINE_SOUND_H

#include <string>
#include <SDL2/SDL_mixer.h>
#include "PlayingSound.h"

using std::string;

/**
 * The wrapper class for SDL sound struct (Mix_Chunk).
 */
class Sound
{
public:
    const string name;
    string path;
    Mix_Chunk* mixChunk;

    Sound(const string& name, const string& path);

    /**
     * Get the name of the sound.
     */
    string getName();

    /**
     * Get the path of the file assigned to this sound.
     */
    string getPath();

    /**
     * Assign another loaded sound file to this Sound object.
     * @param newPath The path of the new sound file. If the file with this path
     * is not loaded, SoundManager.loadMixChunk() will be called automatically.
     */
    Sound* reassign(const string& newPath);

    /**
     * Play the sound.
     * @return A PlayingSound object.
     * @sa PlayingSound.h
     */
    PlayingSound play();

    /**
     * Get a string that contains some information about this Sound object.
     */
    string toString();
};


#endif //THEENGINE_SOUND_H
