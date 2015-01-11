#include "FontLoader.hpp"

namespace hpl
{

FontLoader::FontLoader()
{
    fileBrowser.addSearchPath("../fonts/");
}

FontLoader::~FontLoader()
{
    for (auto it = fonts.begin(); it != fonts.end(); ++it) {
        FontTexture* ft = it->second;
        if (! ft->isInitialised()) {
            delete ft;
        }
    }
}

FontTexture* FontLoader::getFont(const std::string& name)
{
    auto it = fonts.find(name);
    if (it != fonts.end()) {
        return it->second;
    } else {
        std::string path = fileBrowser.getFontPath(name);
        FontTexture* ft = new FontTexture(path);
        fonts[name] = ft;
        return ft;
    }
}

void FontLoader::deleteFont(const std::string& name)
{
    auto it = fonts.find(name);
    if (it != fonts.end()) {
        FontTexture* ft = it->second;
        fonts.erase(it);
        ft->destroy();
        delete ft;
    }
}

}
