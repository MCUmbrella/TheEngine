//
// Created by mcumbrella on 23-10-30.
//

#ifndef THEENGINE_LUAEXCEPTION_H
#define THEENGINE_LUAEXCEPTION_H

#include "EngineException.h"

class LuaException : public EngineException
{
public:
    explicit LuaException(const string& msg) : EngineException(msg)
    {}
};

#endif //THEENGINE_LUAEXCEPTION_H
