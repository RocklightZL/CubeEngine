#pragma once

#include <functional>
#include <unordered_map>
#include <string>

#include "Cube/Reflection/Type.h"

namespace Cube {

#define EVENT_TYPE(type) virtual Cube::TypeID getType() const override { return Cube::getTypeID<type>(); }\
                         virtual std::string toString() const override { return #type; }

    class Event {
    public:
        Event() = default;
        virtual ~Event() = default;
        virtual TypeID getType() const = 0;
        virtual std::string toString() const = 0;
    };

    class EventDispatcher {
        using Handler = std::function<bool(const Event& e)>;
    public:
        EventDispatcher() = default;
        ~EventDispatcher() = default;

        void dispatch(const Event& e);

        // TODO: unsubscribe
        template<typename Type>
        void subscribe(const Handler& handler) {
            listener[getTypeID<Type>()].push_back(handler);
        }

    private:
        std::unordered_map<TypeID, std::vector<Handler>> listener;
    };
}  // namespace Cube