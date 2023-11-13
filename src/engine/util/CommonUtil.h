//
// Created by mcumbrella on 23-10-18.
//

#ifndef THEENGINE_COMMONUTIL_H
#define THEENGINE_COMMONUTIL_H

#include <cstdint>
#include <string>
#include <thread>
#include <mutex>
#include "../../thirdparty/snowflake-cpp-71c1342/showflake.h"

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))

using std::string;

class CommonUtil
{
public:
    /**
     * How many nanoseconds have passed since 0:00:00 on January 1, 1970 (UTC)?
     * @return UNIX timestamp with nanoseconds.
     */
    static int64_t currentTimeNanos();

    /**
     * Convert the UNIX timestamp (with nanoseconds) to string.
     * @param timestampNs 1684558955107104193
     * @return "2023-05-20 13:02:35:107"
     */
    static string nano2string(const int64_t& timestampNs);

    /**
     * Send the terminate signal to a std::thread object.
     * @param t The reference to the thread.
     */
    static void killThread(std::thread& t);

    /**
     * Freeze the program for a few moment.
     * @param ms The milliseconds of the time to freeze.
     */
    static void freeze(const unsigned int& ms);

    /**
     * Check if the given string represents a decimal integer.
     */
    static bool isInteger(const string& s);

    /**
     * Check if the given string represents an unsigned decimal integer.
     */
    static bool isUnsigned(const string& s);

    /**
     * Check if the given string represents a decimal number.
     */
    static bool isDecimal(const string& s);

    /**
     * Check if the given string represents a hexagonal number.
     */
    static bool isHexagonal(const string& s);

    /**
     * Generate a snowflake ID.
     */
    static int64_t snowflakeId();
};

#endif //THEENGINE_COMMONUTIL_H
