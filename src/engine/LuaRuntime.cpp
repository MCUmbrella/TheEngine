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
            .addStaticFunction("execute", [](const string& path){
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
            .addStaticFunction("currentTick", &Engine::currentTick)
    );

    logInfo << "-- Mouse";
    l["Mouse"].setClass(
        UserdataMetatable<Mouse>()
            .addStaticFunction("getX", [](){return Engine::getMouse().getX();})
            .addStaticFunction("getY", [](){return Engine::getMouse().getY();})
            .addStaticFunction("lHolding", [](){return Engine::getMouse().lHolding();})
            .addStaticFunction("mHolding", [](){return Engine::getMouse().mHolding();})
            .addStaticFunction("rHolding", [](){return Engine::getMouse().rHolding();})
            .addStaticFunction("x1Holding", [](){return Engine::getMouse().x1Holding();})
            .addStaticFunction("x2Holding", [](){return Engine::getMouse().x2Holding();})
            .addStaticFunction("lPressed", [](){return Engine::getMouse().lPressed();})
            .addStaticFunction("mPressed", [](){return Engine::getMouse().mPressed();})
            .addStaticFunction("rPressed", [](){return Engine::getMouse().rPressed();})
            .addStaticFunction("x1Pressed", [](){return Engine::getMouse().x1Pressed();})
            .addStaticFunction("x2Pressed", [](){return Engine::getMouse().x2Pressed();})
            .addStaticFunction("wheelUp", [](){return Engine::getMouse().wheelUp();})
            .addStaticFunction("wheelDown", [](){return Engine::getMouse().wheelDown();})
            .addStaticFunction("wheelLeft", [](){return Engine::getMouse().wheelLeft();})
            .addStaticFunction("wheelRight", [](){return Engine::getMouse().wheelRight();})
            .addOverloadedFunctions(
                "hidden",
                [](){return Engine::getMouse().isHidden();},
                [](const bool& state){return Engine::getMouse().setHidden(state);}
            )
    );

    logInfo << "-- Keyboard";
    l["Keyboard"].setClass(
        UserdataMetatable<Keyboard>()
            .addStaticFunction("holding", [](const int& scancode){return Engine::getKeyboard().holding(scancode);})
            .addStaticFunction("pressed", [](const int& scancode){return Engine::getKeyboard().pressed(scancode);})
            .addStaticFunction("repeated", [](const int& scancode){return Engine::getKeyboard().repeated(scancode);})
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
            .addFunction("setHitboxSize", &RenderEntity::setHitboxSize)
            .addFunction("setTextureSize", &RenderEntity::setTextureSize)
            .addFunction("resetTextureSize", &RenderEntity::resetTextureSize)
            .addFunction("resetHitboxSize", &RenderEntity::resetHitboxSize)
            .addFunction("setTexture", &RenderEntity::setTexture_l)
            .addFunction("getFlip", &RenderEntity::getFlip)
            .addFunction("setFlip", &RenderEntity::setFlip)
            .addFunction("setCrop", &RenderEntity::setCrop)
            .addFunction("resetCrop", &RenderEntity::resetCrop)
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
            .addOverloadedFunctions(
                "musicVolume",
                &SoundManager::getMusicVolume,
                &SoundManager::setMusicVolume
            )
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
            .addFunction("isValid", &PlayingSound::isValid)
            .addFunction("pause", &PlayingSound::pause)
            .addFunction("resume", &PlayingSound::resume)
            .addFunction("stop", &PlayingSound::stop)
            .addOverloadedFunctions(
                "volume",
                &PlayingSound::getVolume,
                &PlayingSound::setVolume
            )
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
