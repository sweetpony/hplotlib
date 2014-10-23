/*
 * Lines.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "Legend.hpp"
#include "Statistics.hpp"

#include <GL/glld.h>
#include <cstdio>
#include <algorithm>

namespace hpl
{
Legend::Legend(Font* font, int n, double const* x, double const* y, float left, float top, float width, float height)
	: font(font), PlotPart(left, top, width, height)
{
	xmin = hpl::min(n, x);
	xmax = hpl::max(n, x);	
	ymin = hpl::min(n, y);
	ymax = hpl::max(n, y);
}
	
void Legend::init(GLuint lineprogram, GLuint textprogram)
{
	this->lineprogram = lineprogram;
	this->textprogram = textprogram;
	
	float lines[8 + 2*Ticks*4] = {
		XOffset, 1.0f,
		XOffset, YOffset,
		XOffset, YOffset,
		1.0f, YOffset
	};	
    
    struct Label {
		char label[16];
		int len;
		float x, y;
	};
    
    Label labels[2*Ticks];
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

    glGenBuffers(1, &lineBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
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
			++c;
		}
	}

    glGenBuffers(1, &textBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
	glBufferData(GL_ARRAY_BUFFER, numChars*4*4*sizeof(float), text, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    delete[] text;
    
    linepos = glGetAttribLocation(lineprogram, "Position");
    linerect = glGetUniformLocation(lineprogram, "Rect");
	linecolor = glGetUniformLocation(lineprogram, "Color");
	linemvp = glGetUniformLocation(lineprogram, "MVP");

    textpos = glGetAttribLocation(textprogram, "Position");
    textuv = glGetAttribLocation(textprogram, "UV");
    textrect = glGetUniformLocation(textprogram, "Rect");
    textglyphs = glGetUniformLocation(textprogram, "Glyphs");
	textmvp = glGetUniformLocation(textprogram, "MVP");

}

void Legend::destroy()
{
	glDeleteBuffers(1, &lineBuffer);
	glDeleteBuffers(1, &textBuffer);
}

void Legend::draw(float const* mvp)
{
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
	glUniform4f(linerect, left, top, width, height);
    glUniform3f(linecolor, drawColor.r, drawColor.g, drawColor.b);
	glUniformMatrix3fv(linemvp, 1, GL_FALSE, mvp);
	
	glDrawArrays(GL_LINES, 0, 4 + 4*Ticks);
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
		4*sizeof(float),
		(GLvoid const*) (2*sizeof(float))
	);
	glEnableVertexAttribArray(textuv);
	glUniform4f(textrect, left, top, width, height);	
	glUniformMatrix3fv(textmvp, 1, GL_FALSE, mvp);
	font->bind(textglyphs, 0);
	
	glDrawArrays(GL_QUADS, 0, 4*numChars);
	glDisableVertexAttribArray(textpos);
	glDisableVertexAttribArray(textuv);
}
}
