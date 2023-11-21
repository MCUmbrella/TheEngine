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

static ConfigManager::EngineConfig& cfg = ConfigManager::getEngineConfig();
static State L;

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
            break;
    }
}

void LuaRuntime::log_l(const string& msg)
{
    log_l(0, msg);
}

State& LuaRuntime::getLuaState()
{
    return L;
}

void LuaRuntime::init()
{
    logInfo << "Initializing Lua runtime";

    L.openlibs();
    L.setErrorHandler([](int status, const char* msg){
        throw LuaException(
            "Uncaught error in Lua runtime"
            "\nStatus code: " + std::to_string(status) +
            "\nMessage: " + msg
        );
    });

    logInfo << "Registering classes";

    logInfo << "-- Runtime";
    L["Runtime"].setClass(
        UserdataMetatable<LuaRuntime>()
            .addOverloadedFunctions(
                "log",
                static_cast<void (*)(const int&, const string&)>(&log_l),
                static_cast<void (*)(const string&)>(&log_l)
            )
            .addStaticFunction("execute", [](const string& path){
                LuaRuntime::runFile(cfg.userDataPath + "/data/lua/" + path);
            })
    );

    logInfo << "-- Engine";
    L["Engine"].setClass(
        UserdataMetatable<Engine>()
            .addStaticFunction("stop", [](){
                logInfo << "Script called Engine.stop()";
                Engine::stop();
            })
            .addStaticFunction("getState", &Engine::getState)
            .addStaticFunction("currentTick", &Engine::currentTick)
    );

    logInfo << "-- Mouse";
    L["Mouse"].setClass(
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
    L["Keyboard"].setClass(
        UserdataMetatable<Keyboard>()
            .addStaticFunction("holding", [](const int& scancode){return Engine::getKeyboard().holding(scancode);})
            .addStaticFunction("pressed", [](const int& scancode){return Engine::getKeyboard().pressed(scancode);})
            .addStaticFunction("repeated", [](const int& scancode){return Engine::getKeyboard().repeated(scancode);})
    );

    logInfo << "-- Window";
    L["Window"].setClass(
        UserdataMetatable<Window>()
            .addStaticFunction("getX", [](){return RenderManager::getWindow()->getX();})
            .addStaticFunction("getY", [](){return RenderManager::getWindow()->getY();})
            .addStaticFunction(
                "setLocation",
                [](const int& x, const int& y){RenderManager::getWindow()->setLocation(x, y);}
            )
            .addStaticFunction("getWidth", [](){return RenderManager::getWindow()->getWidth();})
            .addStaticFunction("getHeight", [](){return RenderManager::getWindow()->getHeight();})
            .addStaticFunction(
                "setSize",
                [](const int& w, const int& h){RenderManager::getWindow()->setSize(w, h);}
            )
            .addStaticFunction("getTitle", [](){return RenderManager::getWindow()->getTitle();})
            .addStaticFunction("setTitle", [](const string& title){return RenderManager::getWindow()->setTitle(title);})
            .addOverloadedFunctions(
                "fullscreen",
                [](){return RenderManager::getWindow()->isFullscreen();},
                [](const bool& state){return RenderManager::getWindow()->setFullscreen(state);}
            )
            .addOverloadedFunctions(
                "resizable",
                [](){return RenderManager::getWindow()->isResizable();},
                [](const bool& state){return RenderManager::getWindow()->setResizable(state);}
            )
    );

    logInfo << "-- RenderManager";
    L["RenderManager"].setClass(
        UserdataMetatable<RenderManager>()
            .addStaticFunction("setBackgroundColor", &RenderManager::setBackgroundColor)
            .addStaticFunction("loadTexture", [](const string& path){
                RenderManager::loadTexture(cfg.userDataPath + "/assets/textures/" + path);
            })
            .addStaticFunction("unloadTexture", [](const string& path){
                RenderManager::unloadTexture(cfg.userDataPath + "/assets/textures/" + path);
            })
            .addStaticFunction("addLayer", &RenderManager::addLayer)
            .addStaticFunction("removeLayer", &RenderManager::removeLayer)
            .addStaticFunction("getLayer", &RenderManager::getLayer)
            .addStaticFunction("hasLayer", &RenderManager::hasLayer)
            .addStaticFunction("loadFont", [](const string& name, const string& path){
                RenderManager::loadFont(name, cfg.userDataPath + "/assets/fonts/" + path);
            })
            .addStaticFunction("unloadFont", &RenderManager::unloadFont)
            .addStaticFunction("hasFont", &RenderManager::hasFont)
    );

    logInfo << "-- RenderEntity";
    L["RenderEntity"].setClass(
        UserdataMetatable<RenderEntity>()
            .addProperty("x", &RenderEntity::x)
            .addProperty("y", &RenderEntity::y)
            .addProperty("textureWidth", &RenderEntity::textureWidth)
            .addProperty("textureHeight", &RenderEntity::textureHeight)
            .addProperty("textureOffsetX", &RenderEntity::textureOffsetX)
            .addProperty("textureOffsetY", &RenderEntity::textureOffsetY)
            .addProperty("textureDegree", &RenderEntity::textureDegree)
            .addProperty("textureRed", &RenderEntity::textureRed)
            .addProperty("textureGreen", &RenderEntity::textureGreen)
            .addProperty("textureBlue", &RenderEntity::textureBlue)
            .addProperty("textureAlpha", &RenderEntity::textureAlpha)
            .addFunction("getId", &RenderEntity::getId)
            .addFunction("setLocation", &RenderEntity::setLocation)
            .addFunction("move", &RenderEntity::move)
            .addFunction("setTextureOffset", &RenderEntity::setTextureOffset)
            .addFunction("moveTexture", &RenderEntity::moveTexture)
            .addFunction("rotate", &RenderEntity::rotate)
            .addFunction("setTextureSize", &RenderEntity::setTextureSize)
            .addFunction("resetTextureSize", &RenderEntity::resetTextureSize)
            .addStaticFunction("setTexture", [](RenderEntity* instance, const string& path){
                instance->setTexture(cfg.userDataPath + "/assets/textures/" + path);
            }) // actually it is member function, not static
            .addFunction("getFlip", &RenderEntity::getFlip)
            .addFunction("setFlip", &RenderEntity::setFlip)
            .addFunction("setCrop", &RenderEntity::setCrop)
            .addFunction("resetCrop", &RenderEntity::resetCrop)
    );

    logInfo << "-- TextRenderEntity";
    L["TextRenderEntity"].setClass(
        UserdataMetatable<TextRenderEntity, RenderEntity>()
            .addFunction("setTexture", &TextRenderEntity::setTexture)
            .addFunction("getContent", &TextRenderEntity::getContent)
            .addFunction("setContent", &TextRenderEntity::setContent)
    );

    logInfo << "-- RenderLayer";
    L["RenderLayer"].setClass(
        UserdataMetatable<RenderLayer>()
            .addFunction("getOrder", &RenderLayer::getOrder)
            .addStaticFunction("addEntity", [](RenderLayer* instance, const string& texturePath){
                return instance->addEntity(cfg.userDataPath + "/assets/textures/" + texturePath);
            })
            .addOverloadedFunctions(
                "addText",
                static_cast<TextRenderEntity* (RenderLayer::*)(const string&, const string&,
                                                               const int&)> (&RenderLayer::addText),
                static_cast<TextRenderEntity* (RenderLayer::*)(const string&, const string&)> (&RenderLayer::addText),
                static_cast<TextRenderEntity* (RenderLayer::*)(const string&, const int&)> (&RenderLayer::addText),
                static_cast<TextRenderEntity* (RenderLayer::*)(const string&)> (&RenderLayer::addText)
            )
            .addFunction("removeEntity", &RenderLayer::removeEntity)
            .addFunction("getEntity", &RenderLayer::getEntity)
            .addFunction("hasEntity", &RenderLayer::hasEntity)
            .addFunction("size", &RenderLayer::size)
            .addFunction("clear", &RenderLayer::clear)
    );

    logInfo << "-- SoundManager";
    L["SoundManager"].setClass(
        UserdataMetatable<SoundManager>()
            .addStaticFunction("addSound", [](const string& name, const string& path){
                return SoundManager::addSound(name, cfg.userDataPath + "/assets/sounds/" + path);
            })
            .addStaticFunction("removeSound", &SoundManager::removeSound)
            .addStaticFunction("getSound", &SoundManager::getSound)
            .addStaticFunction("hasSound", &SoundManager::hasSound)
            .addStaticFunction("addMusic", [](const string& name, const string& path){
                return SoundManager::addMusic(name, cfg.userDataPath + "/assets/sounds/" + path);
            })
            .addStaticFunction("removeMusic", &SoundManager::removeMusic)
            .addStaticFunction("getMusic", &SoundManager::getMusic)
            .addStaticFunction("hasMusic", &SoundManager::hasMusic)
            .addStaticFunction("gc", &SoundManager::gc)
    );

    logInfo << "-- Sound";
    L["Sound"].setClass(
        UserdataMetatable<Sound>()
            .addFunction("getName", &Sound::getName)
            .addFunction("getPath", &Sound::getPath)
            .addFunction("reassign", &Sound::reassign)
            .addFunction("play", &Sound::play)
    );

    logInfo << "-- Music";
    L["Music"].setClass(
        UserdataMetatable<Music>()
            .addFunction("getName", &Music::getName)
            .addFunction("getPath", &Music::getPath)
            .addFunction("reassign", &Music::reassign)
            .addFunction("isCurrent", &Music::isCurrent)
            .addFunction("play", &Music::play)
            .addStaticFunction("pause", &Music::pause)
            .addStaticFunction("resume", &Music::resume)
            .addStaticFunction("stop", &Music::stop)
            .addOverloadedFunctions(
                "volume",
                &Music::getVolume,
                &Music::setVolume
            )
    );

    logInfo << "-- PlayingSound";
    L["PlayingSound"].setClass(
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
        success = L.dofile(path);
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
    bool success = L.dostring(content);
    if(throws && !success)
        throw LuaException("Error occurred while executing Lua script: " + content);
    return success;
}
