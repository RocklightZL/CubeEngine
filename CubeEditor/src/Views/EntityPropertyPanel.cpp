#include "EntityPropertyPanel.h"

#include "../Project.h"
#include "imgui/imgui.h"

extern Cube::Project* proj;

namespace Cube {

    void EntityPropertyPanel::render(float deltaTime) {
        ImGui::Begin("Entity Properties");

        // Ó²±àÂë
        if(proj->selectedEntity) {

            char name[256] = {};
            strcpy_s(name, proj->selectedEntity->getName().c_str());
            ImGui::Text("name: ");
            ImGui::SameLine();
            if(ImGui::InputText("##InputTextEntityName", name, IM_ARRAYSIZE(name))) proj->selectedScene->isSaved = false;
            proj->selectedEntity->setName(name);

            if(proj->selectedEntity->hasComponent<TransformComponent>()) {
                TransformComponent* tc = proj->selectedEntity->getComponent<TransformComponent>();
                if(ImGui::TreeNode("TransformComponent")) {
                    ImGui::Text("Position: ");
                    ImGui::SameLine();
                    float pos[2] = {tc->position.x, tc->position.y};
                    if(ImGui::DragFloat2("##Position", pos, 1, 0, 0, "%.1f")) proj->selectedScene->isSaved = false;
                    tc->position = {pos[0], pos[1]};

                    ImGui::Text("Scale: ");
                    ImGui::SameLine();
                    float scale[2] = {tc->scale.x, tc->scale.y};
                    if(ImGui::DragFloat2("##Scale", scale, 1, 0, 0, "%.1f")) proj->selectedScene->isSaved = false;
                    tc->scale = {scale[0], scale[1]};

                    ImGui::Text("Rotation: ");
                    ImGui::SameLine();
                    if(ImGui::DragFloat("##Rotation", &tc->rotation, 1, 0, 0, "%.1f")) proj->selectedScene->isSaved = false;

                    ImGui::TreePop();
                }

            }

            if(proj->selectedEntity->hasComponent<SpriteComponent>()) {
                SpriteComponent* sc = proj->selectedEntity->getComponent<SpriteComponent>();
                if(ImGui::TreeNode("SpriteComponent")) {
                    ImGui::Text("TextureRegion: ");
                    ImGui::SameLine();
                    float in[4] = {sc->region.uvMax.x, sc->region.uvMax.y, sc->region.uvMin.x, sc->region.uvMin.y};
                    if(ImGui::DragFloat4("##TextureRegion", in, 0.001f, 0, 1)) proj->selectedScene->isSaved = false;
                    sc->region.uvMax = {in[0], in[1]};
                    sc->region.uvMin = {in[2], in[3]};

                    ImGui::Text("Color: ");
                    ImGui::SameLine();
                    float color[4] = {sc->color.r, sc->color.g, sc->color.b, sc->color.a};
                    if(ImGui::ColorEdit4("##Color", color)) proj->selectedScene->isSaved = false;
                    sc->color = {color[0], color[1], color[2], color[3]};

                    ImGui::TreePop();
                }
            }

            if(proj->selectedEntity->hasComponent<CameraComponent>()) {
                CameraComponent* cc = proj->selectedEntity->getComponent<CameraComponent>();
                if(ImGui::TreeNode("CameraComponent")) {
                    ImGui::Text("Available");
                    ImGui::SameLine();
                    if(ImGui::Checkbox("##available", &cc->available)) proj->selectedScene->isSaved = false;

                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }
}  // namespace Cube