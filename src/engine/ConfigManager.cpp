//
// Created by mcumbrella on 2023/11/6.
//

#include "ConfigManager.h"
#include "../thirdparty/nlohmann-json-3.11.2/json.hpp"
#include "util/Logger.h"
#include "util/FileUtil.h"
#include "exception/ConfigException.h"
#include <SDL2/SDL.h>

using nlohmann::json;

static struct
{
    string engineDataPath;
    string userDataPath;
    int targetTps;
    // don't worry if the compiler complains about the field is not
    // initialized, the default value will be set in the future
} cfgEngine;

static struct
{
    bool softwareRender;
    bool accelerated;
    bool vsync;
} cfgRenderManager;

void ConfigManager::loadConfig(const string& path)
{
    logInfo << "Loading config from " << path;
    string s = FileUtil::readStr(path);
    json j = json::parse(s, nullptr, false, false);
    if(!j.is_object())
        throw ConfigException("Invalid JSON format");

    // read "engine" section
    if(j.contains("engine"))
    {
        auto& je = j["engine"];
        if(!je.is_object())
            throw ConfigException("Invalid field type: 'engine' should be Object");

        // engine.engineDataPath
        cfgEngine.engineDataPath =
            je.contains("engineDataPath") && je["engineDataPath"].is_string() ? je["engineDataPath"] : "the_engine";
        // engine.userDataPath
        if(je.contains("userDataPath"))
        {
            if(je["userDataPath"].is_string())
                cfgEngine.userDataPath = je["userDataPath"];
            else
                throw ConfigException("Invalid field type: 'engine.userDataPath' should be String");
        }
        else
            throw ConfigException("Essential field missing: String 'engine.userDataPath'");
        // engine.targetTps
        cfgEngine.targetTps =
            je.contains("targetTps") && je["targetTps"].is_number_integer() ? je["targetTps"].get<int>() : 60;
        if(cfgEngine.targetTps <= 0)
            throw ConfigException("Invalid value: engine.targetTps must be larger than 0");
    }
    else
        throw ConfigException("Essential field missing: Object 'engine'");

    // read "renderManager" section
    if(j.contains("renderManager"))
    {
        auto& jrm = j["renderManager"];
        if(!jrm.is_object())
            throw ConfigException("Invalid field type: 'renderManager' should be Object");

        // renderManager.softwareRender
        cfgRenderManager.softwareRender =
            jrm.contains("softwareRender") && jrm["softwareRender"].is_boolean() && jrm["softwareRender"].get<bool>();
        // renderManager.accelerated
        cfgRenderManager.accelerated =
            jrm.contains("accelerated") && jrm["accelerated"].is_boolean() && jrm["accelerated"].get<bool>();
        // renderManager.vsync
        cfgRenderManager.vsync =
            jrm.contains("vsync") && jrm["vsync"].is_boolean() && jrm["vsync"].get<bool>();
    }

    logInfo << "Engine data: " << cfgEngine.engineDataPath;
    logInfo << "User data: " << cfgEngine.userDataPath;
    logInfo << "Config loaded";
}

string ConfigManager::getEngineDataPath()
{
    return cfgEngine.engineDataPath;
}

string ConfigManager::getUserDataPath()
{
    return cfgEngine.userDataPath;
}

int ConfigManager::getEngineTargetTps()
{
    return cfgEngine.targetTps;
}

unsigned int ConfigManager::getSdlRendererFlags()
{
    unsigned int flags = 0;
    flags |= cfgRenderManager.softwareRender ? SDL_RENDERER_SOFTWARE : 0;
    flags |= cfgRenderManager.accelerated ? SDL_RENDERER_ACCELERATED : 0;
    flags |= cfgRenderManager.vsync ? SDL_RENDERER_PRESENTVSYNC : 0;
    return flags;
}
