#include "SceneSelectPanel.h"

#include "../App/EditorPage.h"
#include "../Project/Project.h"
#include "Cube/Scene/Scene.h"

#include <imgui/imgui.h>

void SceneSelectPanel::render(float deltaTime) {
    Project* project = editorPage.getProject();
    ImGui::Begin("Scene Select Panel");

    for(SceneData& scene : project->getScenes()) {
        ImGui::PushID(scene.scene);
        bool flag = false;
        if(editorPage.selectedScene == &scene) {
            flag = true;
        }
        if(ImGui::Selectable(std::string(scene.scene->getName() + (scene.isSaved ? "" : "*")).c_str(), &flag)) {
            editorPage.selectedScene = &scene;
            editorPage.selectedEntity = nullptr;
        }
        ImGui::PopID();
    }

    ImGui::End();
}