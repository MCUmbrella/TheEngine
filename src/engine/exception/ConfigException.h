//
// Created by mcumbrella on 23-10-18.
//

#ifndef THEENGINE_CONFIGEXCEPTION_H
#define THEENGINE_CONFIGEXCEPTION_H

#include "EngineException.h"

class ConfigException : public EngineException
{
public:
    explicit ConfigException(const string& msg) : EngineException(msg)
    {}
};

#endif //THEENGINE_CONFIGEXCEPTION_H
