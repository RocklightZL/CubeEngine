#pragma once

#include "Cube/Scene/System.h"

namespace Cube {

    class RenderSystem : public System {
    public:
        RenderSystem(float viewportWidth, float viewportHeight);

        void onUpdate(Scene* scene) override;

        void onAttach() override;
    private:
        float viewportWidth;
        float viewportHeight;
    };

}  // namespace Cube