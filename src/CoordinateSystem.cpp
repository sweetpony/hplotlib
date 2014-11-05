/*
 * Lines.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "CoordinateSystem.hpp"
#include <cstdio>
#include <algorithm>
#include "Statistics.hpp"

namespace hpl
{
CoordinateSystem::CoordinateSystem(Registry<Drawable>& dataContainer)
    : data(dataContainer), drawColor(0.0f, 0.0f, 0.0f)
{
}

CoordinateSystem::~CoordinateSystem()
{
    //delete[] lines;
    //delete[] labels;
}

/*float* CoordinateSystem::getLines() const
{
    const unsigned int m = getLinesCount();
    float* l = new float[m];

    for (unsigned int i = 0; i < m-1; i+=2) {
        l[i] = lines[i] * geometry.width + geometry.leftOffset;
        l[i+1] = lines[i+1] * geometry.height + geometry.topOffset;
    }

    return l;
}

CoordinateSystem::Label* CoordinateSystem::getLabels() const
{
    const unsigned int m = getLabelsCount();
    Label* l = new Label[m];

    for (unsigned int i = 0; i < m; i++) {
        l[i].x = labels[i].x * geometry.width + geometry.leftOffset;
        l[i].y = labels[i].y * geometry.height + geometry.topOffset;
        for (unsigned int j = 0; j < 16; j++) {
            l[i].label[j] = labels[i].label[j];
        }
        l[i].len = labels[i].len;
        l[i].width = labels[i].width * geometry.width;
        l[i].height = labels[i].height * geometry.height;
    }

    return l;
}*/

void CoordinateSystem::setColor(const Color& c)
{
    drawColor = c;
    changed.invoke();
}

void CoordinateSystem::setGeometry(Geometry geom)
{
	geometry = geom;

	geom.leftOffset += XOffset * geom.width;
	geom.topOffset += YOffset * geom.height;
	geom.width *= (1.0 - XOffset);
	geom.height *= (1.0 - YOffset);
    for (auto it = myPlots.begin(); it != myPlots.end(); ++it) {
        data.lookup(*it).setGeometry(geom);
	}
	changed.invoke();
}

void CoordinateSystem::updateLimits(double xmin, double xmax, double ymin, double ymax)
{
    this->xmin = std::min(this->xmin, xmin);
    this->xmax = std::min(this->xmax, xmax);
    this->ymin = std::min(this->ymin, ymin);
    this->ymax = std::min(this->ymax, ymax);

    for (auto it = myPlots.begin(); it != myPlots.end(); ++it) {
        data.lookup(*it).setLimits(xmin, ymin, xmax, ymax);
    }

    updateLabels = true;
    needLimitUpdate = false;
    changed.invoke();
}
	
