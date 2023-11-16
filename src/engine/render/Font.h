//
// Created by mcumbrella on 2023/11/16.
//

#ifndef THEENGINE_FONT_H
#define THEENGINE_FONT_H

#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>

using std::string;

class Font
{
private:
    const string name, path;
    std::unordered_map<int, TTF_Font*> sdlFont;

    TTF_Font* loadSdlFont(const int& pt);

public:
    Font(const Font&) = delete;

    Font& operator=(const Font&) = delete;

    explicit Font(const string& name, const string& path);

    ~Font();

    TTF_Font* getSdlFont(const int& pt);

    string toString();
};

#endif //THEENGINE_FONT_H
