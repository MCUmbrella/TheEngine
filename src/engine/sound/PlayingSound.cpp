//
// Created by mcumbrella on 2023/11/9.
//

#include "PlayingSound.h"

PlayingSound::PlayingSound(Mix_Chunk* mixChunk, const int& channelId) : mixChunk(mixChunk), channelId(channelId)
{}

void PlayingSound::pause()
{
    if(isValid())
        Mix_Pause(channelId);
}

void PlayingSound::resume()
{
    if(isValid())
        Mix_Resume(channelId);
}

void PlayingSound::stop()
{
    if(isValid())
    {
        Mix_HaltChannel(channelId);
        mixChunk = nullptr;
    }
}

bool PlayingSound::isValid()
{
    return mixChunk != nullptr && Mix_GetChunk(channelId) == mixChunk;
}
