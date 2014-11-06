/*
 * CoordinateSystem.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_COORDINATESYSTEM_HPP
#define HPLOTLIB_COORDINATESYSTEM_HPP

#include <queue>
#include <map>

#include "Drawable.hpp"
#include "Statistics.hpp"
#include "Contour.hpp"
#include "Registry.hpp"

#include "GL/glld.h"

namespace hpl
{
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

    CoordinateSystem(Registry<Drawable>& dataContainer, std::map<Drawable::ID, unsigned int>& dataRevisions);
    ~CoordinateSystem();

    float* getLines() const;
    inline unsigned int getLinesCount() const {
        return 8 + 2*Ticks*4;
    }

    Label* getLabels() const;
    inline unsigned int getLabelsCount() const {
        return 2*Ticks;
    }
    
    void setColor(const Color& c);
    inline Color getColor() const {
        return drawColor;
    }

    void setGeometry(Geometry geom);
    
    template<typename T>
    T& addPlot(int n, double const* x, double const* y);
    template<typename T>
    T& addPlot(int n, double const* x, double const* y, double const* z);

    /*virtual void init(GLuint lineprogram, GLuint textprogram, GLuint mapprogram);
	virtual void destroy();
	virtual void draw(float const* mvp);
	
    virtual void update();*/

    void updateLimits(double xmin, double xmax, double ymin, double ymax);

    Delegate<Drawable::ID> changed;
    
    inline ID id() const { return csId; }
    inline void setId(ID id) { csId = id; }
	
private:
    void addNewPlot(Drawable::ID id);

    Registry<Drawable>& data;
    std::map<Drawable::ID, unsigned int>& dataRevisions;
    std::queue<Drawable::ID> plotInit;
    std::vector<Drawable::ID> myPlots;
    ID csId;
	Geometry geometry;

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
T& CoordinateSystem::addPlot(int n, double const* x, double const* y)
{
    if (needLimitUpdate) {
        updateLimits(hpl::min(n, x), hpl::max(n, x), hpl::min(n, y), hpl::max(n, y));
    }
	
    T* plot = new T(n, x, y);
    plot->setLimits(xmin, ymin, xmax, ymax);
    Drawable::ID id = data.add(plot);
    plot->setId(id);
    plot->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
    addNewPlot(id);
    return *plot;
}

template<typename T>
T& CoordinateSystem::addPlot(int n, double const* x, double const* y, double const* z)
{
    if (needLimitUpdate) {
        updateLimits(hpl::min(n, x), hpl::max(n, x), hpl::min(n, y), hpl::max(n, y));
    }

    T* plot = new T(n, x, y, z);
    plot->setLimits(xmin, ymin, xmax, ymax);
    Drawable::ID id = data.add(plot);
    plot->setId(id);
    plot->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
    addNewPlot(id);
    return *plot;
}
}

#endif

