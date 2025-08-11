#include "SceneView.h"

#include "../Project.h"
#include "../App/EditorApp.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/Renderer.h"

#include "imgui/imgui.h"

#include <glm/ext/matrix_clip_space.hpp>

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

            ImGui::Image(frameBuffer->getTexture(), sceneViewSize, ImVec2(0, 1), ImVec2(1, 0));

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
        ImGui::End();
    }

    void SceneView::selectSubTexturePopup(const TextureData& data, SubTexture* selected, bool* open) {
        if(*open){
            ImGui::OpenPopup("Select SubTexture");
        }
        if(ImGui::BeginPopupModal("Select SubTexture", open)) {
            Texture2D* texture = ResourceManager::getInstance().load<Texture2D>(data.imagePath)->data;
            for(const SubTexture& st : data.textures) {
                ImGui::BeginGroup();
                ImGui::Image(texture->getId(), ImVec2(100, 100), ImVec2(st.uvMin.x, st.uvMin.y + st.size.y / data.size.y), ImVec2(st.uvMax.x, st.uvMax.y - st.size.y / data.size.y));
                ImGui::Text(st.name.c_str());
                ImGui::EndGroup();
            }

            if(ImGui::Button("OK")) {
                *open = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if(ImGui::Button("Cancel")) {
                *open = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
            ResourceManager::getInstance().release(data.imagePath);
        }
    }

}  // namespace Cube