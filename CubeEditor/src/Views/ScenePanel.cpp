#include "ScenePanel.h"

#include "../Project.h"
#include "../Utils/ImGuiExternal.h"
#include "Cube/Core/Log.h"
#include "Cube/Scene/Entity.h"

#include <imgui/imgui.h>

using namespace Cube;

extern Project* proj;

void ScenePanel::render(float deltaTime) {
    ImGui::Begin("Scene Panel"); 
    if(proj->selectedScene){
        static char name[50] = {};
        static std::unique_ptr<ModalPopup> addEntityPopup = std::make_unique<ModalPopup>("Add Entity", [] {
            ImGui::Text("Name:");
            ImGui::InputText("##input", name, IM_ARRAYSIZE(name));
        }, [] {
            auto* entity = proj->selectedScene->scene->createEntity(name);
            proj->selectedScene->isSaved = false;
            addEntityPopup->close();
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
            for(auto& entity : proj->selectedScene->scene->getAllEntities()) {
                ImGui::PushID(entity.get());
                bool f = false;
                if(proj->selectedEntity == entity.get()) {
                    f = true;
                }
                if(ImGui::Selectable(entity->getName().c_str(), &f)) {
                    proj->selectedEntity = entity.get();
                }
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
