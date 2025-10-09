#include "EditorRenderSystem.h"

#include "../Project.h"
#include "Cube/Renderer/Renderer.h"

#include <glm/ext/matrix_clip_space.hpp>

extern Cube::Project* proj;

namespace Cube {

    void EditorRenderSystem::onUpdate(Scene* scene, float deltaTime) {
        auto sprites = scene->getEntitiesWith<TransformComponent, SpriteComponent>();
        std::sort(sprites.begin(), sprites.end(), [](Entity* a, Entity* b) {
            auto a1 = a->getComponent<SpriteComponent>()->texture ? a->getComponent<SpriteComponent>()->texture->getId() : -1;
            auto b1 = b->getComponent<SpriteComponent>()->texture ? b->getComponent<SpriteComponent>()->texture->getId() : -1;
            return a1 < b1;
        });
        auto cameras = scene->getEntitiesWith<TransformComponent, CameraComponent>();

        const EditorCamera& editorCamera = proj->editorCamera;
        glm::mat4 pvMatrix = glm::ortho(0.0f, scene->getViewportSize().x, 0.0f, scene->getViewportSize().y, 0.0f, 1.0f) * glm::inverse(editorCamera.getTransformMatrix());
        Renderer2D::beginFrame(pvMatrix);
        // 坐标系标线
        Renderer2D::drawQuad({0, 0}, glm::vec2(1, 30000) * editorCamera.scale, nullptr, {1.0f, 0.0f, 0.0f, 1.0f});
        Renderer2D::drawQuad({0, 0}, glm::vec2(30000, 1) * editorCamera.scale, nullptr, {0.0f, 0.0f, 1.0f, 1.0f});
        for(auto& e : sprites) {
            auto* sc = e->getComponent<SpriteComponent>();
            auto* tc = e->getComponent<TransformComponent>();
            Renderer2D::drawQuad(tc->getTransformMatrix(), sc->color, sc->texture, glm::vec4(sc->region.uvMin, sc->region.uvMax));
        }
        for(auto& camera : cameras) {
            auto* tc = camera->getComponent<TransformComponent>();
            auto* cc = camera->getComponent<CameraComponent>();
            if(cc->available) {
                glm::vec2 size = tc->scale * scene->getViewportSize();
                glm::vec2 pos = tc->position + scene->getViewportSize() / glm::vec2(2, 2);
                glm::vec4 color = glm::vec4(113, 96, 232, 255) / glm::vec4(255);
                Renderer2D::drawQuad(pos - glm::vec2(0, size.y / 2), glm::vec2(size.x, 1) * glm::vec2(1, editorCamera.scale.y), nullptr, color);
                Renderer2D::drawQuad(pos + glm::vec2(0, size.y / 2), glm::vec2(size.x, 1) * glm::vec2(1, editorCamera.scale.y), nullptr, color);
                Renderer2D::drawQuad(pos - glm::vec2(size.x / 2, 0), glm::vec2(1, size.y) * glm::vec2(editorCamera.scale.x, 1), nullptr, color);
                Renderer2D::drawQuad(pos + glm::vec2(size.x / 2, 0), glm::vec2(1, size.y) * glm::vec2(editorCamera.scale.x, 1), nullptr, color);
            }
        }
        // 选中实体边框线
        if(proj->selectedEntity && proj->selectedEntity->hasComponent<TransformComponent>() && proj->selectedEntity->hasComponent<SpriteComponent>()){
            auto* selectEntityTC = proj->selectedEntity->getComponent<TransformComponent>();
            Renderer2D::drawQuad(selectEntityTC->position - glm::vec2(0, selectEntityTC->scale.y / 2), glm::vec2(selectEntityTC->scale.x, 1) * glm::vec2(1, editorCamera.scale.y), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
            Renderer2D::drawQuad(selectEntityTC->position + glm::vec2(0, selectEntityTC->scale.y / 2), glm::vec2(selectEntityTC->scale.x, 1) * glm::vec2(1, editorCamera.scale.y), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
            Renderer2D::drawQuad(selectEntityTC->position - glm::vec2(selectEntityTC->scale.x / 2, 0), glm::vec2(1, selectEntityTC->scale.y) * glm::vec2(editorCamera.scale.x, 1), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
            Renderer2D::drawQuad(selectEntityTC->position + glm::vec2(selectEntityTC->scale.x / 2, 0), glm::vec2(1, selectEntityTC->scale.y) * glm::vec2(editorCamera.scale.x, 1), nullptr, {1.0f, 1.0f, 0.0f, 1.0f});
        }
        Renderer2D::endFrame();
    }

}  // namespace Cube