#pragma once

namespace Cube {
    class Scene;

    class System {
    public:
        System() = default;
        virtual ~System() = default;

        virtual void onUpdate(Scene* scene) = 0;
        virtual void onAttach(){}
        virtual void onDetach(){}
    };

}