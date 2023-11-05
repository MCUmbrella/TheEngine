//
// Created by mcumbrella on 23-10-18.
//

#ifndef THEENGINE_ENGINEEXCEPTION_H
#define THEENGINE_ENGINEEXCEPTION_H

#include <exception>
#include <string>

using std::string;

class EngineException : public std::runtime_error
{
public:
    explicit EngineException(const string& msg) : std::runtime_error(msg)
    {}
};

#endif //THEENGINE_ENGINEEXCEPTION_H
