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
 * Has the almost same structure as the combination of Sound and PlayingSound class.
 * <br>
 * NOTICE:
 * According to SDL documentation, only one music object can be played at a time. If play() is
 * called while another Music is playing, it will be replaced. All music object shares one volume.
 * @sa Sound.h, PlayingSound.h
 */
class Music
{
public:
    const string name;
    string path;
    Mix_Music* mixMusic;

    Music(const string& name, const string& path);

    /**
     * Get the name of the music.
     */
    string getName();

    /**
     * Get the path of the file assigned to the music.
     */
    string getPath();

    /**
     * Assign another loaded music file to this Music object.
     * @param newPath The path of the new music file. If the file with this path
     * is not loaded, SoundManager.loadMixMusic() will be called automatically.
     */
    Music* reassign(const string& newPath);

    /**
     * Check if the current music is being used.
     */
    bool isCurrent();

    /**
     * Play the music.
     */
    void play();

    /**
     * Check if there's any music playing.
     */
    static bool isPlaying();

    /**
     * Pause the playing music (if any).
     */
    static void pause();

    /**
     * Resume the paused music (if any).
     */
    static void resume();

    /**
     * Stop the music.
     */
    static void stop();

    /**
     * Get the volume of the music.
     * @return Integer from 0 (silent) to 128 (max volume).
     */
    static int getVolume();

    /**
     * Set the volume of the music.
     * @param vol 0 ~ 128.
     * @return The new volume.
     */
    static int setVolume(const int& vol);

    string toString();
};

#endif //THEENGINE_MUSIC_H
