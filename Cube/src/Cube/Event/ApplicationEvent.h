#pragma once

#include "Event.h"

namespace Cube {
	class Window;

	class WindowCloseEvent : public Event{
	public:
		WindowCloseEvent(Window* window) : window(window) {}
		EVENT_TYPE(WindowCloseEvent)

		Window* window = nullptr;
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(Window* window, int width, int height) : window(window), width(width), height(height){}
		EVENT_TYPE(WindowResizeEvent)

		Window* window = nullptr;
		int width;
		int height;
	};
}