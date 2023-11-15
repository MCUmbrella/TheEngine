//
// Created by mcumbrella on 2023/11/15.
//

#ifndef THEENGINE_WINDOW_H
#define THEENGINE_WINDOW_H

#include <SDL2/SDL.h>
#include <string>

using std::string;

class Window
{
private:
    SDL_Window* sdlWindow = nullptr;
    int x = 0;
    int y = 0;
    int w = 640;
    int h = 480;
    unsigned int flags = 0;
    string title = "The Engine";
public:
    explicit Window(unsigned int flags = 0);

    ~Window();

    SDL_Window* getSdlWindow();

    int getX();

    int getY();

    void setLocation(const int& x_, const int& y_);

    int getWidth();

    int getHeight();

    void setSize(const int& w_, const int& h_);

    string getTitle();

    void setTitle(const string& title_);
};

#endif //THEENGINE_WINDOW_H
