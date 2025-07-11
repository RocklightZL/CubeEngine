#include "pch.h"
#include "Event.h"
namespace Cube {

	void EventDispatcher::subscribe(const Handler& handler, const EventType type) {
		listener[type].push_back(handler);
		CB_CORE_TRACE("listener subscribe");
	}

	void EventDispatcher::dispatch(const Event& e) {
		for(auto& handler : listener[e.getType()]) {
			handler(e);
			CB_CORE_TRACE("{} was handled", e.toString());
		}
	}
}