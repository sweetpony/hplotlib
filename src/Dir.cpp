#include "Dir.hpp"

namespace hpl
{
vector<string> Dir::list(int flags)
{
    vector<string> entries;    
#ifdef _WIN32
    char path[2048];
    sprintf(path, "%s/*", _path.c_str());
    
    WIN32_FIND_DATA data;
    HANDLE hfind = INVALID_HANDLE_VALUE;
    DWORD err;

    auto checkAndAdd = [&entries, &flags] (WIN32_FIND_DATA data) {
        if (data.cFileName[0] != '.'
                && (!(flags & Dirs) || (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                && (!(flags & Files) || !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))) {
            entries.push_back(data.cFileName);
        }
    };

    hfind = FindFirstFile(path, &data);
    checkAndAdd(data);

    if (hfind != INVALID_HANDLE_VALUE) {
        while (FindNextFile(hfind, &data) != 0) {
            checkAndAdd(data);
        }
    }

    FindClose(hfind);

#elif __linux__
    DIR *dir = opendir(_path.c_str());
    struct dirent *item = NULL;
    if (dir != NULL) {
        while ((item = readdir(dir))) {
            if (item->d_name[0] != '.'
                && (!(flags & Dirs) || (item->d_type & DT_DIR))
                && (!(flags & Files) || !(item->d_type & DT_DIR))) {
                entries.push_back(item->d_name);
            }
        }
        closedir(dir);
    }
#endif

    return entries;
}

bool Dir::isAbsolutePath()
{
#ifdef _WIN32
    return _path.size() >= 2 &&
           _path[1] == ':' &&
           ((_path[0] >= 'A' && _path[0] <= 'Z') ||
           (_path[0] >= 'a' && _path[0] <= 'z'));
#elif __linux__
    return (_path.size() >= 1) && _path[0] == '/';
#endif
}
}
