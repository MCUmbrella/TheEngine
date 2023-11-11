//
// Created by mcumbrella on 2023/11/11.
//

#include "Mouse.h"
#include <SDL2/SDL.h>

int Mouse::getX()
{
    return x;
}

int Mouse::getY()
{
    return y;
}

bool Mouse::lHolding()
{
    return buttonsH[0];
}

bool Mouse::mHolding()
{
    return buttonsH[1];
}

bool Mouse::rHolding()
{
    return buttonsH[2];
}

bool Mouse::x1Holding()
{
    return buttonsH[3];
}

bool Mouse::x2Holding()
{
    return buttonsH[4];
}

bool Mouse::lPressed()
{
    return buttonsP[0];
}

bool Mouse::mPressed()
{
    return buttonsP[1];
}

bool Mouse::rPressed()
{
    return buttonsP[2];
}

bool Mouse::x1Pressed()
{
    return buttonsP[3];
}

bool Mouse::x2Pressed()
{
    return buttonsP[4];
}

bool Mouse::wheelUp()
{
    return wheel[0];
}

bool Mouse::wheelDown()
{
    return wheel[1];
}

bool Mouse::wheelLeft()
{
    return wheel[2];
}

bool Mouse::wheelRight()
{
    return wheel[3];
}

bool Mouse::isHidden()
{
    return hidden;
}

bool Mouse::setHidden(const bool& state)
{
    hidden = state;
    return SDL_ShowCursor(!state);
}
