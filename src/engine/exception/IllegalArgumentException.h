//
// Created by mcumbrella on 2023/11/8.
//

#ifndef THEENGINE_ILLEGALARGUMENTEXCEPTION_H
#define THEENGINE_ILLEGALARGUMENTEXCEPTION_H

#include "EngineException.h"

class IllegalArgumentException : public EngineException
{
public:
    explicit IllegalArgumentException(const string& msg) : EngineException(msg)
    {}
};

#endif //THEENGINE_ILLEGALARGUMENTEXCEPTION_H