/*void CoordinateSystem::init(GLuint lineprogram, GLuint textprogram, GLuint mapprogram)
{
    while (!plotInit.empty()) {
		Plot::ID id = plotInit.front();
		plotInit.pop();
		if (plots.has(id)) {
            Plot* p = &plots.lookup(id);
            if (dynamic_cast<Contour*>(p) == 0) {
                p->init(lineprogram, textprogram);
            } else {
                p->init(mapprogram, textprogram);
            }
		}
	}

	this->lineprogram = lineprogram;
	this->textprogram = textprogram;
    this->mapprogram = mapprogram;
	
    glGenBuffers(1, &lineBuffer);
    glGenBuffers(1, &textBuffer);
    
    linepos = glGetAttribLocation(lineprogram, "Position");
    linerect = glGetUniformLocation(lineprogram, "Rect");
	linecolor = glGetUniformLocation(lineprogram, "Color");
	linemvp = glGetUniformLocation(lineprogram, "MVP");

    textpos = glGetAttribLocation(textprogram, "Position");
    textuv = glGetAttribLocation(textprogram, "UV");
    textrect = glGetUniformLocation(textprogram, "Rect");
    textglyphs = glGetUniformLocation(textprogram, "Glyphs");
	textmvp = glGetUniformLocation(textprogram, "MVP");
    textcolor = glGetUniformLocation(textprogram, "Color");
}

void CoordinateSystem::update()
{	
    delete[] lines;
    lines = new float[8 + 2*Ticks*4]{
		XOffset, 1.0f,
		XOffset, YOffset,
		XOffset, YOffset,
		1.0f, YOffset
    };

    delete[] labels;
    labels = new Label[2*Ticks];
    numChars = 0;
	
	float xspacing = (1.0 - XOffset) / (Ticks + 1.0f);
	for (int i = 0; i < Ticks; ++i) {
		float x = XOffset + (i+1) * xspacing;
		lines[8 + 4*i] = x;
		lines[8 + 4*i + 1] = YOffset - TickLength / 2.0f;
		lines[8 + 4*i + 2] = x;
		lines[8 + 4*i + 3] = YOffset + TickLength / 2.0f;
		
		labels[i].len = sprintf(labels[i].label, "%.2f", (i+1) / (Ticks + 1.0f) * (xmax-xmin) + xmin);
		numChars += labels[i].len;
		labels[i].x = x;
		labels[i].y = YOffset / 2.0;
	}
	
	for (int i = Ticks; i < 2*Ticks; ++i) {
		float y = YOffset + (1.0 - YOffset) * (i-Ticks+1) / (Ticks + 1.0f);
		lines[8 + 4*i] = XOffset + TickLength / 2.0f;
		lines[8 + 4*i + 1] = y;
		lines[8 + 4*i + 2] = XOffset - TickLength / 2.0f;
		lines[8 + 4*i + 3] = y;
		
		labels[i].len = sprintf(labels[i].label, "%.2f", (i-Ticks+1) / (Ticks + 1.0f) * (ymax-ymin) + ymin);
		numChars += labels[i].len;
		labels[i].x = XOffset / 2.0;
		labels[i].y = y;
	}
	
	numLines =  4 + 4*Ticks;

    glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * numLines * sizeof(float), lines, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    float* text = new float[numChars*4*4];
	float maxTextWidth = std::min(XOffset-TickLength, 0.8f*xspacing);
	float maxTextHeight = YOffset-TickLength;
	
	Header header = font->header();
	int c = 0;
	for (int i = 0; i < 2*Ticks; ++i) {
		float textw = 0.0;
		for (int j = 0; j < labels[i].len; ++j) {			
            textw += font->ch(labels[i].label[j]).xadvance;
		}
		float xscale = maxTextWidth / textw;
		float yscale = maxTextHeight / header.lineHeight;
		float scale = (xscale < yscale) ? xscale : yscale;
		float xadv = 0.0;
		for (int j = 0; j < labels[i].len; ++j) {
			Char ch = font->ch(labels[i].label[j]);
			float x = labels[i].x + scale * (xadv + ch.xoffset - textw / 2.0);
			float y = labels[i].y + scale * (header.lineHeight / 2.0 - ch.yoffset);
			xadv += ch.xadvance;
			text[4*4*c + 0] = x;
			text[4*4*c + 1] = y - ch.height*scale;
			text[4*4*c + 2] = ch.x / header.width;		
			text[4*4*c + 3] = (ch.y + ch.height) / header.height;
			
			text[4*4*c + 4] = x;
			text[4*4*c + 5] = y;
			text[4*4*c + 6] = ch.x / header.width;
			text[4*4*c + 7] = ch.y / header.height;
			
			text[4*4*c + 8] = x + ch.width*scale;
			text[4*4*c + 9] = y;
			text[4*4*c + 10] = (ch.x + ch.width) / header.width;
			text[4*4*c + 11] = ch.y / header.height;
			
			text[4*4*c + 12] = x + ch.width*scale;
			text[4*4*c + 13] = y - ch.height*scale;
			text[4*4*c + 14] = (ch.x + ch.width) / header.width;
			text[4*4*c + 15] = (ch.y + ch.height) / header.height;

            labels[i].width += ch.xadvance + ch.xoffset;
            labels[i].height += ch.height;
			++c;
        }
        labels[i].width *= scale;
        labels[i].height *= scale / labels[i].len;
	}

    glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
	glBufferData(GL_ARRAY_BUFFER, numChars*4*4*sizeof(float), text, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] text;
}

void CoordinateSystem::destroy()
{
    /*for (auto i = plots.begin(); i != plots.end(); i++) {
        i->second->destroy();
    }
	glDeleteBuffers(1, &lineBuffer);
    glDeleteBuffers(1, &textBuffer);
}

void CoordinateSystem::draw(float const* mvp)
{
    if (updateLabels) {
		updateLabels = false;
		update();
	}
	
	for (auto i = plots.begin(); i != plots.end(); i++) {
        i->second->draw(mvp);
    }
    
    glUseProgram(lineprogram);
    glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
	glVertexAttribPointer(
		linepos,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(GLvoid const*) 0
	);
	glEnableVertexAttribArray(linepos);
    glUniform4f(linerect, geometry.leftOffset, geometry.topOffset, geometry.width, geometry.height);
    glUniform3f(linecolor, drawColor.r, drawColor.g, drawColor.b);
	glUniformMatrix3fv(linemvp, 1, GL_FALSE, mvp);
	
	glDrawArrays(GL_LINES, 0, numLines);
	glDisableVertexAttribArray(linepos);
	
    glUseProgram(textprogram);    
    glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
	glVertexAttribPointer(
		textpos,
		2,
		GL_FLOAT,
		GL_FALSE,
		4*sizeof(GLfloat),
		(GLvoid const*) 0
	);
	glEnableVertexAttribArray(textpos);
	glVertexAttribPointer(
		textuv,
		2,
		GL_FLOAT,
		GL_FALSE,
        4*sizeof(GLfloat),
		(GLvoid const*) (2*sizeof(float))
	);
	glEnableVertexAttribArray(textuv);
    glUniform4f(textrect, geometry.leftOffset, geometry.topOffset, geometry.width, geometry.height);
	glUniformMatrix3fv(textmvp, 1, GL_FALSE, mvp);
    glUniform3f(textcolor, drawColor.r, drawColor.g, drawColor.b);
	font->bind(textglyphs, 0);
	
	glDrawArrays(GL_QUADS, 0, 4*numChars);
	glDisableVertexAttribArray(textpos);
    glDisableVertexAttribArray(textuv);
}*/

void CoordinateSystem::addNewPlot(Drawable::ID id)
{
    plotInit.push(id);
    myPlots.push_back(id);
    setGeometry(geometry);
    changed.invoke();
}
}
