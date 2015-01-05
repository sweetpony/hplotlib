#ifndef ENGINE_UTILITY_DIR_H_
#define ENGINE_UTILITY_DIR_H_

#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#include <cstdio>
#elif __linux__
#include <dirent.h>
#endif

using std::string;
using std::vector;

namespace hpl
{
class Dir
{
public:
    enum {
        Dirs = 0x01,
        Files = 0x02
    };

    Dir(const string& path) {
        _path = path;
    }
    vector<string> list(int flags = 0x00);

    bool isAbsolutePath();

private:
    string _path;
};
}

#endif

