//
// Created by mcumbrella on 23-10-27.
//

#include "FileUtil.h"
#include "../exception/IOException.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <cstring>

namespace fs = std::filesystem;

bool FileUtil::exists(const string& path)
{
    return fs::exists(path);
}

bool FileUtil::isFile(const string& path)
{
    return fs::is_regular_file(path);
}

bool FileUtil::isDir(const string& path)
{
    return fs::is_directory(path);
}

string FileUtil::readStr(const string& path)
{
    std::ifstream f(path);
    if(!f.is_open())
        throw IOException("Failed to read file \"" + path + "\": " + strerror(errno));
    std::stringstream buf;
    buf << f.rdbuf();
    f.close();
    return buf.str();
}

string FileUtil::readStr(const string& path, const string& fallback)
{
    try
    {
        return readStr(path);
    }
    catch(...)
    {
        return fallback;
    }
}
