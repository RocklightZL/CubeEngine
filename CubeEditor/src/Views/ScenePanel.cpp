#include "ScenePanel.h"

#include "../Model.h"
#include "Cube/Core/Log.h"
#include "Cube/Scene/Entity.h"

#include <imgui/imgui.h>

extern Cube::Model* data;

namespace Cube {

    void ScenePanel::render(float deltaTime) {
        ImGui::Begin("ScenePanel");
        if(ImGui::TreeNode("Entities")) {
            for(Entity* entity : data->scene.getEntities()) {
                if(ImGui::TreeNode(entity->getName().c_str())) {
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        if(ImGui::BeginPopupContextItem()) {
            if(ImGui::MenuItem("add new entity")) {
                data->scene.createEntity();
            }
            ImGui::EndPopup();
        }
        if(showAddComponentModal) {
            ImGui::OpenPopup("add component");
        }
        if(ImGui::BeginPopupModal("add component", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
            if(ImGui::BeginCombo("##DropDownCheckBox", "select components")) {
                for(auto& check : componentsCheckBox) {
                    ImGui::Checkbox(check.first.c_str(), &check.second);
                }
                ImGui::EndCombo();
            }
            if(ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if(ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            showAddComponentModal = false;
            ImGui::EndPopup();
        }

        if(ImGui::TreeNode("Systems")) {
            ImGui::TreePop();
        }
        if(ImGui::BeginPopupContextItem()) {
            if(ImGui::MenuItem("add system")) {
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }

}  // namespace Cube