#include "Font.hpp"
#include <cstdio>

namespace plot {
void Font::init(std::string const& path)
{
    FILE* in = fopen(path.c_str(), "rb");
    fread(&_header, sizeof(Header), 1, in);
    fread(_chars, sizeof(Char), _header.count, in);

    unsigned texels = _header.width * _header.height;
    unsigned char data[texels];
    fread(data, sizeof(unsigned char), texels, in);
    fclose(in);
    
    for (int i = _header.count-1; i >= 0; --i) {
		char id = _chars[i].id;
		_chars[id] = _chars[i];
	}

    glGenTextures(1, &_glyphs);

    glBindTexture(GL_TEXTURE_2D, _glyphs);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY8, _header.width, _header.height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Font::destroy()
{
    glDeleteTextures(1, &_glyphs);
}

void Font::bind(GLint position, GLuint textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, _glyphs);
    glUniform1i(position, textureUnit);
}
}
