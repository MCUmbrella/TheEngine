//
// Created by mcumbrella on 23-10-16.
//

#include <bitset>
#include "Engine.h"
#include "util/Logger.h"
#include "exception/InvalidStateException.h"
#include "render/RenderManager.h"
#include "LuaRuntime.h"
#include "exception/LuaException.h"
#include "ConfigManager.h"
#include "sound/SoundManager.h"

using std::bitset;

static EngineState state = UNINITIALIZED;
static int targetTps;
static long maxNsPerTick;
static unsigned long tickCounter = 0;
static Mouse mouse;
static Keyboard keyboard;

const Engine& Engine::getInstance()
{
    const static Engine THE_ENGINE;
    return THE_ENGINE;
}

void Engine::pollSDLEvents()
{
    memcpy(keyboard.keysH_p, keyboard.keysH, sizeof(keyboard.keysH));
    memcpy(mouse.buttonsH_p, mouse.buttonsH, sizeof(mouse.buttonsH));
    memset(mouse.wheel, 0, sizeof(mouse.wheel));
    SDL_GetMouseState(&mouse.x, &mouse.y);
    static SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        SDL_KeyboardEvent& ke = event.key;
        SDL_Scancode& kc = ke.keysym.scancode;
        SDL_MouseButtonEvent& mbe = event.button;
        SDL_MouseWheelEvent& mwe = event.wheel;
        switch(event.type)
        {
            case SDL_QUIT:
            {
                stop();
                return;
            }
            case SDL_KEYDOWN:
            {
                keyboard.keysH[0].set(kc, true);
                keyboard.keysH[1].set(kc, ke.repeat);
                break;
            }
            case SDL_KEYUP:
            {
                keyboard.keysH[0].set(kc, false);
                keyboard.keysH[1].set(kc, ke.repeat);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                if(mbe.button == SDL_BUTTON_LEFT)
                    mouse.buttonsH[0] = true;
                else if(mbe.button == SDL_BUTTON_MIDDLE)
                    mouse.buttonsH[1] = true;
                else if(mbe.button == SDL_BUTTON_RIGHT)
                    mouse.buttonsH[2] = true;
                else if(mbe.button == SDL_BUTTON_X1)
                    mouse.buttonsH[3] = true;
                else if(mbe.button == SDL_BUTTON_X2)
                    mouse.buttonsH[4] = true;
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if(mbe.button == SDL_BUTTON_LEFT)
                    mouse.buttonsH[0] = false;
                else if(mbe.button == SDL_BUTTON_MIDDLE)
                    mouse.buttonsH[1] = false;
                else if(mbe.button == SDL_BUTTON_RIGHT)
                    mouse.buttonsH[2] = false;
                else if(mbe.button == SDL_BUTTON_X1)
                    mouse.buttonsH[3] = false;
                else if(mbe.button == SDL_BUTTON_X2)
                    mouse.buttonsH[4] = false;
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                mouse.wheel[mwe.y > 0 ? 0 : 1] = true;
                mouse.wheel[mwe.x < 0 ? 2 : 3] = true;
                break;
            }
            default:
                break;
        }
    }
    keyboard.keysP = keyboard.keysH_p[0].flip() & keyboard.keysH[0];
    for(int i = 0; i != sizeof(mouse.buttonsH); i++)
        mouse.buttonsP[i] = !mouse.buttonsH_p[i] && mouse.buttonsH[i];
}

void Engine::init(const string& configPath)
{
    if(getState() == UNINITIALIZED)
    {
        logInfo << "Initializing";
        state = INITIALIZING;
        // load config
        ConfigManager::loadConfig(configPath);
        targetTps = ConfigManager::getEngineTargetTps();
        maxNsPerTick = 1000000000L / targetTps;
        // initialize subsystems
        LuaRuntime::init();
        LuaRuntime::runFile(ConfigManager::getEngineDataPath() + "/data/lua/preInit.lua");
        RenderManager::init();
        SoundManager::init();
        state = STOPPED;
        LuaRuntime::runFile(ConfigManager::getEngineDataPath() + "/data/lua/postInit.lua");
        logInfo << "The Engine is ready";
    }
    else
    {
        throw InvalidStateException("The Engine is already initialized");
    }
}

void Engine::run()
{
    if(state == STOPPED)
    {
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

    int64_t tickProfiler = 0;

    l["prepare"]();
    while(state == RUNNING)
    {
        int64_t startTime = CommonUtil::currentTimeNanos();

        // tick logics
        pollSDLEvents();
        RenderManager::getWindow()->update();
        l["tick"]();
        RenderManager::render();

        // delay & correction
        int64_t endTime = CommonUtil::currentTimeNanos();
        int64_t executionTime = endTime - startTime;
        int64_t delay = maxNsPerTick - executionTime;

        // if a single tick took too long to process, cancel the delay
        if(executionTime > maxNsPerTick)
        {
            delay = 0;
            logWarn << "Can't keep up! Tick #" << tickCounter << " took " << executionTime
                    << " ns (should be <= " << maxNsPerTick << " ns)";
        }

        // tick profiler things
        if(ConfigManager::enableProfiler())
        {
            tickProfiler += executionTime;
            if(tickCounter % ConfigManager::getEngineTargetTps() == ConfigManager::getEngineTargetTps() - 1)
            {
                long double avg = (long double) tickProfiler / ConfigManager::getEngineTargetTps();
                logInfo << "Average tick cost: " << avg
                        << "ns -- " << avg * 100 / maxNsPerTick << "% of max tick execution time";
                tickProfiler = 0;
            }
        }

        // ready for next tick
        std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
        ++tickCounter;
    }

    logInfo << "Exited main loop";
    l["cleanup"]();
    l.garbageCollect();
    SoundManager::shutdown();
    RenderManager::shutdown();
    SDL_Quit();
    state = STOPPED;
}

void Engine::stop()
{
    if(getState() == RUNNING)
    {
        logInfo << "Stopping The Engine";
        state = STOPPING;
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

unsigned long Engine::currentTick()
{
    return tickCounter;
}

Mouse& Engine::getMouse()
{
    return mouse;
}

Keyboard& Engine::getKeyboard()
{
    return keyboard;
}
