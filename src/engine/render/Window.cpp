//
// Created by mcumbrella on 2023/11/15.
//

#include "Window.h"
#include "../exception/EngineException.h"

Window::Window(unsigned int f) : flags(f)
{
    if((sdlWindow = SDL_CreateWindow(
        title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        w, h, flags
    )) == nullptr)
    {
        throw EngineException(string("Failed to create SDL window: ") + SDL_GetError());
    }
    update();
}

Window::~Window()
{
    SDL_DestroyWindow(sdlWindow);
}

SDL_Window* Window::getSdlWindow()
{
    return sdlWindow;
}

int Window::getX()
{
    return x;
}

int Window::getY()
{
    return y;
}

void Window::setLocation(const int& x_, const int& y_)
{
    x = x_;
    y = y_;
    SDL_SetWindowPosition(sdlWindow, x, y);
}

int Window::getWidth()
{
    return w;
}

int Window::getHeight()
{
    return h;
}

void Window::setSize(const int& w_, const int& h_)
{
    w = w_;
    h = h_;
    SDL_SetWindowSize(sdlWindow, w, h);
}

string Window::getTitle()
{
    return title;
}

void Window::setTitle(const string& title_)
{
    title = title_;
    SDL_SetWindowTitle(sdlWindow, title.c_str());
}

void Window::update()
{
    SDL_GetWindowPosition(sdlWindow, &x, &y);
    SDL_GetWindowSize(sdlWindow, &w, &h);
}

bool Window::isFullscreen()
{
    return SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN;
}

bool Window::setFullscreen(const bool& state)
{
    return SDL_SetWindowFullscreen(sdlWindow, state);
}

bool Window::isResizable()
{
    return SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_RESIZABLE;
}

bool Window::setResizable(const bool& state)
{
    SDL_SetWindowResizable(sdlWindow, state ? SDL_TRUE : SDL_FALSE);
    // i hate when they define their own true and false while stdbool.h or zero/non-zero is available, get fucked
    return isResizable();
}
