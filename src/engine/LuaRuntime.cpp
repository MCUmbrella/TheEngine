//
// Created by mcumbrella on 23-10-30.
//

#include "LuaRuntime.h"
#include "util/Logger.h"
#include "exception/LuaException.h"
#include "Engine.h"
#include "ConfigManager.h"
#include "render/RenderManager.h"
#include "sound/SoundManager.h"

using kaguya::UserdataMetatable;

// this function is used in Lua scripts
void LuaRuntime::log_l(const int& lvl, const string& msg)
{
    switch(lvl)
    {
        case 0:
            logInfo << "{L} " << msg;
            break;
        case 1:
            logWarn << "{L} " << msg;
            break;
        default:
            logError << "{L} " << msg;
    }
}

void LuaRuntime::log_l(const string& msg)
{
    log_l(0, msg);
}

State& LuaRuntime::getLua()
{
    static State L;
    return L;
}

void LuaRuntime::init()
{
    logInfo << "Initializing Lua runtime";

    State& l = getLua();
    l.openlibs();
    l.setErrorHandler([](int status, const char* msg){
        throw LuaException(
            "Uncaught error in Lua runtime"
            "\nStatus code: " + std::to_string(status) +
            "\nMessage: " + msg
        );
    });

    logInfo << "Registering classes";

    logInfo << "-- Runtime";
    l["Runtime"].setClass(
        UserdataMetatable<LuaRuntime>()
            .addOverloadedFunctions(
                "log",
                static_cast<void (*)(const int&, const string&)>(log_l),
                static_cast<void (*)(const string&)>(log_l)
            )
            .addStaticFunction("switchTo", [](const string& path){
                LuaRuntime::runFile(ConfigManager::getUserDataPath() + "/data/lua/" + path);
            })
    );

    logInfo << "-- Engine";
    l["Engine"].setClass(
        UserdataMetatable<Engine>()
            .addStaticFunction("stop", [](){
                logInfo << "Script called Engine.stop()";
                Engine::stop();
            })
            .addStaticFunction("getState", &Engine::getState)
            .addStaticFunction("keyPressed", &Engine::keyPressed)
            .addStaticFunction("keyHolding", &Engine::keyHolding)
            .addStaticFunction("keyRepeated", &Engine::keyRepeated)
            .addStaticFunction("currentTick", &Engine::currentTick)
    );

    logInfo << "-- RenderManager";
    l["RenderManager"].setClass(
        UserdataMetatable<RenderManager>()
            .addStaticFunction("getWindowWidth", &RenderManager::getWindowWidth)
            .addStaticFunction("getWindowHeight", &RenderManager::getWindowHeight)
            .addStaticFunction("setWindowSize", &RenderManager::setWindowSize)
            .addStaticFunction("setBackgroundColor", &RenderManager::setBackgroundColor)
            .addStaticFunction("getWindowTitle", &RenderManager::getWindowTitle)
            .addStaticFunction("setWindowTitle", &RenderManager::setWindowTitle)
            .addStaticFunction("getWindowX", &RenderManager::getWindowX)
            .addStaticFunction("getWindowY", &RenderManager::getWindowY)
            .addStaticFunction("setWindowLocation", &RenderManager::setWindowLocation)
            .addStaticFunction("loadTexture", [](const string& path){
                RenderManager::loadTexture(ConfigManager::getUserDataPath() + "/assets/textures/" + path);
            })
            .addStaticFunction("unloadTexture", [](const string& path){
                RenderManager::unloadTexture(ConfigManager::getUserDataPath() + "/assets/textures/" + path);
            })
            .addStaticFunction("addLayer", &RenderManager::addLayer)
            .addStaticFunction("removeLayer", &RenderManager::removeLayer)
            .addStaticFunction("getLayer", &RenderManager::getLayer)
            .addStaticFunction("hasLayer", &RenderManager::hasLayer)
    );

    logInfo << "-- RenderEntity";
    l["RenderEntity"].setClass(
        UserdataMetatable<RenderEntity>()
            .addProperty("x", &RenderEntity::x)
            .addProperty("y", &RenderEntity::y)
            .addProperty("hitboxWidth", &RenderEntity::hitboxWidth)
            .addProperty("hitboxHeight", &RenderEntity::hitboxHeight)
            .addProperty("textureWidth", &RenderEntity::textureWidth)
            .addProperty("textureHeight", &RenderEntity::textureHeight)
            .addProperty("textureOffsetX", &RenderEntity::textureOffsetX)
            .addProperty("textureOffsetY", &RenderEntity::textureOffsetY)
            .addProperty("textureDegree", &RenderEntity::textureDegree)
            .addFunction("getId", &RenderEntity::getId)
            .addFunction("setLocation", &RenderEntity::setLocation)
            .addFunction("move", &RenderEntity::move)
            .addFunction("setTextureOffset", &RenderEntity::setTextureOffset)
            .addFunction("moveTexture", &RenderEntity::moveTexture)
            .addFunction("rotate", &RenderEntity::rotate)
            .addFunction("resizeHitbox", &RenderEntity::resizeHitbox)
            .addFunction("resizeTexture", &RenderEntity::resizeTexture)
            .addFunction("resetTextureSize", &RenderEntity::resetTextureSize)
            .addFunction("resetHitboxSize", &RenderEntity::resetHitboxSize)
            .addFunction("changeTexture", &RenderEntity::changeTexture_l)
    );
    logInfo << "-- RenderLayer";
    l["RenderLayer"].setClass(
        UserdataMetatable<RenderLayer>()
            .addFunction("getOrder", &RenderLayer::getOrder)
            .addFunction("addEntity", &RenderLayer::addEntity_l)
            .addFunction("removeEntity", &RenderLayer::removeEntity)
            .addFunction("getEntity", &RenderLayer::getEntity)
            .addFunction("hasEntity", &RenderLayer::hasEntity)
            .addFunction("size", &RenderLayer::size)
            .addFunction("clear", &RenderLayer::clear)
    );

    logInfo << "-- SoundManager";
    l["SoundManager"].setClass(
        UserdataMetatable<SoundManager>()
            .addStaticFunction("addSound", [](const string& name, const string& path) -> Sound*{
                return SoundManager::addSound(name, ConfigManager::getUserDataPath() + "/assets/sounds/" + path);
            })
            .addStaticFunction("removeSound", &SoundManager::removeSound)
            .addStaticFunction("getSound", &SoundManager::getSound)
            .addStaticFunction("hasSound", &SoundManager::hasSound)
            .addOverloadedFunctions(
                "playSound",
                static_cast<PlayingSound (*)(const string&)>(&SoundManager::playSound),
                static_cast<PlayingSound (*)(const Sound*)>(&SoundManager::playSound)
            )
            .addStaticFunction("addMusic", [](const string& name, const string& path) -> Music*{
                return SoundManager::addMusic(name, ConfigManager::getUserDataPath() + "/assets/sounds/" + path);
            })
            .addStaticFunction("removeMusic", &SoundManager::removeMusic)
            .addStaticFunction("getMusic", &SoundManager::getMusic)
            .addStaticFunction("hasMusic", &SoundManager::hasMusic)
            .addOverloadedFunctions(
                "playMusic",
                static_cast<void (*)(const string&)>(&SoundManager::playMusic),
                static_cast<void (*)(const Music*)>(&SoundManager::playMusic)
            )
            .addStaticFunction("pauseMusic", &SoundManager::pauseMusic)
            .addStaticFunction("resumeMusic", &SoundManager::resumeMusic)
            .addStaticFunction("stopMusic", &SoundManager::stopMusic)
            .addStaticFunction("gc", &SoundManager::gc)
    );

    logInfo << "-- Sound";
    l["Sound"].setClass(
        UserdataMetatable<Sound>()
            .addFunction("getName", &Sound::getName)
            .addFunction("getPath", &Sound::getPath)
            .addFunction("reassign", &Sound::reassign)
            .addFunction("play", &Sound::play)
    );

    logInfo << "-- Music";
    l["Music"].setClass(
        UserdataMetatable<Music>()
            .addFunction("getName", &Music::getName)
            .addFunction("getPath", &Music::getPath)
            .addFunction("reassign", &Music::reassign)
            .addFunction("play", &Music::play)
            .addFunction("pause", &Music::pause)
            .addFunction("resume", &Music::resume)
            .addFunction("stop", &Music::stop)
    );

    logInfo << "-- PlayingSound";
    l["PlayingSound"].setClass(
        UserdataMetatable<PlayingSound>()
            .addFunction("pause", &PlayingSound::pause)
            .addFunction("resume", &PlayingSound::resume)
            .addFunction("stop", &PlayingSound::stop)
            .addFunction("isValid", &PlayingSound::isValid)
    );

    logInfo << "Lua runtime initialization success";
}

bool LuaRuntime::runFile(const string& path, const bool& throws)
{
//    logInfo << "Executing Lua script: " << path;
    bool success = false;
    try
    {
        success = getLua().dofile(path);
    }
    catch(LuaException& e)
    {
        if(throws)
            throw e;
        else
            logError << e.what();
    }
    return success;
}

bool LuaRuntime::run(const string& content, const bool& throws)
{
    bool success = getLua().dostring(content);
    if(throws && !success)
        throw LuaException("Error occurred while executing Lua script: " + content);
    return success;
}
