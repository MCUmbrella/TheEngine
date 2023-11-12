//
// Created by mcumbrella on 2023/11/12.
//

#include "Keyboard.h"

bool Keyboard::holding(const int& scancode)
{
    return keysH[0][scancode];
}

bool Keyboard::pressed(const int& scancode)
{
    return keysP[scancode];
}

bool Keyboard::repeated(const int& scancode)
{
    return keysH[1][scancode];
}
