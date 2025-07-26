#include "SceneSelectPanel.h"

#include "../Project.h"
#include "Cube/Scene/Scene.h"

#include <imgui/imgui.h>

extern Cube::Project* proj;

namespace Cube {

    void SceneSelectPanel::render(float deltaTime) {
        ImGui::Begin("Scene Select Panel");

        for(Scene* scene : proj->getScenes()) {
            ImGui::PushID(scene);
            bool flag = false;
            if(proj->selectedScene == scene) {
                flag = true;
            }
            if(ImGui::Selectable(scene->getName().c_str(), &flag)) {
                proj->selectedScene = scene;
            }
            ImGui::PopID();
        }

        ImGui::End();
    }
}  // namespace Cube