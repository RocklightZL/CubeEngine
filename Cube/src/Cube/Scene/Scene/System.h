#pragma once

#include <string>

namespace Cube {
    class Scene;

    class System {
    public:
        System() = default;
        virtual ~System() = default;

        virtual void onUpdate(Scene* scene, float deltaTime) = 0;
        virtual void onAttach(){}
        virtual void onDetach(){}

        virtual std::string getName() const = 0;
    };

}