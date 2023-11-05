//
// Created by mcumbrella on 23-10-18.
//

#ifndef THEENGINE_INVALIDSTATEEXCEPTION_H
#define THEENGINE_INVALIDSTATEEXCEPTION_H

#include "EngineException.h"

class InvalidStateException : public EngineException
{
public:
    explicit InvalidStateException(const string& msg) : EngineException(msg)
    {}
};

#endif //THEENGINE_INVALIDSTATEEXCEPTION_H
