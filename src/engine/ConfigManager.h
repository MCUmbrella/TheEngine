//
// Created by mcumbrella on 2023/11/6.
//

#ifndef THEENGINE_CONFIGMANAGER_H
#define THEENGINE_CONFIGMANAGER_H

#include <string>

using std::string;

class ConfigManager
{
public:
    class EngineConfig
    {
    public:
        string engineDataPath;
        string userDataPath;
        int targetTps;
        bool enableProfiler;
    };

    class RenderManagerConfig
    {
    public:
        bool softwareRender;
        bool accelerated;
        bool vsync;

        unsigned int rendererFlags();
    };

    static void loadConfig(const string& path);

    static EngineConfig& getEngineConfig();

    static RenderManagerConfig& getRenderManagerConfig();
};


#endif //THEENGINE_CONFIGMANAGER_H
