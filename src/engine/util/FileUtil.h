//
// Created by mcumbrella on 23-10-27.
//

#ifndef THEENGINE_FILEUTIL_H
#define THEENGINE_FILEUTIL_H

#include <string>

using std::string;

class FileUtil
{
public:
    static bool exists(const string& path);

    static bool isFile(const string& path);

    static bool isDir(const string& path);

    static string readStr(const string& path);

    static string readStr(const string& path, const string& fallback);
};

#endif //THEENGINE_FILEUTIL_H
