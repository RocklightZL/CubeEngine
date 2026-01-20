#include "EntityPropertyPanel.h"

#include "../Project.h"
#include "../Utils/ImGuiExternal.h"
#include "Cube/Animation/Animation.h"
#include "Cube/Core/Log.h"
#include "Cube/Resource/ResourceManager.h"
#include "Cube/Scene/Camera2D.h"
#include "Cube/Scene/SpriteRender.h"
#include "SceneView.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

using namespace Cube;

extern Project* proj;

void EntityPropertyPanel::render(float deltaTime) {
    ImGui::Begin("Entity Properties");

    if(proj->selectedEntity) {
        if(proj->selectedEntity != preEntity) {
            updateCache();
            preEntity = proj->selectedEntity;
        }
        float posX = ImGui::GetWindowWidth() / 2 - 30.0f;
        float width = ImGui::GetWindowWidth() - posX - 10.0f;
        if(ImGui::TreeNodeEx("Transform", treeNodeFlags)) {
            Transform& transform = proj->selectedEntity->getTransform();
            ImGui::Text("position");
            ImGui::SameLine();
            ImGui::SetCursorPosX(posX);
            ImGui::SetNextItemWidth(width);
            float pos[2] = {transform.getPosition().x, transform.getPosition().y};
            if(ImGui::DragFloat2("##position", pos, 1, 0, 0, "%.1f")) {
                proj->selectedScene->isSaved = false;
                transform.setPosition({pos[0], pos[1]});
            }

            ImGui::Text("scale");
            float scale[2] = {transform.getScale().x, transform.getScale().y};
            ImGui::SameLine();
            ImGui::SetCursorPosX(posX);
            ImGui::SetNextItemWidth(width);
            if(ImGui::DragFloat2("##Scale", scale, 1, 0, 0, "%.1f")){
                proj->selectedScene->isSaved = false;
                transform.setScale({scale[0], scale[1]});
            }
            
            ImGui::Text("rotation");
            ImGui::SameLine();
            ImGui::SetCursorPosX(posX);
            ImGui::SetNextItemWidth(width);
            float rotation = transform.getRotation();
            if(ImGui::DragFloat("##Rotation", &rotation, 1, 0, 0, "%.1f")) {
                proj->selectedScene->isSaved = false;
                transform.setRotation(rotation);
            }

            ImGui::TreePop();
        }
        TypeID toDelete = 0;
        for(auto [typeID, c] : componentsCache) {
            Class* classInfo = ClassRegistry::get().getClass(typeID);
            if(ImGui::TreeNodeEx(classInfo->getName().c_str(), treeNodeFlags)) {
                if(ImGui::BeginPopupContextItem()) {
                    if(ImGui::MenuItem("Delete")) {
                        toDelete = typeID;
                    }
                    ImGui::EndPopup();
                }
                for(auto& property : classInfo->getAllProperties()) {
                    ImGui::Text(property->getName().c_str());
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(posX);
                    ImGui::SetNextItemWidth(width);
                    if(property->getTypeID() == getTypeID<float>()) {
                        float v = property->getValue(c).as<float>();
                        if(ImGui::DragFloat(("##" + property->getName()).c_str(), &v, 1, 0, 0, "%.3f")) {
                            proj->selectedScene->isSaved = false;
                            property->setValue(c, v);
                        }
                    } else if(property->getTypeID() == getTypeID<double>()) {
                        float v = (float)property->getValue(c).as<double>();
                        if(ImGui::DragFloat(("##" + property->getName()).c_str(), &v, 1, 0, 0, "%.3f")) {
                            proj->selectedScene->isSaved = false;
                            property->setValue(c, v);
                        }
                    } else if(property->getTypeID() == getTypeID<glm::vec2>()) {
                        glm::vec2 v = property->getValue(c).as<glm::vec2>();
                        float v2[2] = {v.x, v.y};
                        if(ImGui::DragFloat2(("##" + property->getName()).c_str(), v2, 1, 0, 0, "%.3f")) {
                            proj->selectedScene->isSaved = false;
                            property->setValue(c, glm::vec2(v2[0], v2[1]));
                        }
                    } else if(property->getTypeID() == getTypeID<glm::vec3>()) {
                        glm::vec3 v = property->getValue(c).as<glm::vec3>();
                        float v3[3] = {v.x, v.y, v.z};
                        if(ImGui::DragFloat3(("##" + property->getName()).c_str(), v3, 1, 0, 0, "%.3f")) {
                            proj->selectedScene->isSaved = false;
                            property->setValue(c, glm::vec3(v3[0], v3[1], v3[2]));
                        }
                    } else if(property->getTypeID() == getTypeID<glm::vec4>()) {
                        glm::vec4 v = property->getValue(c).as<glm::vec4>();
                        float v4[4] = {v.x, v.y, v.z, v.w};
                        if(ImGui::DragFloat3(("##" + property->getName()).c_str(), v4, 1, 0, 0, "%.3f")) {
                            proj->selectedScene->isSaved = false;
                            property->setValue(c, glm::vec4(v4[0], v4[1], v4[2], v4[3]));
                        }
                    } else if(property->getTypeID() == getTypeID<bool>()) {
                        bool v = property->getValue(c).as<bool>();
                        if(ImGui::Checkbox(("##" + property->getName()).c_str(), &v)) {
                            proj->selectedScene->isSaved = false;
                            property->setValue(c, v);
                        }
                    } else if(property->getTypeID() == getTypeID<int>()) {
                        int v = property->getValue(c).as<int>();
                        if(ImGui::DragInt(("##" + property->getName()).c_str(), &v, 1, 0, 0)) {
                            proj->selectedScene->isSaved = false;
                            property->setValue(c, v);
                        }
                    } else if(property->getTypeID() == getTypeID<std::string>()) {
                        char buffer[256] = {};
                        strcpy_s(buffer, property->getValue(c).as<std::string>().c_str());
                        if(ImGui::InputText(("##" + property->getName()).c_str(), buffer, IM_ARRAYSIZE(buffer))) {
                            proj->selectedScene->isSaved = false;
                            property->setValue(c, std::string(buffer));
                        }
                    } else if(property->getTypeID() == getTypeID<Color>()) {
                        Color color = property->getValue(c).as<Color>();
                        float colorV[4] = {color.r, color.g, color.b, color.a};
                        if(ImGui::ColorEdit4(("##" + property->getName()).c_str(), colorV, ImGuiColorEditFlags_DisplayHex)) {
                            proj->selectedScene->isSaved = false;
                            property->setValue(c, Color(colorV[0], colorV[1], colorV[2], colorV[3]));
                        }
                    } else if(property->getTypeID() == getTypeID<TextureRegion>()) {
                        glm::vec4 v = property->getValue(c).as<TextureRegion>().getUVCoord();
                        float v4[4] = {v.x, v.y, v.z, v.w};
                        if(ImGui::DragFloat4(("##" + property->getName()).c_str(), v4, 0.001f, 0, 1)) {
                            proj->selectedScene->isSaved = false;
                            TextureRegion tr;
                            tr.uvMin = {v4[0], v4[1]};
                            tr.uvMax = {v4[2], v4[3]};
                            property->setValue(c, tr);
                        }
                    } else if(property->getTypeID() == getTypeID<ResPtr<Texture2D>>()) {
                        ResPtr<Texture2D> res = property->getValue(c).as<ResPtr<Texture2D>>();
                        if(res) {
                            glm::vec2 size = res->getSize() * (100.0f / (float)std::max(res->getWidth(), res->getHeight()));
                            ImGui::Image(res->getId(), toImVec2(size), {0, 1}, {1, 0});
                        }else {
                            ImGui::Text("None");
                        }
                        if(ImGui::BeginDragDropTarget()) {
                            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetRUID")) {
                                RUID ruid = *(RUID*)payload->Data;
                                if(getResourceType(ruid) == ResourceType::Texture) {
                                    proj->selectedScene->isSaved = false;
                                    property->setValue(c, ResPtr<Texture2D>(ruid));
                                }
                            }
                            ImGui::EndDragDropTarget();
                        }
                    } else {
                        ImGui::Text("Failed to display");
                    }
                }
                ImGui::TreePop();
            }
        }
        if(toDelete) {
            proj->selectedEntity->removeComponent(toDelete);
            proj->selectedScene->isSaved = false;
            updateCache();
        }

        float w = ImGui::GetContentRegionAvail().x;
        static bool show = false;
        if(ImGui::Button("Add Component", {w, 0})) {
            ImGui::OpenPopup("addComponent");
        }
        if(ImGui::BeginPopup("addComponent")) {
            if(ImGui::MenuItem("SpriteRender")) {
                proj->selectedEntity->addComponent<SpriteRender>();
                updateCache();
            }
            if(ImGui::MenuItem("Camera2D")) {
                proj->selectedEntity->addComponent<Camera2D>();
                updateCache();
            }
            if(ImGui::MenuItem("Animation")) {
                proj->selectedEntity->addComponent<Animation>();
                updateCache();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void EntityPropertyPanel::updateCache() {
    componentsCache.clear();
    for(auto& c : proj->selectedEntity->getComponents()) {
        auto it = std::find_if(proj->selectedEntity->getComponentsMap().begin(), proj->selectedEntity->getComponentsMap().end(), [&c](const auto& pair) {
            return pair.second == c.get();
        });
        componentsCache.emplace_back(std::pair<TypeID, Component*>{it->first, c.get()});
    }
}