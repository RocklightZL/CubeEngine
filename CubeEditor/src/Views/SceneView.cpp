#include "SceneView.h"

#include <imgui/imgui.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <thread>

#include "../App/EditorApp.h"
#include "../Game.h"
#include "../Project.h"
#include "../Utils/ImGuiExternal.h"
#include "../Utils/misc.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Scene/Camera2D.h"
#include "Cube/Scene/SpriteRender.h"
#include "Cube/Utils/Utils.h"

using namespace Cube;

extern Project* proj;
extern EditorApp* app;

SceneView::SceneView() {
    frameBuffer = new FrameBuffer();
    frameBuffer->bindAttachment((int)sceneViewSize.x, (int)sceneViewSize.y);
}

SceneView::~SceneView() {
    delete frameBuffer;
}

void SceneView::render(float deltaTime) {
    ImGui::Begin("Scene View");
    
    ImGui::BeginChild("ToolBar", {ImGui::GetWindowWidth(), 45});
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImVec2 toolButtonSize(37, 37);
    static bool isGameOver = true;
    if(ImGui::ImageButton("play", play_png->getId(), toolButtonSize, ImVec2(0, 1), ImVec2(1, 0)) && isGameOver) {
        isGameOver = false;
        if(app->gameThread.joinable()) {
            app->gameThread.join();
        }
        app->gameThread = std::thread(gameThreadFunction, &isGameOver);
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset")) {
        proj->editorCamera.position = {0, 0};
        proj->editorCamera.zoom = 1.0f;
        proj->editorCamera.viewport = toGlmVec2(sceneViewSize);
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
            proj->editorCamera.viewport = {sceneViewSize.x, sceneViewSize.y};
            frameBuffer->resize((int)sceneViewSize.x, (int)sceneViewSize.y);
        }

        proj->selectedScene->scene->update(deltaTime);
    
        frameBuffer->bind();
        Renderer2D::setViewport((int)sceneViewSize.x, (int)sceneViewSize.y);
        Renderer2D::setClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        Renderer2D::clearBuffer();
        // scene render
        sceneRender(deltaTime);
    
        FrameBuffer::bindDefaultFrameBuffer();
    
        static bool showSelectSubTexturePopup = false;
        static std::shared_ptr<TextureData> textureData;
        ImGui::Image(frameBuffer->getTexture(), sceneViewSize, ImVec2(0, 1), ImVec2(1, 0));
        if(ImGui::BeginDragDropTarget()) {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetRUID")) {
                RUID ruid = *(RUID*)payload->Data;
                glm::vec2 pos = glm::vec2(ImGui::GetMousePos().x - ImGui::GetWindowPos().x, ImGui::GetWindowSize().y - (ImGui::GetMousePos().y - ImGui::GetWindowPos().y));
                pos += proj->editorCamera.position / proj->editorCamera.zoom;
                if(getResourceType(ruid) == ResourceType::Texture) {
                    auto e = proj->selectedScene->scene->createEntity(std::filesystem::path(ResourceManager::get().getAssetMeta(ruid)->sourcePath).filename().string());
                    e->getTransform().setPosition(pos);
                    auto sprite = e->addComponent<SpriteRender>();
                    sprite->texture = ResPtr<Texture2D>(ruid);
                }
            }
            ImGui::EndDragDropTarget();
        }
        // if(showSelectSubTexturePopup && textureData) {
        //     if(auto* subTexture = selectSubTexturePopup(*textureData, &showSelectSubTexturePopup)) {
        //         Texture2D* texture = ResourceManager::get().load<Texture2D>(texturePath)->data;
        //         Scene* scene = proj->selectedScene->scene;
        //         Entity* entity = scene->createEntity(subTexture->name);
        //         TransformComponent* tc = entity->addComponent<TransformComponent>();
        //         tc->scale = subTexture->size;
        //         tc->position = pos;
        //         SpriteComponent* sc = entity->addComponent<SpriteComponent>();
        //         sc->texture = texture;
        //         sc->region = {subTexture->uvMin, subTexture->uvMax};
        //         proj->selectedScene->isSaved = false;
        //     }
        // }
    
        if(ImGui::IsWindowFocused() && ImGui::IsWindowHovered()) {
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
                glm::vec2 delta = {-io.MouseDelta.x * editorCamera.zoom, io.MouseDelta.y * editorCamera.zoom};
                editorCamera.position += delta;
    
                if(!ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
                    isPanning = false;
                }
            }
    
            if(io.MouseWheel != 0.0f) {
                glm::vec2 mousePos = {io.MousePos.x - ImGui::GetWindowPos().x, ImGui::GetWindowSize().y - (io.MousePos.y - ImGui::GetWindowPos().y)};
                glm::vec2 mouseWorldPos = mousePos * editorCamera.zoom + editorCamera.position;
                static constexpr float E = 0.08f;
                float k = std::pow(1.0f + E, io.MouseWheel);
                editorCamera.zoom = editorCamera.zoom * k;
                editorCamera.position = mouseWorldPos - mousePos * editorCamera.zoom;
            }
            static bool isDragging = false;
            if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
                bool choose = false;
                glm::vec2 mousePos = {io.MousePos.x - ImGui::GetWindowPos().x, ImGui::GetWindowSize().y - (io.MousePos.y - ImGui::GetWindowPos().y)};
                mousePos += editorCamera.position / editorCamera.zoom;
                Entity* selected = nullptr;
                for(auto& e : proj->selectedScene->scene->getEntitiesWith<SpriteRender>()) {
                    Transform& tc = e->getTransform();
                    SpriteRender* sprite = e->getComponent<SpriteRender>();
                    glm::vec2 position = tc.getWorldPos();
                    glm::vec2 scale = sprite->getSize() * tc.getWorldScale();
                    glm::vec2 min = {position.x, position.y};
                    glm::vec2 max = {position.x + scale.x, position.y + scale.y};
                    glm::vec4 tMin = glm::inverse(editorCamera.getTransformMatrix()) * glm::vec4(min, 0, 0);
                    glm::vec4 tMax = glm::inverse(editorCamera.getTransformMatrix()) * glm::vec4(max, 0, 0);
                    if(mousePos.x >= tMin.x && mousePos.x <= tMax.x && mousePos.y >= tMin.y && mousePos.y <= tMax.y) {
                        if(!selected || sprite->order > selected->getComponent<SpriteRender>()->order) {
                            selected = e;
                        }
                    }
                }
                if(selected) {
                    if(proj->selectedEntity == selected) {
                        isDragging = true;
                    }
                    proj->selectedEntity = selected;
                    choose = true;
                }
                if(!choose) proj->selectedEntity = nullptr;
            }
            if(isDragging) {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                glm::vec2 delta = {io.MouseDelta.x * editorCamera.zoom, -io.MouseDelta.y * editorCamera.zoom};
                if(proj->selectedEntity) {
                    Transform& tc = proj->selectedEntity->getTransform();
                    tc.setPosition(tc.getPosition() + delta);
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
    // static SubTexture* current = nullptr;
    // if(*open) {
    //     ImGui::OpenPopup("Select SubTexture");
    // } else {
    //     current = nullptr;
    // }
    // ImVec4 originalBorderColor = ImGui::GetStyle().Colors[ImGuiCol_Border];
    // static ImVec4 borderColor = originalBorderColor;
    // constexpr ImVec4 highLightColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    // ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
    // if(ImGui::BeginPopupModal("Select SubTexture", open, ImGuiWindowFlags_NoResize)) {
    //     Texture2D* texture = ResourceManager::get().load<Texture2D>(data.imagePath)->data;
    //     const ImVec4 originalBgColor = ImGui::GetStyle().Colors[ImGuiCol_ChildBg];
    //     const ImVec4 hoveredBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
    //     const ImVec4 selectedBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
    //     int i = 0;
    //     static int selected = -1;
    //     static int hovered = -1;
    //     for(SubTexture& st : data.textures) {
    //         ImGui::PushStyleColor(ImGuiCol_ChildBg, i == hovered ? hoveredBgColor : (i == selected ? selectedBgColor : originalBorderColor));
    //         if(i % 5) ImGui::SameLine();
    //         ImGui::BeginChild(st.name.c_str(), {200, 275});
    //         ImGui::Image(texture->getId(), ImVec2(st.size.x, st.size.y) * (200 / max(st.size.x, st.size.y)), ImVec2(st.uvMin.x, st.uvMin.y + st.size.y / data.size.y), ImVec2(st.uvMax.x, st.uvMax.y - st.size.y / data.size.y));
    //         ImGui::SetCursorPos(ImVec2(0, 200));
    //         ImGui::TextWrapped(st.name.c_str());
    //         if(ImGui::IsWindowHovered()) {
    //             hovered = i;
    //             if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    //                 current = &st;
    //                 selected = i;
    //             }
    //         } else if(hovered == i) {
    //             hovered = -1;
    //         }
    //         ImGui::EndChild();
    //         ImGui::PopStyleColor();
    //         ++i;
    //     }
    //
    //     static bool showWarning = false;
    //     if(showWarning) {
    //         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    //         ImGui::Text("Please select a subtexture!");
    //         ImGui::PopStyleColor();
    //         ImGui::SameLine();
    //     }
    //
    //     constexpr float buttonWidth = 100.0f;
    //     constexpr float spacing = 100.0f;
    //     ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (buttonWidth * 2 + ImGui::GetStyle().FramePadding.x * 2 + spacing));
    //     ImGui::BeginGroup();
    //     if(ImGui::Button("OK", ImVec2(buttonWidth, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
    //         if(current != nullptr) {
    //             *open = false;
    //             res = current;
    //             showWarning = false;
    //             ImGui::CloseCurrentPopup();
    //         } else {
    //             showWarning = true;
    //         }
    //     }
    //     ImGui::SameLine(0.0f, spacing);
    //     if(ImGui::Button("Cancel", ImVec2(buttonWidth, 0))) {
    //         *open = false;
    //         ImGui::CloseCurrentPopup();
    //     }
    //     ImGui::EndGroup();
    //
    //     if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    //         ImVec2 mousePos = ImGui::GetMousePos();
    //         ImVec2 winPos = ImGui::GetWindowPos();
    //         ImVec2 size = ImGui::GetWindowSize();
    //         if(!(mousePos.x >= winPos.x && mousePos.y >= winPos.y && mousePos.x <= winPos.x + size.x && mousePos.y <= winPos.y + size.y)) {
    //             borderColor = highLightColor;
    //             MessageBeep(MB_ICONASTERISK);  // TODO: ¿çÆ½Ì¨ÊÊÅä
    //         } else {
    //             borderColor = originalBorderColor;
    //         }
    //     }
    //
    //     ResourceManager::get().release(data.imagePath);
    //     ImGui::EndPopup();
    // }
    // ImGui::PopStyleColor();
    return res;
}

void SceneView::sceneRender(float deltaTime) {
    Scene* scene = proj->selectedScene->scene;
    auto sprites = scene->getEntitiesWith<SpriteRender>();
    std::sort(sprites.begin(), sprites.end(), [](const Entity* a, const Entity* b) {
        SpriteRender* spriteA = a->getComponent<SpriteRender>();
        SpriteRender* spriteB = b->getComponent<SpriteRender>();
        if(spriteA->order != spriteB->order) {
            return spriteA->order < spriteB->order;
        }
        return (spriteA->texture.get() ? spriteA->texture->getId() : -1) < (spriteB->texture.get() ? spriteB->texture->getId() : -1);
    });
    auto cameras = scene->getEntitiesWith<Camera2D>();
    
    const EditorCamera& editorCamera = proj->editorCamera;
    Renderer2D::beginFrame(editorCamera.getPVMatrix());
    // the axis lines
    Renderer2D::drawQuad({0, -15000}, glm::vec2(1, 30000) * editorCamera.zoom, nullptr, {1.0f, 0.0f, 0.0f, 1.0f});
    Renderer2D::drawQuad({-15000, 0}, glm::vec2(30000, 1) * editorCamera.zoom, nullptr, {0.0f, 0.0f, 1.0f, 1.0f});
    
    for(auto& camera : cameras) {
        auto* tc = &camera->getTransform();
        auto* cc = camera->getComponent<Camera2D>();
        if(cc->available) {
            glm::vec2 size = cc->viewport;
            glm::vec2 pos = tc->getWorldPos();
            Color color = {113, 96, 232, 255};
            Renderer2D::drawQuad(pos, glm::vec2(size.x, 1) * glm::vec2(1, editorCamera.zoom), nullptr, color);
            Renderer2D::drawQuad(pos + glm::vec2(0, size.y), glm::vec2(size.x, 1) * glm::vec2(1, editorCamera.zoom), nullptr, color);
            Renderer2D::drawQuad(pos, glm::vec2(1, size.y) * glm::vec2(editorCamera.zoom, 1), nullptr, color);
            Renderer2D::drawQuad(pos + glm::vec2(size.x, 0), glm::vec2(1, size.y) * glm::vec2(editorCamera.zoom, 1), nullptr, color);
        }
    }

    for(auto& e : sprites) {
        auto* sc = e->getComponent<SpriteRender>();
        Renderer2D::drawQuad(e->getTransform().getWorldMatrix(), sc->tintColor, sc->texture.get(), sc->texRegion.getUVCoord());
    }

    // the outline of selected entity
    if(proj->selectedEntity && proj->selectedEntity->hasComponent<SpriteRender>()) {
        auto* selectEntityTC = &proj->selectedEntity->getTransform();
        glm::vec2 spriteSize = proj->selectedEntity->getComponent<SpriteRender>()->getSize();
        Renderer2D::drawQuad(selectEntityTC->getWorldPos(), glm::vec2(selectEntityTC->getWorldScale().x * spriteSize.x, 1) * glm::vec2(1, editorCamera.zoom), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
        Renderer2D::drawQuad(selectEntityTC->getWorldPos() + glm::vec2(0, selectEntityTC->getWorldScale().y * spriteSize.y), glm::vec2(selectEntityTC->getWorldScale().x * spriteSize.x, 1) * glm::vec2(1, editorCamera.zoom), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
        Renderer2D::drawQuad(selectEntityTC->getWorldPos(), glm::vec2(1, selectEntityTC->getWorldScale().y * spriteSize.y) * glm::vec2(editorCamera.zoom, 1), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
        Renderer2D::drawQuad(selectEntityTC->getWorldPos() + glm::vec2(selectEntityTC->getWorldScale().x * spriteSize.x, 0), glm::vec2(1, selectEntityTC->getWorldScale().y * spriteSize.y) * glm::vec2(editorCamera.zoom, 1), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
    }

    Renderer2D::endFrame();
}