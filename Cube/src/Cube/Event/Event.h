#pragma once

#include <unordered_map>
#include <functional>

namespace Cube {

    enum EventType {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased,
        MousePressed, MouseReleased, MouseMoved, MouseScrolled
    };

#define EVENT_TYPE(type) static EventType getStaticType(){return EventType::##type;}\
                         virtual EventType getType() const override{return getStaticType();}\
                         virtual const char* toString() const override{return #type;}

    class Event {
    public:
        Event() = default;
        virtual ~Event() = default;
        virtual EventType getType() const = 0;
        virtual const char* toString() const = 0;
    };

    class EventDispatcher {
        using Handler = std::function<bool(const Event& e)>; // ������
    public:
        EventDispatcher() = default;
        ~EventDispatcher() = default;
        void subscribe(const Handler& handler, EventType type);
        void dispatch(const Event& e);
    private:
        // ������
        std::unordered_map<EventType, std::vector<Handler>> listener;
    };
}  // namespace Cube