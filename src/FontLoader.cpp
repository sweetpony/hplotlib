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
        FontTexture* ft = *it;
        //! @todo this has to be done in ogl thread
        if (ft->isInitialised()) {
            ft->destroy();
        }
        delete ft;
    }
}

FontTexture* FontLoader::getFont(const std::string& name)
{
    FontTexture* ft = findExistingFont(name);
    if (ft != nullptr) {
        //! @todo reuse or create new??
        return new FontTexture(*it);
    } else {
        std::string path = fileBrowser.getFontPath(name);
        FontTexture* ft = new FontTexture(path);
        fonts.push_back(std::pair<std::string, FontTexture*>(name, ft));
        return ft;
    }
}

FontTexture* FontLoader::findExistingFont(const std::string& name)
{
    for (auto it = fonts.begin(); it != fonts.end(); ++it) {
        if (it->first == name) {
            return it->second;
        }
    }
    return nullptr;
}

}
