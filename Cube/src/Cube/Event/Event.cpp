#include "pch.h"
#include "Event.h"
namespace Cube {

    EventDispatcher& EventDispatcher::get() {
        static EventDispatcher instance;
		return instance;
    }

    void EventDispatcher::dispatch(const Event& e) {
		for(auto& handler : listener[e.getType()]) {
			handler(e);
			CB_CORE_TRACE("{} was handled", e.toString());
		}
	}
}