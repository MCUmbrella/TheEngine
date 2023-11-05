//
// Created by mcumbrella on 23-10-18.
//

#ifndef THEENGINE_IOEXCEPTION_H
#define THEENGINE_IOEXCEPTION_H

#include "EngineException.h"

class IOException : public EngineException
{
public:
    explicit IOException(const string& msg) : EngineException(msg)
    {}
};

#endif //THEENGINE_IOEXCEPTION_H
