/*
 * Canvas.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_CANVAS_HPP
#define HPLOTLIB_CANVAS_HPP

#include "Window.hpp"
#include "ProgramDatabase.hpp"
#include "Font.hpp"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Layout.hpp"
#include "PostscriptPrinter.hpp"
#include "Registry.hpp"

#include <vector>
#include <string>
#include <queue>

namespace hpl
{
class Canvas : public Window {
public:
	Canvas(std::string const& fontFile) : fontFile(fontFile) {}
	~Canvas();
	
	void addCoordinateSystemToLayout(CoordinateSystem::ID cs, Layout::ID to) { addSlotToLayout(Slot(cs), to); }
	void addLayoutToLayout(Layout::ID layout, Layout::ID to) { addSlotToLayout(Slot(layout), to); }
	
	template<typename T>
	T& addLayout() {
		T* layout = new T;
		Layout::ID id = layouts.add(layout);
		layout->changed.template bind<Canvas, &Canvas::recalculateLayout>(this);
		return *layout;
	}
	
	template<typename T>
    T& addCoordinateSystem();

    inline void setBackgroundColor(const Color& c) {
        backgroundColor = c;
    }

    bool saveToFile(const std::string& fileName);


protected:
	virtual void init();
	virtual void destroy();
	virtual void draw();
	virtual void moveEvent(int deltax, int deltay);
	virtual void mouseWheelEvent(int x, int y, double delta);
	virtual void resetEvent();
	
private:
	struct Slot {
        Slot(Layout::ID l) : layout(l) {}
		Slot(CoordinateSystem::ID c) : cs(c) {}
		bool operator==(Slot const& other) {
			return layout == other.layout && cs == other.cs;
		}
		void invalidate() {
			layout.invalidate();
			cs.invalidate();
		}
		Layout::ID layout;
		CoordinateSystem::ID cs;
	};
	struct Rack {
		std::vector<Slot> slots;
		std::vector<Geometry> geometries;
	};

	Slot* findSlot(Slot const& slot);
	void addSlotToLayout(Slot const& slot, Layout::ID to);
	void recalculateLayout(Layout::ID layout);

	Registry<Layout> layouts;
	Registry<CoordinateSystem> csystems;
	std::queue<CoordinateSystem::ID> csInit;

	std::unordered_map<Layout::ID, Rack, std::hash<Layout::ID::Type>> racks;

	std::string fontFile;
	Font font;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    Color backgroundColor = Color(1.0f, 1.0f, 1.0f);
	
    ProgramDatabase programsDatabase;
    
    float mvp[9] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};
};

template<typename T>
T& Canvas::addCoordinateSystem()
{
    T* cs = new T(&font);
    cs->changed.template bind<Window, &Window::update>(this);

    pthread_mutex_lock(&mutex);
    CoordinateSystem::ID id = csystems.add(cs);
    csInit.push(id);
	pthread_mutex_unlock(&mutex);

    update();

    return *cs;
}
}

#endif

