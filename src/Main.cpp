#include "engine/Engine.h"

int main(int argc, char** argv)
{
    string configPath = "the_engine_config.json";
    if(argc > 1)
        configPath = argv[1];
    Engine::init(configPath);
    Engine::start();
    return 0;
}
