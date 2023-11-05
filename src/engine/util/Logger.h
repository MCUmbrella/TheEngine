//
// Created by mcumbrella on 23-10-16.
//

#ifndef THEENGINE_LOGGER_H
#define THEENGINE_LOGGER_H

#include <iostream>
#include <sstream>
#include <filesystem>
#include "CommonUtil.h"

namespace Logger
{
    class LogStream
    {
    private:
        std::stringstream stream;
    public:
        template<typename T>
        LogStream& operator<<(const T& value)
        {
            stream << value;
            return *this;
        }

        ~LogStream()
        {
            std::cerr << this->str() << std::endl;
        }

        std::string str() const
        {
            return stream.str();
        }
    };
}

/**
 * Extract the name of a file from its path.
 * @param path File path. e.g. "/home/me/Project/src/Logger.h"
 * @return The name of the file. e.g. "Logger.h"
 */
constexpr const char* extractFileName(const char* path)
{
    const char* fileName = path;
    while(*path != '\0')
        if(*path++ == std::filesystem::path::preferred_separator)
            fileName = path;
    return fileName;
}

#define STRINGIZE(x) #x
#define TOSTRING(s) STRINGIZE(s)

#define LOG_MESSAGE_PREFIX(lvl) \
    '[' << CommonUtil::nano2string(CommonUtil::currentTimeNanos()) \
    << "] [" lvl "] [" << extractFileName(__FILE__) << ":" TOSTRING(__LINE__) "] "

#define logInfo \
    Logger::LogStream() << LOG_MESSAGE_PREFIX("INFO")

#define logWarn \
    Logger::LogStream() << LOG_MESSAGE_PREFIX("WARN")

#define logError \
    Logger::LogStream() << LOG_MESSAGE_PREFIX("ERRO")

#define logCustom(lvl) \
    Logger::LogStream() << LOG_MESSAGE_PREFIX(lvl)

#endif //THEENGINE_LOGGER_H
