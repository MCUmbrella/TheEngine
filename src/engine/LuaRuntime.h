//
// Created by mcumbrella on 23-10-30.
//

#ifndef THEENGINE_LUARUNTIME_H
#define THEENGINE_LUARUNTIME_H

#include "../thirdparty/kaguya-9d77cad/kaguya.hpp"
#include <string>

using std::string;
using kaguya::State;

class LuaRuntime
{
private:
    static void log_l(const int& lvl, const string& msg);

    static void log_l(const string& msg);

public:
    /**
     * Get the reference of the Lua state.
     */
    static State& getLua();

    /**
     * Initialize the Lua runtime.
     * Must be called before calling other functions.
     */
    static void init();

    /**
     * Load and execute a Lua script file.
     * @param path The path of the Lua script.
     * @param throws Whether to throw LuaException if the execution fails.
     * @return true if the execution succeeds, false otherwise.
     */
    static bool runFile(const string& path, const bool& throws = true);

    /**
     * Load and execute a Lua code from a string.
     * @param content The content of the script.
     * @param throws Whether to throw LuaException if the execution fails.
     * @return true if the execution succeeds, false otherwise.
     */
    static bool run(const string& content, const bool& throws = true);
};

#endif //THEENGINE_LUARUNTIME_H
