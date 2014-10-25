/*
 * Lines.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "Lines.hpp"
#include "Statistics.hpp"

namespace hpl
{
Lines::Lines(int n, double const* x, double const* y, Geometry* geometry)
    : PlotPart(geometry), n(n)
{
	double xmin = hpl::min(n, x);
	double xmax = hpl::max(n, x);
	double ymin = hpl::min(n, y);
	double ymax = hpl::max(n, y);
	
    interleave = new float[2*n];
    for (int i = 0; i < n; ++i) {
		interleave[(i << 1)] = (x[i] - xmin) / (xmax - xmin);
		interleave[(i << 1) + 1] = (y[i] - ymin) / (ymax - ymin);
	}
}

Lines::~Lines()
{
}
	
void Lines::init(GLuint lineprogram, GLuint)
{
	program = lineprogram;

    glGenBuffers(1, &lineBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * n * sizeof(float), interleave, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	delete[] interleave;
    
	pos = glGetAttribLocation(program, "Position");
	rect = glGetUniformLocation(program, "Rect");
	color = glGetUniformLocation(program, "Color");
	linemvp = glGetUniformLocation(program, "MVP");
}

void Lines::destroy()
{
    glDeleteBuffers(1, &lineBuffer);
}

void Lines::draw(float const* mvp)
{
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
	glVertexAttribPointer(
		pos,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(GLvoid const*) 0
	);
	glEnableVertexAttribArray(pos);
    glUniform4f(rect, geometry->leftOffset, geometry->topOffset, geometry->width, geometry->height);
    glUniform3f(color, drawColor.r, drawColor.g, drawColor.b);
	glUniformMatrix3fv(linemvp, 1, GL_FALSE, mvp);
	
	glDrawArrays(GL_LINE_STRIP, 0, n);
	glDisableVertexAttribArray(pos);
}
}
