//
// Created by mcumbrella on 23-10-18.
//

#ifndef THEENGINE_ENGINESTATE_H
#define THEENGINE_ENGINESTATE_H

/**
 * Represents the state of the Engine.
 */
enum EngineState
{
    /** The Engine is not ready, call init() before doing anything. */
    UNINITIALIZED = 0,
    /** The Engine is initializing, don't do anything. */
    INITIALIZING = 1,
    /** The Engine is ready to run. */
    STOPPED = 2,
    /** The Engine has entered main loop. */
    RUNNING = 3,
    /** The Engine is stopping the main loop, don't do anything. */
    STOPPING = 4
};

#endif //THEENGINE_ENGINESTATE_H
