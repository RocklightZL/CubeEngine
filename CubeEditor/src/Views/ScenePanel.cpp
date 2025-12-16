#include "ScenePanel.h"

#include "../Project.h"
#include "../Utils/ImGuiExternal.h"
#include "Cube/Core/Log.h"
#include "Cube/Scene/Entity.h"

#include <imgui/imgui.h>

using namespace Cube;

extern Project* proj;

void ScenePanel::render(float deltaTime) {
    ImGui::Begin("Entity Panel"); 
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

        if(ImGui::BeginPopupContextWindow()) {
            if(ImGui::MenuItem("Add Entity")) {
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
            if(ImGui::BeginPopupContextItem()) {
                if(ImGui::MenuItem("Delete")) {
                    // TODO: add confirm dialog
                    if(entity.get() == proj->selectedEntity) {
                        proj->selectedEntity = nullptr;
                    }
                    proj->selectedScene->scene->destroyEntity(entity.get());
                    proj->selectedScene->isSaved = false;
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
    }
    ImGui::End();
}
