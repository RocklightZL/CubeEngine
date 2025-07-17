#pragma once

#include "Cube/Scene/System.h"

namespace Cube {

    class RenderSystem : public System {
    public:
        RenderSystem(float viewportWidth, float viewportHeight);
        virtual ~RenderSystem();

        void onUpdate(Scene* scene, float deltaTime) override;

        void onAttach() override;
    private:
        float viewportWidth;
        float viewportHeight;
    };

}  // namespace Cube