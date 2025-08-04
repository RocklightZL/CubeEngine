#pragma once
#include "Cube/Scene/System.h"

#include <string>

namespace Cube {

    class EditorRenderSystem : public System {
    public:
        EditorRenderSystem() = default;
        ~EditorRenderSystem() override = default;

        void onUpdate(Scene* scene, float deltaTime) override;
        std::string getName() const override { return "EditorRenderSystem"; }
    };

}