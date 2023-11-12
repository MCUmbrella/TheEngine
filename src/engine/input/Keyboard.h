//
// Created by mcumbrella on 2023/11/12.
//

#ifndef THEENGINE_KEYBOARD_H
#define THEENGINE_KEYBOARD_H

#include <bitset>

using std::bitset;

class Keyboard
{
public:
    bitset<1024> keysP{}; // pressed keys at a single tick (calculated at the end of pollSDLEvents)
    bitset<1024> keysH[2]{}; // currently holding keys. [0]: down, [1]: repeat
    bitset<1024> keysH_p[2]{}; // keysH at previous tick

    /**
     * Check if a key is holding.
     * @param code The SDL scancode of the key.
     * @return true if the key is holding, false otherwise.
     * @sa keyPressed(int)
     *
     * @code Example:
     *   SEQ \ VAL
     *   KEY | 01111111100111111110
     *   FUN | 01111111100111111110
     */
    bool holding(const int& scancode);

    /**
     * Check if a key is pressed.
     * @param code The SDL scancode of the key.
     * @return true if the key is pressed, false otherwise.
     * @sa SDL2/SDL_scancode.h or <a href="https://www.freepascal-meets-sdl.net/sdl-2-0-scancode-lookup-table/">SDL scancode lookup table</a>
     *
     * @code Example:
     *   SEQ \ VAL
     *   KEY | 01111111100111111110
     *   FUN | 01000000000100000000
     */
    bool pressed(const int& scancode);

    /**
     * Check if a key is repeating.
     * @param code The SDL scancode of the key.
     * @return true if the key is repeating, false otherwise.
     * @sa keyPressed(int)
     *
     * @code Example:
     *   SEQ \ VAL
     *   KEY | 01111111100111111110
     *   FUN | 00001111100000111110
     */
    bool repeated(const int& scancode);
};

#endif //THEENGINE_KEYBOARD_H
