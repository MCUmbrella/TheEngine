//
// Created by mcumbrella on 2023/11/9.
//

#ifndef THEENGINE_PLAYINGSOUND_H
#define THEENGINE_PLAYINGSOUND_H

#include <SDL2/SDL_mixer.h>

/**
 * Represents a currently playing sound, produced by Sound.play()
 * or SoundManager.playSound(). When the sound has been played to
 * end or has been stopped, the object is not used anymore.
 */
class PlayingSound
{
public:
    // the pointer to the Mix_Chunk that the sound uses.
    Mix_Chunk* mixChunk;
    // the ID of the SDL Mixer channel that the sound is playing on.
    // (SDL Mixer plays each Mix_Chunk on a separate channel)
    const int channelId;

    PlayingSound(Mix_Chunk* mixChunk, const int& channelId);

    /**
     * Check if this sound hasn't been played to the end or hasn't
     * been stopped.
     */
    bool isValid();

    /**
     * Pause the playing sound.
     */
    void pause();

    /**
     * Resume the paused sound.
     */
    void resume();

    /**
     * Stop the sound. After calling this function, call to other
     * functions is meaningless.
     */
    void stop();

    /**
     * Get the volume of the playing sound.
     * @return Integer from 0 (silent) to 128 (max volume).
     */
    int getVolume();

    /**
     * Set the volume of the playing sound.
     * @param vol 0 ~ 128.
     * @return The new volume.
     */
    int setVolume(const int& vol);
};

#endif //THEENGINE_PLAYINGSOUND_H
