#pragma once
#include "Event.h"
#include "Cube/Core/KeyCodes.h"

namespace Cube {
	class KeyEvent : public Event {
	public:
		virtual KeyCode getKeyCode() const {
			return keyCode;
		}
	protected:
		KeyEvent(KeyCode keyCode) :keyCode(keyCode) {};
		virtual ~KeyEvent() = default;
	private:
		KeyCode keyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(KeyCode keyCode) : KeyEvent(keyCode){}
		virtual ~KeyPressedEvent() = default;
		EVENT_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(KeyCode keyCode) : KeyEvent(keyCode){}
		virtual ~KeyReleasedEvent() = default;
		EVENT_TYPE(KeyReleased)
	};
}
