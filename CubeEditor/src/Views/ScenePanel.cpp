#include "ScenePanel.h"

#include "../Project.h"
#include "../Utils/ImGuiExternal.h"
#include "Cube/Animator/AnimatorSystem.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/RenderSystem.h"
#include "Cube/Scene/Entity.h"

#include <imgui/imgui.h>

extern Cube::Project* proj;

namespace Cube {

    void ScenePanel::render(float deltaTime) {
        ImGui::Begin("Scene Panel"); 
        if(proj->selectedScene){
            static bool showAddComponentModal = false;
            static std::vector<std::pair<std::string, bool>> componentsCheckBox = {
                {"TransformComponent", false},
                {"SpriteComponent", false},
                {"CameraComponent", false},
                {"AnimatorComponent", false}
            };
            static char name[50] = {};
            static std::shared_ptr<ModalPopup> addEntityPopup = std::make_shared<ModalPopup>("Add Entity", [] {
                ImGui::Text("Name:");
                ImGui::InputText("##input", name, IM_ARRAYSIZE(name));

                ImGui::Text("Select components:");
                ImGui::BeginChild("Select Components", ImVec2(ImGui::GetContentRegionAvail().x, 180.0f));
                for(auto& check : componentsCheckBox) {
                    ImGui::Checkbox(check.first.c_str(), &check.second);
                }
                ImGui::EndChild();
            }, [] {
                auto* entity = proj->selectedScene->scene->createEntity(name);
                for(auto& selectedComponent : componentsCheckBox) {
                    if(selectedComponent.second) {
                        entity->addComponent(selectedComponent.first);
                        proj->selectedScene->isSaved = false;
                    }
                }
            }, [] {
                memset(name, '\0', sizeof(name));
            });
            addEntityPopup->render();

            if(ImGui::TreeNodeEx("Entities", treeNodeFlags)) {
                if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                    ImGui::OpenPopup("RightMenu");
                }
                if(ImGui::BeginPopup("RightMenu")) {
                    if(ImGui::MenuItem("add new entity")) {
                        addEntityPopup->open();
                    }
                    ImGui::EndPopup();
                }
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

            if(ImGui::TreeNodeEx("Systems", treeNodeFlags)) {
                if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                    ImGui::OpenPopup("SystemRightMenu");
                }
                if(ImGui::BeginPopup("SystemRightMenu")) {
                    if(ImGui::BeginMenu("Add System")) {
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

                for(auto& system : proj->selectedScene->scene->getSystems()) {
                    ImGui::Selectable(system->getName().c_str());
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }

}  // namespace Cube
