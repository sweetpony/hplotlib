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
CoordinateSystem::CoordinateSystem(Registry<Drawable>& dataContainer, std::map<Drawable::ID, unsigned int>& dataRevisions)
    : data(dataContainer), dataRevisions(dataRevisions)
{
    setUpCoordLines();
}

CoordinateSystem::~CoordinateSystem()
{
    delete[] linesX;
    delete[] linesY;
    //delete[] labels;
}

void CoordinateSystem::setColor(const Color& c)
{
    coordLinesColor = c;
    if (coordLines != nullptr) {
        coordLines->setColor(c);
    }
}

void CoordinateSystem::setGeometry(Geometry geom)
{
    geometry = geom;

    setUpCoordLines();

    geom.leftOffset += XOffset * geom.width;
    geom.topOffset += YOffset * geom.height;
    geom.width *= (1.0 - XOffset);
    geom.height *= (1.0 - YOffset);

    for (auto it = myPlots.begin(); it != myPlots.end(); ++it) {
        if (*it != coordLinesID) {
            data.lookup(*it).setGeometry(geom);
        }
    }
}

void CoordinateSystem::updateLimits(double xmin, double xmax, double ymin, double ymax)
{
    this->xmin = std::min(this->xmin, xmin);
    this->xmax = std::min(this->xmax, xmax);
    this->ymin = std::min(this->ymin, ymin);
    this->ymax = std::min(this->ymax, ymax);

    setUpCoordLines();

    for (auto it = myPlots.begin(); it != myPlots.end(); ++it) {
        data.lookup(*it).setLimits(this->xmin, this->ymin, this->xmax, this->ymax);
    }

    updateLabels = true;
    needLimitUpdate = false;
    changed.invoke(Drawable::ID());
}
	
