#pragma once

#include "Event.h"

namespace Cube {
	class WindowCloseEvent : public Event{
	public:
		WindowCloseEvent() = default;
		EVENT_TYPE(WindowClose)
	};
}