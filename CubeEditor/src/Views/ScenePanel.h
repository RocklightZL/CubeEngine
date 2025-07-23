#pragma once

#include "View.h"
#include "Cube/Scene/Entity.h"

#include <string>
#include <vector>

namespace Cube {

    class ScenePanel : public View {
    public:
        ScenePanel() = default;
        ~ScenePanel() override = default;

        void render(float deltaTime) override;
    private:
        void drawComponentSelectionCheckBox();

        bool showAddComponentModal = false;
        std::vector<std::pair<std::string, bool>> componentsCheckBox = {
            {"TransformComponent", false},
            {"SpriteComponent", false},
            {"CameraComponent", false},
            {"AnimatorComponent", false}
        };
        char name[256] = "";
    };

}