/*void CoordinateSystem::init(GLuint lineprogram, GLuint textprogram, GLuint mapprogram)
{
    glGenBuffers(1, &textBuffer);

    textpos = glGetAttribLocation(textprogram, "Position");
    textuv = glGetAttribLocation(textprogram, "UV");
    textrect = glGetUniformLocation(textprogram, "Rect");
    textglyphs = glGetUniformLocation(textprogram, "Glyphs");
	textmvp = glGetUniformLocation(textprogram, "MVP");
    textcolor = glGetUniformLocation(textprogram, "Color");
}

void CoordinateSystem::update()
{
    delete[] labels;
    labels = new Label[2*Ticks];
    numChars = 0;
	
	float xspacing = (1.0 - XOffset) / (Ticks + 1.0f);
    for (int i = 0; i < Ticks; ++i) {
		labels[i].len = sprintf(labels[i].label, "%.2f", (i+1) / (Ticks + 1.0f) * (xmax-xmin) + xmin);
		numChars += labels[i].len;
		labels[i].x = x;
		labels[i].y = YOffset / 2.0;
	}
	
    for (int i = Ticks; i < 2*Ticks; ++i) {
		labels[i].len = sprintf(labels[i].label, "%.2f", (i-Ticks+1) / (Ticks + 1.0f) * (ymax-ymin) + ymin);
		numChars += labels[i].len;
		labels[i].x = XOffset / 2.0;
		labels[i].y = y;
    }

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
    glDeleteBuffers(1, &textBuffer);
}

void CoordinateSystem::draw(float const* mvp)
{
    if (updateLabels) {
		updateLabels = false;
		update();
    }
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

void CoordinateSystem::setAxisProperties(int xFlags, int yFlags)
{
    this->xFlags = xFlags;
    this->yFlags = yFlags;
    //! @todo give logscale attribute to plots
    setUpCoordLines();
    changed.invoke(Drawable::ID());
}

void CoordinateSystem::setTickMode(TickMode mode)
{
    this->tickMode = mode;
    //! @todo do sth with data?
    setUpCoordLines();
    changed.invoke(Drawable::ID());
}

Drawable::ID CoordinateSystem::addNewPlot(Drawable* plot)
{
    Drawable::ID id = data.add(plot);
    plot->setId(id);
    dataRevisions[id] = 1;
    myPlots.push_back(id);
    plot->setGeometry(geometry);
    plot->setLimits(xmin, ymin, xmax, ymax);
    plot->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
    changed.invoke(id);
    return id;
}

void CoordinateSystem::removePlot(Drawable::ID id)
{
    if (data.has(id)) {
        data.remove(id);
    }

    for (auto it = myPlots.begin(); it != myPlots.end(); it++) {
        if (*it == id) {
            myPlots.erase(it);
            break;
        }
    }

    for (auto it = dataRevisions.begin(); it != dataRevisions.end(); it++) {
        if (it->first == id) {
            dataRevisions.erase(it);
            break;
        }
    }
}

void CoordinateSystem::setUpCoordLines()
{
    const int n = (2 + 2 * Ticks) * (((xFlags & Axis_PaintPrimary) ? 1 : 0) + ((xFlags & Axis_PaintSecondary) ? 1 : 0)
                                   + ((yFlags & Axis_PaintPrimary) ? 1 : 0) + ((yFlags & Axis_PaintSecondary) ? 1 : 0));
    delete[] linesX;
    delete[] linesY;

    if (n != 0) {
        linesX = new double[n];
        linesY = new double[n];
        unsigned int offset = 0;

        if (xFlags & Axis_PaintPrimary) {
            setUpHorizontalAxis(linesX, linesY, offset, YOffset, xFlags & Axis_Logscale);
            offset += 2 + 2 * Ticks;
        }
        if (xFlags & Axis_PaintSecondary) {
            setUpHorizontalAxis(linesX, linesY, offset, 1.0, xFlags & Axis_Logscale);
            offset += 2 + 2 * Ticks;
        }
        if (yFlags & Axis_PaintPrimary) {
            setUpVerticalAxis(linesX, linesY, offset, XOffset, yFlags & Axis_Logscale);
            offset += 2 + 2 * Ticks;
        }
        if (yFlags & Axis_PaintSecondary) {
            setUpVerticalAxis(linesX, linesY, offset, 1.0, yFlags & Axis_Logscale);
            offset += 2 + 2 * Ticks;
        }
    } else {
        linesX = nullptr;
        linesY = nullptr;
    }

    if (coordLines != nullptr) {
        removePlot(coordLinesID);
    }

    if (n != 0) {
        coordLines = new Lines(n, linesX, linesY, true);
        coordLinesID = addNewPlot(coordLines);
        coordLines->setLimits(0.0, 0.0, 1.0, 1.0);
        coordLines->setColor(coordLinesColor);
    } else {
        coordLines = nullptr;
        coordLinesID = Drawable::ID();
    }
}

void CoordinateSystem::setUpHorizontalAxis(double* linesX, double* linesY, unsigned int indexOffset, double yMean, bool log) const
{
    linesX[indexOffset] = 1.0;
    linesY[indexOffset] = yMean;
    linesX[indexOffset + 1] = XOffset;
    linesY[indexOffset + 1] = yMean;

    if (tickMode == FixedAmount) {
        if (log) {
            //! @todo implement
        } else {
            float xspacing = (1.0 - XOffset) / (Ticks + 1.0f);
            for (int i = 0; i < Ticks; ++i) {
                setUpTick(linesX, linesY, indexOffset+2+2*i, XOffset+(i+1)*xspacing, yMean);
            }
        }
    } else if (tickMode == Smart) {
        std::vector<double> tickPoints = getDataPointsForTicks(xmin, xmax, log);
        float xspacing = (1.0 - XOffset) / (xmax - xmin);
        for (unsigned int i = 0; i < tickPoints.size(); ++i) {
            setUpTick(linesX, linesY, indexOffset+2+2*i, XOffset+(tickPoints[i]-xmin)*xspacing, yMean);
        }
        //! @todo ugly hack better adjust array length apropriately, doesn't even work properly; instead fixing the hack better fix the problem XD
        for (unsigned int i = indexOffset+2+2*tickPoints.size()-1; i < 2+2*Ticks; ++i) {
            linesX[i] = 0.0;
            linesY[i] = 0.0;
        }
    } else {
        //! @todo implement
    }
}

void CoordinateSystem::setUpVerticalAxis(double* linesX, double* linesY, unsigned int indexOffset, double xMean, bool log) const
{
    linesX[indexOffset] = xMean;
    linesY[indexOffset] = 1.0;
    linesX[indexOffset + 1] = xMean;
    linesY[indexOffset + 1] = YOffset;

    if (tickMode == FixedAmount) {
        if (log) {
            //! @todo implement
        } else {
            float yspacing = (1.0 - YOffset) / (Ticks + 1.0f);
            for (int i = 0; i < Ticks; ++i) {
                setUpTick(linesY, linesX, indexOffset+2+2*i, YOffset+(i+1)*yspacing, xMean);
            }
        }
    } else if (tickMode == Smart) {
        std::vector<double> tickPoints = getDataPointsForTicks(ymin, ymax, log);
        float yspacing = (1.0 - YOffset) / (ymax - ymin);
        for (unsigned int i = 0; i < tickPoints.size(); ++i) {
            setUpTick(linesY, linesX, indexOffset+2+2*i, YOffset+(tickPoints[i]-ymin)*yspacing, xMean);
        }
        //! @todo ugly hack better adjust array length apropriately, doesn't even work properly; instead fixing the hack better fix the problem XD
        for (unsigned int i = indexOffset+2+2*tickPoints.size()-1; i < 2+2*Ticks; ++i) {
            linesX[i] = 0.0;
            linesY[i] = 0.0;
        }
    } else {
        //! @todo implement
    }
}

void CoordinateSystem::setUpTick(double* primary, double* secondary, unsigned int indexOffset, double primaryValue, double secondaryMeanValue) const
{
    primary[indexOffset] = primaryValue;
    primary[indexOffset+1] = primaryValue;
    secondary[indexOffset] = secondaryMeanValue-0.5*TickLength;
    secondary[indexOffset+1] = secondaryMeanValue+0.5*TickLength;
}

std::vector<double> CoordinateSystem::getDataPointsForTicks(double min, double max, bool log) const
{
    std::vector<double> ret;
    double div = 10.0, exponent = 1;
    bool todo = true, up = false, down = false;
    unsigned int countMin = 3, countMax = 8;
    int index = 0, size;
    double* steps;

    if (log) {
        size = 1;
        steps = new double[1] {1.0};
    } else {
        size = 3;
        steps = new double[3] {1.0, 2.0, 5.0};
    }

    while (todo) {
        ret = getDataPointsInside(min, max, div);
        if (ret.size() > countMax) {
            index ++;
            if (index >= size) {
                index = 0;
                exponent ++;
            }
            div = steps[index] * pow(10.0, exponent);
            down = true;
        } else if (ret.size() < countMin) {
            index --;
            if (index < 0) {
                index = size - 1;
                exponent --;
            }
            div = steps[index] * pow(10.0, exponent);
            up = true;
            if (down && up) {
                todo = false;
            }
        } else {
            todo = false;
        }
    }

    delete[] steps;
    return ret;
}

std::vector<double> CoordinateSystem::getDataPointsInside(double min, double max, double divisor) const
{
    std::vector<double> ret;

    double val = ceil(min / divisor) * divisor;
    while (val <= max) {
        ret.push_back(val);
        val += divisor;
    }

    return ret;
}
}
