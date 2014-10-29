/*
 * Canvas.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "Canvas.hpp"
#include <GL/glld.h>
#include <cmath>
#include <iostream>

namespace hpl
{
void showCompileLog(GLuint id)
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

void showLinkLog(GLuint id)
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

Canvas::~Canvas()
{
}

Canvas::Slot* Canvas::findSlot(Slot const& slot)
{
	for (auto it = racks.begin(); it != racks.end(); ++it) {
		auto f = std::find(it->second.slots.begin(), it->second.slots.end(), slot);
		if (f != it->second.slots.end()) {
			return &(*f);
		}
	}
	
	return nullptr;
}

void Canvas::addSlotToLayout(Slot const& slot, Layout::ID to)
{
	Slot* last = findSlot(slot);
	if (last != nullptr) {
		last->invalidate();
	}
	Rack& rack = racks[to];
	rack.slots.push_back(slot);
	rack.geometries.push_back(Geometry());
	
	recalculateLayout(to);
}

void Canvas::recalculateLayout(Layout::ID layout)
{
	Rack& rack = racks[layout];
	layouts.lookup(layout).recalculate(rack.geometries);
	
	auto s = rack.slots.cbegin();
	auto g = rack.geometries.cbegin();
	for (; s != rack.slots.cend() && g != rack.geometries.cend(); ++s, ++g) {
		if (s->plot.valid()) {
			plots.lookup(s->plot).setGeometry(*g);
		}
	}
	
	update();
}

bool Canvas::saveToFile(const std::string& fileName)
{
    /*PostscriptPrinter p;

    pthread_mutex_lock(&mutex);
    std::vector<Plot*> pl = plots;
    pthread_mutex_unlock(&mutex);

    p.saveToFile(fileName, pl);*/
    return true;
}
	
void Canvas::init()
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    font.init(fontFile);
    
	linevshader = glCreateShader(GL_VERTEX_SHADER);
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
    
    linefshader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar const* fsource =
		"#version 120\n"
		"uniform vec3 Color;\n"
		"void main(void) {\n"
		"gl_FragColor = vec4(Color, 1.0);\n"
		"}\n";
    glShaderSource(linefshader, 1, &fsource, NULL);
    glCompileShader(linefshader);
    showCompileLog(linefshader);

    lineprogram = glCreateProgram();
    glAttachShader(lineprogram, linevshader);
    glAttachShader(lineprogram, linefshader);
    glLinkProgram(lineprogram);
    showLinkLog(lineprogram);
    
    textfshader = glCreateShader(GL_FRAGMENT_SHADER);
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

    textprogram = glCreateProgram();
    glAttachShader(textprogram, linevshader);
    glAttachShader(textprogram, textfshader);
    glLinkProgram(textprogram);
    showLinkLog(textprogram);
}
	
void Canvas::destroy()
{
	pthread_mutex_lock(&mutex);
	for (auto it = plots.cbegin(); it != plots.cend(); ++it) {
		it->second->destroy();
	}
	pthread_mutex_unlock(&mutex);
	
	glDeleteShader(linevshader);
    glDeleteShader(linefshader);
    glDeleteProgram(lineprogram);

    glDeleteShader(textfshader);
    glDeleteProgram(textprogram);
}

void Canvas::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pthread_mutex_lock(&mutex);
	while (!plotInit.empty()) {
		Plot::ID id = plotInit.front();
		plotInit.pop();
		if (plots.has(id)) {
			plots.lookup(id).init(lineprogram, textprogram);
		}
	}
		
	for (auto it = plots.cbegin(); it != plots.cend(); ++it) {
		it->second->draw(mvp);
	}
	pthread_mutex_unlock(&mutex);
}

void Canvas::resetEvent()
{
	mvp[0] = 1.0;
	mvp[4] = 1.0;
	mvp[6] = 0.0;
	mvp[7] = 0.0;
}

void Canvas::moveEvent(int deltax, int deltay)
{
	mvp[6] += deltax / static_cast<double>(width);
	mvp[7] += -deltay / static_cast<double>(height);
}

void Canvas::mouseWheelEvent(int x, int y, double delta)
{
	delta *= 0.2f;
	double old = mvp[0];
	mvp[0] += delta;
	mvp[4] += delta;
	if (mvp[0] < 0.5f) {
		mvp[0] = 0.5f;
	}
	if (mvp[4] < 0.5f) {
		mvp[4] = 0.5f;
	}
	double xs = (x / static_cast<double>(width) - mvp[6]) / old;
	double ys = (1.0 - y / static_cast<double>(height) - mvp[7]) / old;
	mvp[6] += (old-mvp[0]) * xs;
	mvp[7] += (old-mvp[4]) * ys;
}

}
