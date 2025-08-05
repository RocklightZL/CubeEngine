#pragma once

#include "Event.h"

namespace Cube {
	class Window;

	class WindowCloseEvent : public Event{
	public:
		WindowCloseEvent(Window* window) : window(window) {}
		EVENT_TYPE(WindowClose)

		Window* getWindow() const { return window; }
	private:
		Window* window = nullptr;
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(Window* window, int width, int height) : window(window), width(width), height(height){}
		EVENT_TYPE(WindowResize)
		int getWidth() const {return width;}
		int getHeight() const {return height;}
		Window* getWindow() const { return window; }
	private:
		Window* window = nullptr;
		int width;
		int height;
	};
}