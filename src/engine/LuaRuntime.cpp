//
// Created by mcumbrella on 23-10-30.
//

#include "LuaRuntime.h"
#include "util/Logger.h"
#include "exception/LuaException.h"
#include "Engine.h"
#include "RenderManager.h"
#include "render/RenderEntity.h"
#include "ConfigManager.h"

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

kaguya::State& LuaRuntime::getLua()
{
    static kaguya::State L;
    return L;
}

void LuaRuntime::init()
{
    logInfo << "Initializing Lua runtime";

    kaguya::State& l = getLua();
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
        kaguya::UserdataMetatable<LuaRuntime>()
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
        kaguya::UserdataMetatable<Engine>()
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
        kaguya::UserdataMetatable<RenderManager>()
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
        kaguya::UserdataMetatable<RenderEntity>()
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
        kaguya::UserdataMetatable<RenderLayer>()
            .addFunction("getOrder", &RenderLayer::getOrder)
            .addFunction("addEntity", &RenderLayer::addEntity_l)
            .addFunction("removeEntity", &RenderLayer::removeEntity)
            .addFunction("getEntity", &RenderLayer::getEntity)
            .addFunction("hasEntity", &RenderLayer::hasEntity)
            .addFunction("size", &RenderLayer::size)
            .addFunction("clear", &RenderLayer::clear)
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
