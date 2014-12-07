#ifndef HPLOTLIB_FONT_HPP
#define HPLOTLIB_FONT_HPP

#include "GL/glld.h"

#include <string>

namespace hpl
{
struct Header {
    Header() : lineHeight(0.0), base(0.0), count(0u), width(0u), height(0u) {}

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
class Font
{
public:    
	void init(std::string const& path);
	void destroy();    
    void bind(GLint position, GLuint textureUnit);
    
    inline Header const& header() const { return _header; }
    inline Char const& ch(unsigned char id) const { return _chars[id]; }

private:  
    GLuint _glyphs;
    Header _header;
    Char _chars[128];
};
}

#endif
