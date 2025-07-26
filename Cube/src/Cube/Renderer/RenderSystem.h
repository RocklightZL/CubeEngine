#pragma once

#include "Cube/Scene/System.h"

namespace Cube {

    class RenderSystem : public System {
    public:
        RenderSystem() = default;
        ~RenderSystem() override = default;

        void onUpdate(Scene* scene, float deltaTime) override;

        void onAttach() override;

        std::string getName() const override;
    };

}  // namespace Cube