//
// Created by mcumbrella on 2023/11/6.
//

#include "ConfigManager.h"
#include "../thirdparty/nlohmann-json-3.11.2/json.hpp"
#include "util/Logger.h"
#include "util/FileUtil.h"

static string engineDataPath;
static string userDataPath;

using nlohmann::json;

void ConfigManager::loadConfig(const string& path)
{
    logInfo << "Loading config from " << path;
    string s = FileUtil::readStr(path);
    json j = json::parse(s, nullptr, false, false);
    if(j.contains("engineDataPath"))
        engineDataPath = j["engineDataPath"];
    else
        engineDataPath = "the_engine";
    userDataPath = j["userDataPath"];
    logInfo << "Config loaded";
}

string ConfigManager::getEngineDataPath()
{
    return engineDataPath;
}

string ConfigManager::getUserDataPath()
{
    return userDataPath;
}
