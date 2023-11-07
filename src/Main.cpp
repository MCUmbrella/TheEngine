#include "engine/Engine.h"
#include "engine/util/Logger.h"

int main(int argc, char** argv)
{
    string configPath = "the_engine_config.json";
    if(argc > 1)
        configPath = argv[1];
    logInfo << "The Engine is coming up, please wait";
    Engine::init(configPath);
    Engine::run();
    logInfo << "The Engine is down";
    return 0;
}
