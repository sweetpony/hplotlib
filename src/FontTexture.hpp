#ifndef HPLOTLIB_FONTTEXTURE_HPP
#define HPLOTLIB_FONTTEXTURE_HPP

#include "GL/glld.h"
#include "FileBrowser.hpp"

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
    FontTexture();

    void read(std::string const& fontname);
    void init(std::string const& fontname);
	void destroy();    
    void bind(GLint position, GLuint textureUnit);
    
    inline Header const& header() const { return _header; }
    inline Char const& ch(unsigned char id) const { return _chars[id]; }

private:
    static FileBrowser fb;
    static bool firstInstantiation;

    GLuint _glyphs;
    Header _header = {0};
    Char _chars[128];
};
}

#endif
