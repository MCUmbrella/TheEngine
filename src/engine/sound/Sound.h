//
// Created by mcumbrella on 2023/11/7.
//

#ifndef THEENGINE_SOUND_H
#define THEENGINE_SOUND_H

#include <string>
#include <SDL2/SDL_mixer.h>

using std::string;

class Sound
{
public:
    const string& name;
    string path;
    Mix_Chunk* mixChunk;

    Sound(const string& name, const string& path);

    Sound* reassign(const string& newPath);
};


#endif //THEENGINE_SOUND_H
