#ifndef FONTLOADER_HPP
#define FONTLOADER_HPP

#include <string>
#include <map>
#include <vector>

#include "FileBrowser.hpp"
#include "FontTexture.hpp"

namespace hpl
{
class FontLoader
{

public:
    FontLoader();
    ~FontLoader();

    inline void addSearchPath(const std::string& path) {
        fileBrowser.addSearchPath(path);
    }

    FontTexture* getFont(const std::string& name);

private:
    FontTexture* findExistingFont(const std::string& name);

    FileBrowser fileBrowser;
    std::vector<std::pair<std::string, FontTexture*> > fonts;

};
}

#endif // FONTLOADER_HPP
