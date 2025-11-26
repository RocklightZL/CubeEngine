#pragma once
#include "Scene.h"
#include "Cube/Core/Layer.h"

namespace Cube {

    class SceneLayer : public Layer{
    public:
        SceneLayer() = default;
        SceneLayer(const std::shared_ptr<Scene>& scene) : scene(scene) {}
        ~SceneLayer() override = default;

        void onUpdate(float deltaTime) override;

    private:
        std::shared_ptr<Scene> scene;
    };

}