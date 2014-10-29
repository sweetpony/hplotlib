#include "ProgramDatabase.hpp"

namespace hpl
{
ProgramDatabase::ProgramDatabase()
{
}

void ProgramDatabase::init()
{
    lineVertexShader = compileLineVertexShader();
    lineFragmentShader = compileLineFragmentShader();
    textFragmentShader = compileTextFragmentShader();

    lineProgram = linkLineProgram(lineVertexShader, lineFragmentShader);
    textProgram = linkTextProgram(lineVertexShader, textFragmentShader);
}

void ProgramDatabase::destroy()
{
    glDeleteShader(lineVertexShader);
    glDeleteShader(lineFragmentShader);
    glDeleteShader(textFragmentShader);

    glDeleteProgram(lineProgram);
    glDeleteProgram(textProgram);
}

GLuint ProgramDatabase::compileLineVertexShader() const
{
    GLuint linevshader = glCreateShader(GL_VERTEX_SHADER);
    GLchar const* vsource =
        "#version 120\n"
        "attribute vec2 Position;\n"
        "attribute vec2 UV;\n"
        "uniform vec4 Rect;\n"
        "uniform mat3 MVP;\n"
        "varying vec2 vUV;\n"
        "void main(void) {\n"
        "vec3 pos = MVP*vec3(Rect.zw * Position + Rect.xy, 1.0);\n"
        "gl_Position = vec4(2.0 * pos.xy - vec2(1.0), 0.0, 1.0);\n"
        "vUV = UV;\n"
        "}\n";

    glShaderSource(linevshader, 1, &vsource, NULL);
    glCompileShader(linevshader);
    showCompileLog(linevshader);

    return linevshader;
}

GLuint ProgramDatabase::compileLineFragmentShader() const
{
    GLuint linefshader = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar const* fsource =
        "#version 120\n"
        "uniform vec3 Color;\n"
        "void main(void) {\n"
        "gl_FragColor = vec4(Color, 1.0);\n"
        "}\n";

    glShaderSource(linefshader, 1, &fsource, NULL);
    glCompileShader(linefshader);
    showCompileLog(linefshader);

    return linefshader;
}

GLuint ProgramDatabase::compileTextFragmentShader() const
{
    GLuint textfshader = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar const* textfsource =
        "#version 120\n"
        "uniform sampler2D Glyphs;\n"
        "uniform vec3 Color;\n"
        "varying vec2 vUV;\n"
        "float contour(in float d, in float w) {\n"
        "return smoothstep(0.5 - w, 0.5 + w, d);\n"
        "}\n"
        "float samp(in vec2 uv, in float w) {\n"
        "return contour(texture2D(Glyphs, uv).a, w);\n"
        "}\n"
        "void main(void) {\n"
        "float dist = texture2D(Glyphs, vUV).a;\n"
        "float width = fwidth(dist);\n"
        "float alpha = contour(dist, width);\n"
        "float dscale = 0.254;\n"
        "vec2 duv = dscale * (dFdx(vUV) + dFdy(vUV));\n"
        "vec4 box = vec4(vUV-duv, vUV+duv);\n"
        "float asum = samp(box.xy, width) + samp(box.zw, width) + samp(box.xw, width) + samp(box.zy, width);\n"
        "alpha = (alpha + 0.5 * asum) / 3.0;\n"
        "gl_FragColor = vec4(Color, alpha);\n"
        "}\n";

    glShaderSource(textfshader, 1, &textfsource, NULL);
    glCompileShader(textfshader);
    showCompileLog(textfshader);

    return textfshader;
}

GLuint ProgramDatabase::linkLineProgram(GLuint vertexShader, GLuint fragmentShader) const
{
    GLuint lineprogram = glCreateProgram();

    glAttachShader(lineprogram, vertexShader);
    glAttachShader(lineprogram, fragmentShader);
    glLinkProgram(lineprogram);
    showLinkLog(lineprogram);

    return lineprogram;
}

GLuint ProgramDatabase::linkTextProgram(GLuint vertexShader, GLuint fragmentShader) const
{
    GLuint textprogram = glCreateProgram();

    glAttachShader(textprogram, vertexShader);
    glAttachShader(textprogram, fragmentShader);
    glLinkProgram(textprogram);
    showLinkLog(textprogram);

    return textprogram;
}

void ProgramDatabase::showCompileLog(GLuint id) const
{
    GLint compileStatus;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint logLength;
        GLchar* log;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        log = new GLchar[logLength+1];
        log[logLength] = '\0';
        glGetShaderInfoLog(id, logLength, NULL, log);
        std::cout << log << std::endl;
        delete[] log;
    }
}

void ProgramDatabase::showLinkLog(GLuint id) const
{
    GLint compileStatus;
    glGetProgramiv(id, GL_LINK_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint logLength;
        GLchar* log;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
        log = new GLchar[logLength + 1];
        log[logLength] = '\0';
        glGetProgramInfoLog(id, logLength, NULL, log);
        std::cout << log << std::endl;
        delete[] log;
    }
}
}
