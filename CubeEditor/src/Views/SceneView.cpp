#include "SceneView.h"

#include <imgui/imgui.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <thread>

#include "../App/EditorPage.h"
#include "../App/EditorApp.h"
#include "../Game.h"
#include "../Project/Project.h"
#include "../Utils/ImGuiExternal.h"
#include "../Utils/EditorTextureCache.h"
#include "../Utils/misc.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Scene/Camera2D.h"
#include "Cube/Scene/SpriteRender.h"
#include "Cube/Utils/Utils.h"

using namespace Cube;

SceneView::SceneView(EditorPage& editorPage) : View(editorPage) {
    frameBuffer = new FrameBuffer();
    frameBuffer->bindAttachment((int)sceneViewSize.x, (int)sceneViewSize.y);
}

SceneView::~SceneView() {
    delete frameBuffer;
}

void SceneView::render(float deltaTime) {
    Texture2D* play_png = EditorTextureCache::get().request("assets/icons/play.png");

    ImGui::Begin("Scene View");
    
    ImGui::BeginChild("ToolBar", {ImGui::GetWindowWidth(), 45});
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImVec2 toolButtonSize(37, 37);
    EditorApp* app = &EditorApp::get();
    static bool isGameOver = true;
    if(ImGui::ImageButton("play", play_png->getId(), toolButtonSize, ImVec2(0, 1), ImVec2(1, 0)) && isGameOver) {
        isGameOver = false;
        if(app->gameThread.joinable()) {
            app->gameThread.join();
        }
        app->gameThread = std::thread(gameThreadFunction,
            app,
            editorPage.getProject(),
            editorPage.selectedScene->scene,
            &isGameOver);
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset")) {
        editorPage.editorCamera.position = {0, 0};
        editorPage.editorCamera.zoom = 1.0f;
        editorPage.editorCamera.viewport = toGlmVec2(sceneViewSize);
    }
    ImGui::PopStyleColor();
    ImGui::EndChild();
    
    ImGui::BeginChild("Scene");
    if(editorPage.selectedScene) {
        ImVec2 currentSize = ImGui::GetContentRegionAvail();
        if(currentSize.x <= 0) currentSize.x = 1;
        if(currentSize.y <= 0) currentSize.y = 1;
        if((int)currentSize.x != (int)sceneViewSize.x || (int)currentSize.y != (int)sceneViewSize.y) {
            sceneViewSize = currentSize;
            editorPage.editorCamera.viewport = {sceneViewSize.x, sceneViewSize.y};
            frameBuffer->resize((int)sceneViewSize.x, (int)sceneViewSize.y);
        }

        editorPage.selectedScene->scene->update(deltaTime);
    
        frameBuffer->bind();
        Renderer2D::setViewport((int)sceneViewSize.x, (int)sceneViewSize.y);
        Renderer2D::setClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        Renderer2D::clearBuffer();
        // scene render
        sceneRender(deltaTime);
    
        FrameBuffer::bindDefaultFrameBuffer();
    
        static bool showSelectSubTexturePopup = false;
        ImGui::Image(frameBuffer->getTexture(), sceneViewSize, ImVec2(0, 1), ImVec2(1, 0));
        if(ImGui::BeginDragDropTarget()) {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
                AssetNode* asset = *(AssetNode**)payload->Data;
                glm::vec2 pos = glm::vec2(ImGui::GetMousePos().x - ImGui::GetWindowPos().x, ImGui::GetWindowSize().y - (ImGui::GetMousePos().y - ImGui::GetWindowPos().y));
                pos *= editorPage.editorCamera.zoom;
                pos += editorPage.editorCamera.position;
                if(asset->type == ResourceType::Texture) {
                    auto e = editorPage.selectedScene->scene->createEntity(asset->identifier);
                    e->getTransform().setPosition(pos);
                    auto spriteRender = e->addComponent<SpriteRender>();
                    spriteRender->sprite = ResPtr<Sprite>("spr:" + asset->identifier);
                }
            }
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetSprite")){
                std::string spriteIdentifier((char*)payload->Data, payload->DataSize);
                size_t posStr = spriteIdentifier.find('#');
                if(posStr != std::string::npos) {
                    glm::vec2 pos = glm::vec2(ImGui::GetMousePos().x - ImGui::GetWindowPos().x, ImGui::GetWindowSize().y - (ImGui::GetMousePos().y - ImGui::GetWindowPos().y));
                    pos *= editorPage.editorCamera.zoom;
                    pos += editorPage.editorCamera.position;
                    std::string spriteName = spriteIdentifier.substr(posStr + 1);
                    auto e = editorPage.selectedScene->scene->createEntity(spriteName);
                    e->getTransform().setPosition(pos);
                    auto spriteRender = e->addComponent<SpriteRender>();
                    spriteRender->sprite = ResPtr<Sprite>(spriteIdentifier);
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
            EditorCamera& editorCamera = editorPage.editorCamera;
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
                for(auto& e : editorPage.selectedScene->scene->getEntitiesWith<SpriteRender>()) {
                    Transform& tc = e->getTransform();
                    SpriteRender* sprite = e->getComponent<SpriteRender>();
                    glm::vec2 position = tc.getWorldPos();
                    glm::vec2 scale = sprite->sprite->getSize() * tc.getWorldScale();
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
                    if(editorPage.selectedEntity == selected) {
                        isDragging = true;
                    }
                    editorPage.selectedEntity = selected;
                    choose = true;
                }
                if(!choose) editorPage.selectedEntity = nullptr;
            }
            if(isDragging) {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                glm::vec2 delta = {io.MouseDelta.x * editorCamera.zoom, -io.MouseDelta.y * editorCamera.zoom};
                if(editorPage.selectedEntity) {
                    Transform& tc = editorPage.selectedEntity->getTransform();
                    tc.setPosition(tc.getPosition() + delta);
                    editorPage.selectedScene->isSaved = false;
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

void SceneView::sceneRender(float deltaTime) {
    Scene* scene = editorPage.selectedScene->scene;
    auto sprites = scene->getEntitiesWith<SpriteRender>();
    std::sort(sprites.begin(), sprites.end(), [](const Entity* a, const Entity* b) {
        SpriteRender* spriteA = a->getComponent<SpriteRender>();
        SpriteRender* spriteB = b->getComponent<SpriteRender>();
        if(spriteA->order != spriteB->order) {
            return spriteA->order < spriteB->order;
        }
        return (spriteA->sprite->getTexture() ? spriteA->sprite->getTexture()->getId() : -1) < (spriteB->sprite->getTexture() ? spriteB->sprite->getTexture()->getId() : -1);
    });
    auto cameras = scene->getEntitiesWith<Camera2D>();
    
    const EditorCamera& editorCamera = editorPage.editorCamera;
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
        Renderer2D::drawQuad(e->getTransform().getWorldMatrix(), sc->tintColor, sc->sprite->getTexture(), sc->sprite->getTexRegion().getUVCoord());
    }

    // the outline of selected entity
    if(editorPage.selectedEntity && editorPage.selectedEntity->hasComponent<SpriteRender>()) {
        auto* selectEntityTC = &editorPage.selectedEntity->getTransform();
        glm::vec2 spriteSize = editorPage.selectedEntity->getComponent<SpriteRender>()->sprite->getSize();
        Renderer2D::drawQuad(selectEntityTC->getWorldPos(), glm::vec2(selectEntityTC->getWorldScale().x * spriteSize.x, 1) * glm::vec2(1, editorCamera.zoom), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
        Renderer2D::drawQuad(selectEntityTC->getWorldPos() + glm::vec2(0, selectEntityTC->getWorldScale().y * spriteSize.y), glm::vec2(selectEntityTC->getWorldScale().x * spriteSize.x, 1) * glm::vec2(1, editorCamera.zoom), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
        Renderer2D::drawQuad(selectEntityTC->getWorldPos(), glm::vec2(1, selectEntityTC->getWorldScale().y * spriteSize.y) * glm::vec2(editorCamera.zoom, 1), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
        Renderer2D::drawQuad(selectEntityTC->getWorldPos() + glm::vec2(selectEntityTC->getWorldScale().x * spriteSize.x, 0), glm::vec2(1, selectEntityTC->getWorldScale().y * spriteSize.y) * glm::vec2(editorCamera.zoom, 1), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
    }

    Renderer2D::endFrame();
}