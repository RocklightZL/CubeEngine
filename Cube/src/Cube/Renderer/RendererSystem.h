#pragma once

#include "Cube/Scene/System.h"

namespace Cube {

    class RendererSystem : public System {
    public:
        RendererSystem(float viewportWidth, float viewportHeight);

        void onUpdate(Scene* scene) override;

        void onAttach() override;
    private:
        float viewportWidth;
        float viewportHeight;
    };

}  // namespace Cube