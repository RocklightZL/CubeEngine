#include "SceneSelectPanel.h"

#include "../Project.h"
#include "Cube/Scene/Scene.h"

#include <imgui/imgui.h>

extern Cube::Project* proj;

namespace Cube {

    void SceneSelectPanel::render(float deltaTime) {
        ImGui::Begin("Scene Select Panel");

        for(SceneData& scene : proj->scenes) {
            ImGui::PushID(scene.scene);
            bool flag = false;
            if(proj->selectedScene == &scene) {
                flag = true;
            }
            if(ImGui::Selectable(std::string(scene.scene->getName() + (scene.isSaved ? "" : "*")).c_str(), &flag)) {
                proj->selectedScene = &scene;
            }
            ImGui::PopID();
        }

        ImGui::End();
    }
}  // namespace Cube