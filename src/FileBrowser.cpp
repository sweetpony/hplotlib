#include "FileBrowser.hpp"

namespace hpl
{
FileBrowser::FileBrowser()
{
}

void FileBrowser::addSearchPath(const std::string& path)
{
    Dir dir(path);
    std::vector<std::string> fileList = dir.list(Dir::Files);
    for (auto it = fileList.begin(); it != fileList.end(); ++it) {
        if (hasFileExtension(*it, "font")) {
            std::string s(path);
            fontPaths[getFileName(*it)] = appendSystemPathSeperator(s).append(*it);
        }
    }
}

bool FileBrowser::hasFileExtension(const std::string& file, const std::string& ext)
{
    auto n = file.crbegin();
    auto e = ext.crbegin();
    while (n != file.crend() && e != ext.crend()) {
        if (tolower(*n) != tolower(*e)) {
            return false;
        }
        ++n;
        ++e;
    }
    return e == ext.crend();

}

std::string FileBrowser::getFileName(const std::string& file)
{
    int n = 0;
     auto it = file.crbegin();
     while (it != file.crend()) {
         ++n;
         if (*it == '.') {
             break;
         }
         ++it;
     }
     return file.substr(0, file.length() - n);
}

std::string& FileBrowser::appendSystemPathSeperator(string& path) {
    char lastChar = *path.rbegin();
    char seperator = '/';
    if (lastChar != seperator) {
        path += seperator;
    }
    return path;
}
}
