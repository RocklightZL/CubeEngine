#include "SceneView.h"

#include "../Project.h"
#include "../App/EditorApp.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/Renderer.h"
#include "../Game.h"
#include "../Utils/ImGuiExternal.h"
#include "../Utils/misc.h"

#include <imgui/imgui.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <thread>

extern Cube::Project* proj;
extern Cube::EditorApp* app;

namespace Cube {

    SceneView::SceneView() {
        frameBuffer = new FrameBuffer();
        frameBuffer->bindAttachment((int)sceneViewSize.x, (int)sceneViewSize.y);
        editorRenderSystem = new EditorRenderSystem();
    }

    SceneView::~SceneView() {
        delete frameBuffer;
        delete editorRenderSystem;
    }

    void SceneView::render(float deltaTime) {
        ImGui::Begin("Scene View");

        ImGui::BeginChild("ToolBar", {ImGui::GetWindowWidth(), 45});
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        static bool isGameWindowOpened = false;
        ImVec2 toolButtonSize(37, 37);
        if(ImGui::ImageButton("play", app->icons["play.png"]->getId(), toolButtonSize, ImVec2(0, 1), ImVec2(1, 0)) && !isGameWindowOpened) {
            isGameWindowOpened = true;
            std::thread gameThread(gameThreadFunction, &isGameWindowOpened);
            gameThread.detach();
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::BeginChild("Scene");
        if(proj->selectedScene) {
            ImVec2 currentSize = ImGui::GetContentRegionAvail();
            if(currentSize.x <= 0) currentSize.x = 1;
            if(currentSize.y <= 0) currentSize.y = 1;
            if((int)currentSize.x != (int)sceneViewSize.x || (int)currentSize.y != (int)sceneViewSize.y) {
                sceneViewSize = currentSize;
                frameBuffer->resize((int)sceneViewSize.x, (int)sceneViewSize.y);
            }
            proj->selectedScene->scene->setViewportSize({sceneViewSize.x, sceneViewSize.y});

            frameBuffer->bind();
            Renderer2D::setViewport((int)sceneViewSize.x, (int)sceneViewSize.y);
            Renderer2D::clearBuffer();
            // scene render
            editorRenderSystem->onUpdate(proj->selectedScene->scene, deltaTime);

            FrameBuffer::bindDefaultFrameBuffer();

            static bool showSelectSubTexturePopup = false;
            static std::shared_ptr<TextureData> textureData;
            static glm::vec2 pos;
            static std::string texturePath;
            ImGui::Image(frameBuffer->getTexture(), sceneViewSize, ImVec2(0, 1), ImVec2(1, 0));
            if(ImGui::BeginDragDropTarget()) {
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TexturePath")) {
                    texturePath = (const char*)payload->Data;
                    pos = glm::vec2(ImGui::GetMousePos().x - ImGui::GetWindowPos().x, ImGui::GetWindowSize().y - (ImGui::GetMousePos().y - ImGui::GetWindowPos().y));
                    if(Utils::isFileExists(texturePath + ".meta")) {
                        showSelectSubTexturePopup = true;
                        textureData = std::make_shared<TextureData>(texturePath + ".meta", texturePath);
                    }else{
                        Texture2D* texture = ResourceManager::getInstance().load<Texture2D>(texturePath)->data;
                        Scene* scene = proj->selectedScene->scene;
                        Entity* entity = scene->createEntity(texturePath);
                        TransformComponent* tc = entity->addComponent<TransformComponent>();
                        tc->scale = {texture->getWidth(), texture->getHeight()};
                        tc->position = pos;
                        SpriteComponent* sc = entity->addComponent<SpriteComponent>();
                        sc->texture = texture;
                        proj->selectedScene->isSaved = false;
                    }
                }
                ImGui::EndDragDropTarget();
            }
            if(showSelectSubTexturePopup && textureData) {
                if(auto* subTexture = selectSubTexturePopup(*textureData, &showSelectSubTexturePopup)) {
                    Texture2D* texture = ResourceManager::getInstance().load<Texture2D>(texturePath)->data;
                    Scene* scene = proj->selectedScene->scene;
                    Entity* entity = scene->createEntity(subTexture->name);
                    TransformComponent* tc = entity->addComponent<TransformComponent>();
                    tc->scale = subTexture->size;
                    tc->position = pos;
                    SpriteComponent* sc = entity->addComponent<SpriteComponent>();
                    sc->texture = texture;
                    sc->region = {subTexture->uvMin, subTexture->uvMax};
                    proj->selectedScene->isSaved = false;
                }
            }

            if(ImGui::IsWindowFocused()) {
                EditorCamera& editorCamera = proj->editorCamera;
                if(ImGui::IsKeyDown(ImGuiKey_LeftArrow)) {
                    editorCamera.position.x -= deltaTime * 500;
                }
                if(ImGui::IsKeyDown(ImGuiKey_RightArrow)) {
                    editorCamera.position.x += deltaTime * 500;
                }
                if(ImGui::IsKeyDown(ImGuiKey_UpArrow)) {
                    editorCamera.position.y += deltaTime * 500;
                }
                if(ImGui::IsKeyDown(ImGuiKey_DownArrow)) {
                    editorCamera.position.y -= deltaTime * 500;
                }

                static bool isPanning = false;
                ImGuiIO& io = ImGui::GetIO();
                if(ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
                    isPanning = true;
                }
                if(isPanning) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
                    glm::vec2 delta = {-io.MouseDelta.x * editorCamera.scale.x, io.MouseDelta.y * editorCamera.scale.y};
                    editorCamera.position += delta;

                    if(!ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
                        isPanning = false;
                    }
                }

                if(io.MouseWheel != 0.0f) {
                    editorCamera.scale += io.MouseWheel * 0.1f;
                }
                static bool isDragging = false;
                if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
                    bool choose = false;
                    glm::vec2 mousePos = {io.MousePos.x - ImGui::GetWindowPos().x, ImGui::GetWindowSize().y - (io.MousePos.y - ImGui::GetWindowPos().y)};
                    mousePos += editorCamera.position / editorCamera.scale;
                    for(auto& e : proj->selectedScene->scene->getEntitiesWith<TransformComponent, SpriteComponent>()) {
                        TransformComponent* tc = e->getComponent<TransformComponent>();
                        glm::vec2 min = {tc->position.x - tc->scale.x / 2, tc->position.y - tc->scale.y / 2};
                        glm::vec2 max = {tc->position.x + tc->scale.x / 2, tc->position.y + tc->scale.y / 2};
                        glm::vec4 tMin = glm::inverse(editorCamera.getTransformMatrix()) * glm::vec4(min, 0, 0);
                        glm::vec4 tMax = glm::inverse(editorCamera.getTransformMatrix()) * glm::vec4(max, 0, 0);
                        if(mousePos.x >= tMin.x && mousePos.x <= tMax.x && mousePos.y >= tMin.y && mousePos.y <= tMax.y) {
                            if(proj->selectedEntity == e) {
                                isDragging = true;
                            }
                            proj->selectedEntity = e;
                            choose = true;
                            break;
                        }
                    }
                    if(!choose) proj->selectedEntity = nullptr;
                }
                if(isDragging) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    glm::vec2 delta = {io.MouseDelta.x * editorCamera.scale.x, -io.MouseDelta.y * editorCamera.scale.y};
                    if(proj->selectedEntity) {
                        proj->selectedEntity->getComponent<TransformComponent>()->position += delta;
                        proj->selectedScene->isSaved = false;
                    }
                    if(!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                        isDragging = false;
                    }
                }
            }
        }
        ImGui::EndChild();
        ImGui::End();
    }

    SubTexture* SceneView::selectSubTexturePopup(TextureData& data, bool* open) {
        SubTexture* res = nullptr;
        static SubTexture* current = nullptr;
        if(*open){
            ImGui::OpenPopup("Select SubTexture");
        }else {
            current = nullptr;
        }
        ImVec4 originalBorderColor = ImGui::GetStyle().Colors[ImGuiCol_Border];
        static ImVec4 borderColor = originalBorderColor;
        constexpr ImVec4 highLightColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
        if(ImGui::BeginPopupModal("Select SubTexture", open, ImGuiWindowFlags_NoResize)) {
            Texture2D* texture = ResourceManager::getInstance().load<Texture2D>(data.imagePath)->data;
            const ImVec4 originalBgColor = ImGui::GetStyle().Colors[ImGuiCol_ChildBg];
            const ImVec4 hoveredBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            const ImVec4 selectedBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
            int i = 0;
            static int selected = -1;
            static int hovered = -1;
            for(SubTexture& st : data.textures) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, i == hovered ? hoveredBgColor : (i == selected ? selectedBgColor : originalBorderColor));
                if(i % 5) ImGui::SameLine();
                ImGui::BeginChild(st.name.c_str(), {200, 275});
                ImGui::Image(texture->getId(), ImVec2(st.size.x, st.size.y) * (200 / max(st.size.x, st.size.y)), ImVec2(st.uvMin.x, st.uvMin.y + st.size.y / data.size.y), ImVec2(st.uvMax.x, st.uvMax.y - st.size.y / data.size.y));
                ImGui::SetCursorPos(ImVec2(0, 200));
                ImGui::TextWrapped(st.name.c_str());
                if(ImGui::IsWindowHovered()) {
                    hovered = i;
                    if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                        current = &st;
                        selected = i;
                    }
                }else if(hovered == i) {
                    hovered = -1;
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ++i;
            }

            static bool showWarning = false;
            if(showWarning){
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Text("Please select a subtexture!");
                ImGui::PopStyleColor();
                ImGui::SameLine();
            }

            constexpr float buttonWidth = 100.0f;
            constexpr float spacing = 100.0f;
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (buttonWidth * 2 + ImGui::GetStyle().FramePadding.x * 2 + spacing));
            ImGui::BeginGroup();
            if(ImGui::Button("OK", ImVec2(buttonWidth, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
                if(current != nullptr){
                    *open = false;
                    res = current;
                    showWarning = false;
                    ImGui::CloseCurrentPopup();
                }else {
                    showWarning = true;
                }
            }
            ImGui::SameLine(0.0f, spacing);
            if(ImGui::Button("Cancel", ImVec2(buttonWidth, 0))) {
                *open = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndGroup();

            if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                ImVec2 mousePos = ImGui::GetMousePos();
                ImVec2 winPos = ImGui::GetWindowPos();
                ImVec2 size = ImGui::GetWindowSize();
                if(!(mousePos.x >= winPos.x && mousePos.y >= winPos.y && mousePos.x <= winPos.x + size.x && mousePos.y <= winPos.y + size.y)) {
                    borderColor = highLightColor;
                    MessageBeep(MB_ICONASTERISK); //TODO: ¿çÆ½Ì¨ÊÊÅä
                }else {
                    borderColor = originalBorderColor;
                }
            }

            ResourceManager::getInstance().release(data.imagePath);
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor();
        return res;
    }

}  // namespace Cube