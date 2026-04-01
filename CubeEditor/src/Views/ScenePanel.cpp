#include "ScenePanel.h"

#include "../App/EditorPage.h"
#include "../Project/Project.h"
#include "../Utils/ImGuiExternal.h"
#include "Cube/Core/Log.h"
#include "Cube/Scene/Entity.h"

#include <imgui/imgui.h>

using namespace Cube;

void ScenePanel::render(float deltaTime) {
    ImGui::Begin("Entity Panel"); 
    if(editorPage.selectedScene){
        static char name[50] = {};
        static std::unique_ptr<ModalPopup> addEntityPopup = std::make_unique<ModalPopup>("Add Entity", [] {
            ImGui::Text("Name:");
            ImGui::InputText("##input", name, IM_ARRAYSIZE(name));
        }, [this] {
            auto* entity = editorPage.selectedScene->scene->createEntity(name);
            editorPage.selectedScene->isSaved = false;
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
        for(auto& entity : editorPage.selectedScene->scene->getAllEntities()) {
            ImGui::PushID(entity.get());
            bool f = false;
            if(editorPage.selectedEntity == entity.get()) {
                f = true;
            }
            if(ImGui::Selectable(entity->getName().c_str(), &f)) {
                editorPage.selectedEntity = entity.get();
            }
            if(ImGui::BeginPopupContextItem()) {
                if(ImGui::MenuItem("Delete")) {
                    // TODO: add confirm dialog
                    if(entity.get() == editorPage.selectedEntity) {
                        editorPage.selectedEntity = nullptr;
                    }
                    editorPage.selectedScene->scene->destroyEntity(entity.get());
                    editorPage.selectedScene->isSaved = false;
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
    }
    ImGui::End();
}
