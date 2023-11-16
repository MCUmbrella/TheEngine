//
// Created by mcumbrella on 2023/11/16.
//

#include "Font.h"
#include "../exception/EngineException.h"
#include "../exception/IllegalArgumentException.h"

Font::Font(const string& name, const string& path) : name(name), path(path)
{
    loadSdlFont(32);
}

Font::~Font()
{
    for(auto& p : sdlFont)
        TTF_CloseFont(p.second);
}

TTF_Font* Font::loadSdlFont(const int& pt)
{
    if(pt < 1)
        throw IllegalArgumentException("Parameter 'pt' in Font.loadSdlFont(pt) must be positive");
    TTF_Font* f = TTF_OpenFont(path.c_str(), pt);
    if(f == nullptr)
        throw EngineException(
            string("Failed to load font file \"") + path +
            "\" with size " + std::to_string(pt) + ": " + SDL_GetError()
        );
    sdlFont.emplace(pt, f);
    return f;
}

TTF_Font* Font::getSdlFont(const int& pt)
{
    return sdlFont.contains(pt)
           ? sdlFont.at(pt)
           : loadSdlFont(pt);
}

string Font::toString()
{
    return "Font \"" + name + "\" (file: " + path + ")";
}
