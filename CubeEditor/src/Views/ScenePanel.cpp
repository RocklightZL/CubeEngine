#include "ScenePanel.h"

#include "../Project.h"
#include "Cube/Animator/AnimatorSystem.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/RenderSystem.h"
#include "Cube/Scene/Entity.h"

#include <imgui/imgui.h>

extern Cube::Project* proj;

namespace Cube {

    void ScenePanel::render(float deltaTime) {
        ImGui::Begin("ScenePanel"); 
        if(proj->selectedScene){
            if(ImGui::TreeNodeEx("Entities")) {
                for(Entity* entity : proj->selectedScene->scene->getEntities()) {
                    ImGui::PushID(entity->getID());
                    bool f = false;
                    if(proj->selectedEntity == entity) {
                        f = true;
                    }
                    if(ImGui::Selectable(entity->getName().c_str(), &f)) {
                        proj->selectedEntity = entity;
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

                if(ImGui::Button("Cancel##1", ImVec2(120, 0))) {
                    memset(name, '\0', sizeof(name));
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if(ImGui::Button("OK##1", ImVec2(120, 0))) {
                    auto* entity = proj->selectedScene->scene->createEntity(name);
                    for(auto& selectedComponent : componentsCheckBox) {
                        if(selectedComponent.second) {
                            entity->addComponent(selectedComponent.first);
                            proj->selectedScene->isSaved = false;
                        }
                    }
                    memset(name, '\0', sizeof(name));
                    ImGui::CloseCurrentPopup();
                }
                showAddComponentModal = false;
                ImGui::EndPopup();
            }

            if(ImGui::TreeNode("Systems")) {
                for(auto& system : proj->selectedScene->scene->getSystems()) {
                    ImGui::Selectable(system->getName().c_str());
                }
                ImGui::TreePop();
            }
            if(ImGui::BeginPopupContextItem()) {
                if(ImGui::BeginMenu("Add System#1")) {
                    if(ImGui::MenuItem("RenderSystem##1")) {
                        if(!proj->selectedScene->scene->hasSystem("RenderSystem")) {
                            proj->selectedScene->scene->addSystem(new RenderSystem());
                            proj->selectedScene->isSaved = false;
                        }
                    }
                    if(ImGui::MenuItem("AnimatorSystem##1")) {
                        if(!proj->selectedScene->scene->hasSystem("AnimatorSystem")) {
                            proj->selectedScene->scene->addSystem(new AnimatorSystem());
                            proj->selectedScene->isSaved = false;
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndPopup();
            }
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
