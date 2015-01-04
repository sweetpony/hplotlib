#ifndef HPLOTLIB_WINDOWBASE_HPP
#define HPLOTLIB_WINDOWBASE_HPP

namespace hpl
{
	class Window;
	class WindowBase {
	public:
		WindowBase(Window* win) : win(win) {}
		virtual ~WindowBase() {}
		virtual bool show() = 0;
		virtual void poll() = 0;
		virtual void update() = 0;
		
	protected:		
		Window* win;
	};
}

#endif

