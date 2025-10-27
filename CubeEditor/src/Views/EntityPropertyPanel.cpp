#include "EntityPropertyPanel.h"

#include "SceneView.h"
#include "../Project.h"
#include "Cube/Core/Log.h"
#include "Cube/Resource/ResourceManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

extern Cube::Project* proj;

namespace Cube {

    void EntityPropertyPanel::render(float deltaTime) {
        ImGui::Begin("Entity Properties");

        // Ó²±àÂë
        if(proj->selectedEntity) {
            float posX = ImGui::GetWindowWidth() / 2 - 30.0f;
            float width = ImGui::GetWindowWidth() - posX - 10.0f;

            char name[256] = {};
            strcpy_s(name, proj->selectedEntity->getName().c_str());
            ImGui::Text("name");
            ImGui::SameLine();
            ImGui::SetCursorPosX(posX);
            ImGui::SetNextItemWidth(width);
            if(ImGui::InputText("##InputTextEntityName", name, IM_ARRAYSIZE(name))) proj->selectedScene->isSaved = false;
            proj->selectedEntity->setName(name);

            if(proj->selectedEntity->hasComponent<TransformComponent>()) {
                TransformComponent* tc = proj->selectedEntity->getComponent<TransformComponent>();
                if(ImGui::TreeNodeEx("TransformComponent", treeNodeFlags)) {
                    ImGui::Text("Position");
                    float pos[2] = {tc->position.x, tc->position.y};
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(posX);
                    ImGui::SetNextItemWidth(width);
                    if(ImGui::DragFloat2("##Position", pos, 1, 0, 0, "%.1f")) proj->selectedScene->isSaved = false;
                    tc->position = {pos[0], pos[1]};

                    ImGui::Text("Scale");
                    float scale[2] = {tc->scale.x, tc->scale.y};
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(posX);
                    ImGui::SetNextItemWidth(width);
                    if(ImGui::DragFloat2("##Scale", scale, 1, 0, 0, "%.1f")) proj->selectedScene->isSaved = false;
                    tc->scale = {scale[0], scale[1]};

                    ImGui::Text("Rotation");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(posX);
                    ImGui::SetNextItemWidth(width);
                    if(ImGui::DragFloat("##Rotation", &tc->rotation, 1, 0, 0, "%.1f")) proj->selectedScene->isSaved = false;

                    ImGui::TreePop();
                }

            }

            if(proj->selectedEntity->hasComponent<SpriteComponent>()) {
                SpriteComponent* sc = proj->selectedEntity->getComponent<SpriteComponent>();
                if(ImGui::TreeNodeEx("SpriteComponent", treeNodeFlags)) {
                    ImGui::Text("TextureRegion");
                    float in[4] = {sc->region.uvMin.x, sc->region.uvMin.y, sc->region.uvMax.x, sc->region.uvMax.y};
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(posX);
                    ImGui::SetNextItemWidth(width);
                    if(ImGui::DragFloat4("##TextureRegion", in, 0.001f, 0, 1)) proj->selectedScene->isSaved = false;
                    sc->region.uvMin = {in[0], in[1]};
                    sc->region.uvMax = {in[2], in[3]};

                    ImGui::Text("Color");
                    float color[4] = {sc->color.r, sc->color.g, sc->color.b, sc->color.a};
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(posX);
                    ImGui::SetNextItemWidth(width);
                    if(ImGui::ColorEdit4("##Color", color, ImGuiColorEditFlags_DisplayHex)) proj->selectedScene->isSaved = false;
                    sc->color = {color[0], color[1], color[2], color[3]};

                    static bool showSelectedSubTexturePopup = false;
                    static std::string texturePath = "Texture";
                    static std::shared_ptr<TextureData> textureData;
                    ImGui::Text("Texture");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(posX);
                    ImGui::SetNextItemWidth(width);
                    if(sc->texture) {
                        ImVec2 size = ImVec2(sc->texture->getWidth(), sc->texture->getHeight()) * ImVec2(sc->region.uvMax.x - sc->region.uvMin.x, sc->region.uvMax.y - sc->region.uvMin.y);
                        ImGui::Image(sc->texture->getId(), size * 50 / std::max(size.x, size.y), ImVec2(sc->region.uvMin.x, sc->region.uvMax.y), ImVec2(sc->region.uvMax.x, sc->region.uvMin.y));
                        ImGui::SetItemTooltip(sc->texture->getFilePath().c_str());
                    }else {
                        ImGui::Button("None");
                        ImGui::SetItemTooltip("No texture");
                    }
                    if(ImGui::BeginDragDropTarget()) {
                        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TexturePath")) {
                            texturePath = (const char*)payload->Data;
                            if(Utils::isFileExists(texturePath + ".meta")) {
                                showSelectedSubTexturePopup = true;
                                textureData = std::make_shared<TextureData>(texturePath + ".meta", texturePath);
                            }else{
                                if(sc->texture) {
                                    ResourceManager::get().release(sc->texture->getFilePath());
                                }
                                sc->texture = ResourceManager::get().load<Texture2D>(texturePath)->data;
                                proj->selectedScene->isSaved = false;
                            }
                        }
                        ImGui::EndDragDropTarget();
                    }
                    if(showSelectedSubTexturePopup && textureData) {
                        if(auto* res = SceneView::selectSubTexturePopup(*textureData, &showSelectedSubTexturePopup)) {
                            if(sc->texture) {
                                ResourceManager::get().release(sc->texture->getFilePath());
                            }
                            sc->texture = ResourceManager::get().load<Texture2D>(texturePath)->data;
                            sc->region = {res->uvMin, res->uvMax};
                            proj->selectedScene->isSaved = false;
                        }
                    }
                    ImGui::TreePop();
                }
            }

            if(proj->selectedEntity->hasComponent<CameraComponent>()) {
                CameraComponent* cc = proj->selectedEntity->getComponent<CameraComponent>();
                if(ImGui::TreeNodeEx("CameraComponent", treeNodeFlags)) {
                    ImGui::Text("Available");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(posX);
                    ImGui::SetNextItemWidth(width);
                    if(ImGui::Checkbox("##available", &cc->available)) proj->selectedScene->isSaved = false;

                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }
}  // namespace Cube