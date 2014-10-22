#ifndef HPLOTLIB_FONT_HPP
#define HPLOTLIB_FONT_HPP

#include <string>
#include <GL/glld.h>

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
	char id;
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
    inline Char const& ch(char id) const { return _chars[id]; }

private:  
    GLuint _glyphs;
    Header _header = {0};
    Char _chars[128];
};
}

#endif
