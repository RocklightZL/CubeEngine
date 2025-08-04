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

        const EditorCamera& editorCamera = proj->editorCamera;
        glm::mat4 pvMatrix = glm::ortho(0.0f, scene->getViewportSize().x, 0.0f, scene->getViewportSize().y, 0.0f, 1.0f) * glm::inverse(editorCamera.getTransformMatrix());
        Renderer2D::beginFrame(pvMatrix);
        // 坐标系标线
        Renderer2D::drawQuad({0, 0}, glm::vec2(1, 3000) * editorCamera.scale, nullptr, {1.0f, 0.0f, 0.0f, 1.0f});
        Renderer2D::drawQuad({0, 0}, glm::vec2(3000, 1) * editorCamera.scale, nullptr, {0.0f, 0.0f, 1.0f, 1.0f});
        for(auto e : sprites) {
            auto* sc = e->getComponent<SpriteComponent>();
            auto* tc = e->getComponent<TransformComponent>();
            Renderer2D::drawQuad(tc->getTransformMatrix(), sc->color, sc->texture, glm::vec4(sc->region.uvMin, sc->region.uvMax));
        }
        Renderer2D::endFrame();
    }

}  // namespace Cube