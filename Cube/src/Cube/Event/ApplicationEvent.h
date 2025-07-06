#pragma once

#include "Event.h"

namespace Cube {
	class WindowCloseEvent : public Event{
	public:
		WindowCloseEvent() = default;
		EVENT_TYPE(WindowClose)
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height) : width(width), height(height){}
		EVENT_TYPE(WindowResize)
		int getWidth() const {return width;}
		int getHeight() const {return height;}
	private:
		int width;
		int height;
	};
}