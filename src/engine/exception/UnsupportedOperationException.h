//
// Created by mcumbrella on 2023/11/14.
//

#ifndef THEENGINE_UNSUPPORTEDOPERATIONEXCEPTION_H
#define THEENGINE_UNSUPPORTEDOPERATIONEXCEPTION_H

#include "EngineException.h"

class UnsupportedOperationException : public EngineException
{
public:
    explicit UnsupportedOperationException(const string& msg) : EngineException(msg)
    {}
};

#endif //THEENGINE_UNSUPPORTEDOPERATIONEXCEPTION_H
