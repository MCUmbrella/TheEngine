//
// Created by mcumbrella on 2023/11/7.
//

#ifndef THEENGINE_MUSIC_H
#define THEENGINE_MUSIC_H

#include <string>
#include <SDL2/SDL_mixer.h>

using std::string;

/**
 * The wrapper class for SDL music struct (Mix_Music).
 * Has the same structure as the Sound class.
 * @sa Sound.h
 */
class Music
{
public:
    const string name;
    string path;
    Mix_Music* mixMusic;

    Music(const string& name, const string& path);

    string getName();

    string getPath();

    Music* reassign(const string& newPath);

    void play();

    void pause();

    void resume();

    void stop();

    string toString();
};


#endif //THEENGINE_MUSIC_H
