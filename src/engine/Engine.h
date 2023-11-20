//
// Created by mcumbrella on 23-10-16.
//

#ifndef THEENGINE_ENGINE_H
#define THEENGINE_ENGINE_H

#include "EngineState.h"
#include "input/Mouse.h"
#include "input/Keyboard.h"
#include <string>

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

    /**
     * Initialize the Engine and related components.
     * This function can be called only once and must be called
     * before calling any other functions.
     */
    static void init(const string& configPath);

    /**
     * Start the Engine.
     */
    static void run();

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
     * Get the count of ticks passed.
     */
    static unsigned long currentTick();

    /**
     * Get the representation of the mouse.
     */
    static Mouse& getMouse();

    /**
     * Get the representation of the keyboard.
     */
    static Keyboard& getKeyboard();
};

#endif //THEENGINE_ENGINE_H
