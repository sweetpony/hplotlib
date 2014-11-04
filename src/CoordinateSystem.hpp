/*
 * CoordinateSystem.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_COORDINATESYSTEM_HPP
#define HPLOTLIB_COORDINATESYSTEM_HPP

#include "Plot.hpp"
#include "Statistics.hpp"
#include "Contour.hpp"
#include "PaintServer.hpp"

#include "GL/glld.h"

#include <queue>

namespace hpl
{
class Font;
class CoordinateSystem {
public:
	typedef IDBase<CoordinateSystem> ID;

    struct Label {
        char label[16];
        int len = 0;
        float x, y, width, height;
    };

	static constexpr float XOffset = 0.12f;
	static constexpr float YOffset = 0.08f;
	static constexpr int Ticks = 8;
	static constexpr float TickLength = 0.02f;

    CoordinateSystem(Font* font);
    ~CoordinateSystem();

    float* getLines() const;
    inline unsigned int getLinesCount() const {
        return 8 + 2*Ticks*4;
    }

    Label* getLabels() const;
    inline unsigned int getLabelsCount() const {
        return 2*Ticks;
    }

    inline Registry<Plot>& getPlots() {
        return rawData;
    }
    
    void setColor(const Color& c);
    inline Color getColor() const {
        return drawColor;
    }

    void setGeometry(Geometry geom);
    
    template<typename T>
    T& addPlot(int n, double const* x, double const* y, PaintServer& paintServer);
    template<typename T>
    T& addPlot(int n, double const* x, double const* y, double const* z, PaintServer& paintServer);

    /*virtual void init(GLuint lineprogram, GLuint textprogram, GLuint mapprogram);
	virtual void destroy();
	virtual void draw(float const* mvp);
	
    virtual void update();*/

    void updateLimits(double xmin, double xmax, double ymin, double ymax);

    Delegate<> changed;
    
    inline ID id() const { return csId; }
    inline void setId(ID id) { csId = id; }
	
private:
    Registry<Plot> rawData;
	std::queue<Plot::ID> plotInit;
    ID csId;
	Geometry geometry;

	Font* font;
	Color drawColor;
    bool updateLabels = false, needLimitUpdate = true;

	double xmin = std::numeric_limits<double>::min();
	double xmax = std::numeric_limits<double>::max();
	double ymin = std::numeric_limits<double>::min();
	double ymax = std::numeric_limits<double>::max();

    /*GLuint lineBuffer;
    GLuint textBuffer;
    GLuint lineprogram;
    GLuint textprogram;
    GLuint mapprogram;
    
	GLint linepos, linerect, linecolor, linemvp, textpos, textuv, textrect, textglyphs, textmvp, textcolor;
	int numLines = 0;
	int numChars = 0;

    float* lines = nullptr;
    Label* labels = nullptr;*/
};

template<typename T>
T& CoordinateSystem::addPlot(int n, double const* x, double const* y, PaintServer& paintServer)
{
    if (needLimitUpdate) {
        updateLimits(hpl::min(n, x), hpl::max(n, x), hpl::min(n, y), hpl::max(n, y));
    }
	
    T* plot = new T(n, x, y);
    plot->changed.template bind<Delegate<>, &Delegate<>::invoke>(&changed);
    Plot::ID id1 = rawData.add(plot);
    Plot::ID id2 = paintServer.addPlot<T>(plot, xmin, xmax, ymin, ymax);
    //! @todo need connection id1->id2 for further referencing
    plotInit.push(id2);
    setGeometry(geometry);
    changed.invoke();
    return *plot;
}

template<typename T>
T& CoordinateSystem::addPlot(int n, double const* x, double const* y, double const* z, PaintServer& paintServer)
{
    /*updateLimits(hpl::min(n, x), hpl::max(n, x), hpl::min(n, y), hpl::max(n, y));

    T* plot = new T(n, x, y, z);
    plot->changed.template bind<Delegate<>, &Delegate<>::invoke>(&changed);
    Plot::ID id = rawData.add(plot);
    plotInit.push(id);
    setGeometry(geometry);
    changed.invoke();
    return *plot;*/
}
}

#endif

