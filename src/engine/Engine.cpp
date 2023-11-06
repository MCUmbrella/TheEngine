//
// Created by mcumbrella on 23-10-16.
//

#include <bitset>
#include "Engine.h"
#include "util/Logger.h"
#include "exception/InvalidStateException.h"
#include "RenderManager.h"
#include "LuaRuntime.h"
#include "exception/LuaException.h"
#include "ConfigManager.h"

using std::bitset;

static EngineState state = UNINITIALIZED;
const static int TARGET_TPS = 60;
const static int TPS_CORRECTION_FREQ = 10;
const static long TICK_MAX_NS = 1000000000L / TARGET_TPS;
static unsigned long tickCounter = 0;
static bitset<1024> keysP{}; // pressed keys at a single tick (calculated at the end of pollSDLEvents)
static bitset<1024> keysH[2]{}; // currently holding keys. [0]: down, [1]: repeat
static bitset<1024> keysH_p[2]{}; // keysH at previous tick

const Engine& Engine::getInstance()
{
    const static Engine THE_ENGINE;
    return THE_ENGINE;
}

void Engine::pollSDLEvents()
{
    memcpy(keysH_p, keysH, sizeof(keysH));
    static SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        SDL_KeyboardEvent& ke = event.key;
        SDL_Scancode& kc = ke.keysym.scancode;
        switch(event.type)
        {
            case SDL_QUIT:
            {
                stop();
                return;
            }
            case SDL_KEYDOWN:
            {
                keysH[0].set(kc, true);
                keysH[1].set(kc, ke.repeat);
                break;
            }
            case SDL_KEYUP:
            {
                keysH[0].set(kc, false);
                keysH[1].set(kc, ke.repeat);
                break;
            }
            default:
                break;
        }
    }
    keysP = keysH_p[0].flip() & keysH[0]; // for(int i = 0; i != 1024; i++) keysP.set(i, !keysH_p[0][i] && keysH[0][i]);
}

void Engine::init(const string& configPath)
{
    if(getState() == UNINITIALIZED || getState() == STOPPED)
    {
        logInfo << "Initializing";
        state = INITIALIZING;
        ConfigManager::loadConfig(configPath);
        LuaRuntime::init();
        LuaRuntime::runFile(ConfigManager::getEngineDataPath() + "/data/lua/preInit.lua");
        RenderManager::init();
        logInfo << "Initialization OK";
        state = STOPPED;
        LuaRuntime::runFile(ConfigManager::getEngineDataPath() + "/data/lua/postInit.lua");
    }
    else
    {
        throw InvalidStateException("Can't initialize while The Engine is running");
    }
}

void Engine::start()
{
    if(state == STOPPED)
    {
        logInfo << "Starting The Engine";
        tickCounter = 0;
        state = RUNNING;
        mainLoop();
    }
    else
    {
        throw InvalidStateException("The Engine is uninitialized or already running");
    }
}

void Engine::mainLoop()
{
    logInfo << "Entering main loop";

    LuaRuntime::runFile(ConfigManager::getUserDataPath() + "/data/lua/main.lua");
    kaguya::State& l = LuaRuntime::getLua();
    for(auto& s : {"prepare", "tick", "cleanup"})
        if(!l[s].isType<kaguya::LuaFunction>())
            throw LuaException(string("Missing Lua function: ") + s);

    l["prepare"]();
    while(state == RUNNING)
    {
        int64_t startTime = CommonUtil::currentTimeNanos();

        // tick logics
        pollSDLEvents();
        l["tick"]();
        RenderManager::render();

        // delay & correction
        int64_t endTime = CommonUtil::currentTimeNanos(),
            executionTime = endTime - startTime,
            delay = TICK_MAX_NS - executionTime;
        // if a single tick took too long to process, cancel the delay
        if(executionTime > TICK_MAX_NS)
        {
            delay = 0;
            logWarn << "Can't keep up! Tick #" << tickCounter << " took " << executionTime
                    << " ns (should be <= " << TICK_MAX_NS << " ns)";
            goto label_1;
        }
        // attempt to reduce time drift
        static int64_t prevStartTime = -1, corr = 0;
        if(tickCounter % (TARGET_TPS / TPS_CORRECTION_FREQ) == 0)
        {
            if(prevStartTime == -1)
                prevStartTime = startTime;
            else
            {
                int64_t expectedEndTime = prevStartTime + 1000000000L / TPS_CORRECTION_FREQ;
                corr = (endTime - expectedEndTime) / (TARGET_TPS / TPS_CORRECTION_FREQ);
                prevStartTime = startTime;
            }
        }
        label_1:
        ++tickCounter;
        //SDL_Delay((delay - corr) / 1000000); // not accurate
        std::this_thread::sleep_for(std::chrono::nanoseconds(delay - corr));
    }
    l["cleanup"]();

    logInfo << "Exited main loop";
}

void Engine::stop()
{
    if(getState() == RUNNING)
    {
        logInfo << "Stopping The Engine";
        state = STOPPING;
        RenderManager::shutdown();
        SDL_Quit();
        state = STOPPED;
        logInfo << "Waiting for main loop to exit";
    }
    else
    {
        throw InvalidStateException("The engine is not running");
    }
}

const EngineState& Engine::getState()
{
    return state;
}

bool Engine::keyPressed(int code)
{
    return keysP[code];
}

bool Engine::keyHolding(int code)
{
    return keysH[0][code];
}

bool Engine::keyRepeated(int code)
{
    return keysH[1][code];
}

unsigned long Engine::currentTick()
{
    return tickCounter;
}
