//
// Created by mcumbrella on 23-10-18.
//

#include "CommonUtil.h"

#ifdef _WIN32

#include <Windows.h>

#else

#include <unistd.h>
#include <pthread.h>

#endif

#include <chrono>
#include <cstdio>
#include <regex>
#include <filesystem>
#include "Logger.h"

int64_t CommonUtil::currentTimeNanos()
{
    using namespace std::chrono;
    return duration_cast<nanoseconds>(
        high_resolution_clock::now().time_since_epoch()
    ).count();
}

string CommonUtil::nano2string(const int64_t& timestampNs)
{
    time_t t = timestampNs / 1000000000;
    tm* tt = localtime(&t);
    char buf[24];
    strftime(buf, sizeof(buf), "%F %T:", tt);
    sprintf(&buf[strlen(buf)], "%03ld", timestampNs / 1000000 % 1000);
    return buf;
}

void CommonUtil::killThread(std::thread& t)
{
#ifdef _WIN32
    DWORD result = TerminateThread(t.native_handle(), 0);
        if (result == 0)
        {
            logError << "Error occurred while terminating thread: "
                     << GetLastError();
            throw std::runtime_error("Failed to terminate thread");
        }
#else
    if(pthread_cancel(t.native_handle()))
    {
        logError << "Error occurred while terminating thread: "
                 << strerror(errno);
        throw std::runtime_error("Failed to terminate thread");
    }
#endif
}

void CommonUtil::freeze(const unsigned int& ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000U);
#endif
}

bool CommonUtil::isInteger(const string& s)
{
    if(s.empty()) // empty string is considered not an integer
        return false;
    size_t sIndex = 0;
    if(s[sIndex] == '+' || s[sIndex] == '-') // skip a character if starts with '+' or '-'
        ++sIndex;
    if(sIndex == s.size()) // only sign, no digits?
        return false;
    while(sIndex < s.size() && isdigit(s[sIndex])) // check remaining characters
        ++sIndex;
    return sIndex == s.size(); // all remaining characters are decimal digits?
}

bool CommonUtil::isUnsigned(const string& s)
{
    if(s.empty())
        return false;
    size_t sIndex = 0;
    while(sIndex < s.size() && isdigit(s[sIndex]))
        ++sIndex;
    return sIndex == s.size();
}

bool CommonUtil::isDecimal(const string& s)
{
    if(s.empty())
        return false;
    size_t sIndex = 0;
    bool hasDot = false;
    if(s[sIndex] == '+' || s[sIndex] == '-')
        ++sIndex;
    if(sIndex == s.size())
        return false;
    while(sIndex < s.size())
    {
        if(!isdigit(s[sIndex])) // not a decimal digit?
        {
            if(s[sIndex] != '.') // not a dot? fail
                return false;
            if(hasDot) // dotted before? fail
                return false;
            hasDot = true; // the number is dotted
        }
        ++sIndex;
    }
    return true; // all characters are either digits or dots
}

bool CommonUtil::isHexagonal(const string& s)
{
    if(s.empty() || !isxdigit(s[0])) // must init with a hex digit
        return false;
    size_t sIndex = 1;
    while(sIndex < s.size() && isxdigit(s[sIndex])) // check remaining characters
        ++sIndex;
    return sIndex == s.size(); // all characters are hex digits
}

int64_t CommonUtil::snowflakeId()
{
    static auto s = snowflake<1697456461L, std::mutex>(1, 1);
    return s.nextid();
}
