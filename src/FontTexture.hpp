#ifndef HPLOTLIB_FONTTEXTURE_HPP
#define HPLOTLIB_FONTTEXTURE_HPP

#include "GL/glld.h"

#include <string>

namespace hpl
{
struct Header {
	float lineHeight;
	float base;
	unsigned count;	
	unsigned width;
	unsigned height;
};

struct Char {
    unsigned char id;
	float x;
	float y;
	float width;
	float height;
	float xoffset;
	float yoffset;
	float xadvance;
};
class FontTexture
{
public:
    FontTexture(std::string const& path);
    ~FontTexture();

    bool init();
    void destroy();
    void bind(GLint position, GLuint textureUnit);
    
    inline Header const& header() const { return _header; }
    inline Char const& ch(unsigned char id) const { return _chars[id]; }

    inline bool isInitialised() {
        return initialised;
    }

private:
    GLuint _glyphs;
    Header _header = {0};
    Char _chars[128];
    unsigned char* data = nullptr;

    std::string const& path;
    bool initialised = false;
};
}

#endif
