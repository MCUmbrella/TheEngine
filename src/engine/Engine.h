//
// Created by mcumbrella on 23-10-16.
//

#ifndef THEENGINE_ENGINE_H
#define THEENGINE_ENGINE_H

#include "EngineState.h"
#include <string>
#include <SDL2/SDL_scancode.h>

using std::string;

class Engine
{
private:
    Engine() = default;

    ~Engine() = default;

    static void pollSDLEvents();

    static void mainLoop();

public:
    Engine(const Engine&) = delete;

    Engine& operator=(const Engine&) = delete;

    const static Engine& getInstance();

    /**
     * Initialize the Engine and related components.
     * This function can be called only once and must be called
     * before calling any other functions.
     */
    static void init(const string& configPath);

    /**
     * Start the Engine.
     */
    static void start();

    /**
     * Exit the main loop, release all resources and related components.
     */
    static void stop();

    /**
     * Get the state of the Engine.
     * @sa EngineState.h
     */
    const static EngineState& getState();

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
    static bool keyPressed(int code);

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
    static bool keyHolding(int code);

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
    static bool keyRepeated(int code);

    /**
     * Get the count of ticks passed.
     */
    static unsigned long currentTick();
};

#endif //THEENGINE_ENGINE_H
