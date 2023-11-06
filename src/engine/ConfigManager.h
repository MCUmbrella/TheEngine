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
    static void loadConfig(const string& path);

    static string getEngineDataPath();

    static string getUserDataPath();
};


#endif //THEENGINE_CONFIGMANAGER_H
