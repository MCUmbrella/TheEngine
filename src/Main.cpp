#include "engine/Engine.h"
#include "engine/util/Logger.h"

int main(int argc, char** argv)
{
    logInfo << "Copyright (c) 2023 MCUmbrella. The Engine is provided under the MIT license, "
               "comes with ABSOLUTELY NO WARRANTY to the extent permitted by applicable law.";
    logInfo << "https://github.com/MCUmbrella/TheEngine";
    string configPath = "the_engine_config.json";
    if(argc > 1)
        configPath = argv[1];
    logInfo << "The Engine is coming up, please wait";
    Engine::init(configPath);
    Engine::run();
    logInfo << "The Engine is down";
    return 0;
}
