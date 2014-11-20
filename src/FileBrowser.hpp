#ifndef FILEBROWSER_HPP
#define FILEBROWSER_HPP

#include <unordered_map>
#include <string>

#include "Dir.hpp"
#include "Font.hpp"

namespace hpl
{
class FileBrowser
{

public:
    FileBrowser();

    void addSearchPath(const std::string& path);

    inline std::string getFontPath(const std::string& name) {
        return fontPaths[name];
    }

private:
    bool hasFileExtension(const std::string& file, const std::string& ext);
    std::string getFileName(const std::string& file);
    std::string& appendSystemPathSeperator(string& path);

    std::unordered_map<std::string, std::string> fontPaths;

};
}

#endif // FILEBROWSER_HPP
