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
        ft->destroy();
        delete ft;
    }
}

void FontLoader::deleteTextures()
{
    for (auto it = fonts.begin(); it != fonts.end(); ++it) {
        it->second->destroy();
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

}
