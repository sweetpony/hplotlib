#include "FontTexture.hpp"
#include <cstdio>

namespace hpl {

FontTexture::FontTexture(std::string const& path) : path(path)
{
    FILE* in = fopen(path.c_str(), "rb");
    fread(&_header, sizeof(Header), 1, in);
    fread(_chars, sizeof(Char), _header.count, in);

    unsigned texels = _header.width * _header.height;
    data = new unsigned char[texels];
    fread(data, sizeof(unsigned char), texels, in);
    fclose(in);

    for (int i = _header.count-1; i >= 0; --i) {
        unsigned char id = _chars[i].id;
        _chars[id] = _chars[i];
    }
}

FontTexture::~FontTexture()
{
    delete[] data;
}

//! @todo make sure this also works if two OGLPlotter are used, nescessary?
void FontTexture::init()
{
    if (!initialised) {
        glGenTextures(1, &_glyphs);

        glBindTexture(GL_TEXTURE_2D, _glyphs);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY8, _header.width, _header.height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        initialised = true;
    }
}

void FontTexture::destroy()
{
    if (initialised) {
        glDeleteTextures(1, &_glyphs);

        initialised = false;
    }
}

void FontTexture::bind(GLint position, GLuint textureUnit)
{
    if (initialised) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, _glyphs);
        glUniform1i(position, textureUnit);
    }
}
}
