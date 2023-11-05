//
// Created by mcumbrella on 23-10-18.
//

#ifndef THEENGINE_CASTEXCEPTION_H
#define THEENGINE_CASTEXCEPTION_H

#include "EngineException.h"

class CastException : public EngineException
{
public:
    explicit CastException(const string& msg) : EngineException(msg)
    {}
};

#endif //THEENGINE_CASTEXCEPTION_H
