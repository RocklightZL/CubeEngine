#pragma once

#include <unordered_map>
#include <functional>
#include <typeindex>

namespace Cube {

#define EVENT_TYPE(type) virtual std::type_index getType() const override { return typeid(type); }\
                         virtual std::string toString() const override { return #type; }

    class Event {
    public:
        Event() = default;
        virtual ~Event() = default;
        virtual std::type_index getType() const = 0;
        virtual std::string toString() const = 0;
    };

    class EventDispatcher {
        using Handler = std::function<bool(const Event& e)>;
    public:

        static EventDispatcher& get();

        void dispatch(const Event& e);

        template<typename Type>
        void subscribe(const Handler& handler) {
            listener[typeid(Type)].push_back(handler);
        }

    private:
        EventDispatcher() = default;
        ~EventDispatcher() = default;

        std::unordered_map<std::type_index, std::vector<Handler>> listener;
    };
}  // namespace Cube