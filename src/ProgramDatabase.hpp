#ifndef PROGRAMDATABASE_HPP
#define PROGRAMDATABASE_HPP

#include <iostream>
#include <GL/glld.h>

namespace hpl
{
class ProgramDatabase
{

public:
    ProgramDatabase();

    void init();
    void destroy();

    inline GLuint getLineProgram() const {
        return lineProgram;
    }
    inline GLuint getTextProgram() const {
        return textProgram;
    }

private:
    GLuint compileLineVertexShader() const;
    GLuint compileLineFragmentShader() const;
    GLuint compileTextFragmentShader() const;

    GLuint linkLineProgram(GLuint vertexShader, GLuint fragmentShader) const;
    GLuint linkTextProgram(GLuint vertexShader, GLuint fragmentShader) const;

    void showCompileLog(GLuint id) const;
    void showLinkLog(GLuint id) const;

    GLuint lineVertexShader, lineFragmentShader, textFragmentShader;
    GLuint lineProgram, textProgram;

};
}

#endif // PROGRAMDATABASE_HPP
