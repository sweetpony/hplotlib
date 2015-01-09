#ifndef FONTLOADER_HPP
#define FONTLOADER_HPP

#include <string>
#include <map>

#include "FileBrowser.hpp"
#include "FontTexture.hpp"

namespace hpl
{
class FontLoader
{

public:
    FontLoader();
    ~FontLoader();

    void deleteTextures();

    inline void addSearchPath(const std::string& path) {
        fileBrowser.addSearchPath(path);
    }

    FontTexture* getFont(const std::string& name);

private:
    FileBrowser fileBrowser;
    std::map<std::string, FontTexture*> fonts;

};
}

#endif // FONTLOADER_HPP
