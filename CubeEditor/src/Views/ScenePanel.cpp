#include "ScenePanel.h"

#include "../Model.h"
#include "Cube/Core/Log.h"
#include "Cube/Scene/Entity.h"

#include <imgui/imgui.h>

extern Cube::Model* data;

namespace Cube {

    void ScenePanel::render(float deltaTime) {
        ImGui::Begin("ScenePanel");
        if(ImGui::TreeNodeEx("Entities")) {
            for(Entity* entity : data->scene.getEntities()) {
                ImGui::PushID(entity->getID());
                bool f = false;
                if(data->selectedEntity == entity) {
                    f = true;
                }
                if(ImGui::Selectable(entity->getName().c_str(), &f)) {
                    data->selectedEntity = entity;
                }
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
        if(ImGui::BeginPopupContextItem()) {
            if(ImGui::MenuItem("add new entity")) {
                showAddComponentModal = true;
            }
            ImGui::EndPopup();
        }
        if(showAddComponentModal) {
            ImGui::OpenPopup("addEntity");
        }
        if(ImGui::BeginPopupModal("addEntity", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::InputText("name##input", name, IM_ARRAYSIZE(name));

            drawComponentSelectionCheckBox();

            if(ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if(ImGui::Button("OK", ImVec2(120, 0))) {
                auto* entity = data->scene.createEntity(name);
                for(auto& selectedComponent : componentsCheckBox) {
                    if(selectedComponent.second) {
                        entity->addComponent(selectedComponent.first);
                    }
                }
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

    void ScenePanel::drawComponentSelectionCheckBox() {
        if(ImGui::BeginCombo("##DropDownCheckBox", "select components")) {
            for(auto& check : componentsCheckBox) {
                ImGui::Checkbox(check.first.c_str(), &check.second);
            }
            ImGui::EndCombo();
        }
    }

}  // namespace Cube
