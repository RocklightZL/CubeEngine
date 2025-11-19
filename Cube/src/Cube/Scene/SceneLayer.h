#pragma once
#include "Cube/Core/Layer.h"

namespace Cube {
    class Scene;

    class SceneLayer : public Layer {
    public:
        SceneLayer(const std::string& path);
        ~SceneLayer();

        void onUpdate(float deltaTime) override;
    private:
        Scene* scene = nullptr;
    };

}