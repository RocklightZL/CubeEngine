#include "pch.h"
#include "RenderSystem.h"

#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"
#include "Cube/Scene/Component.h"
#include "Renderer.h"

#include <glm/ext/matrix_clip_space.hpp>

namespace Cube {

    RenderSystem::RenderSystem(float viewportWidth, float viewportHeight) : viewportWidth(viewportWidth), viewportHeight(viewportHeight){}

    void RenderSystem::onUpdate(Scene* scene) {
        std::vector<Entity*> cameras(scene->getEntitiesWith<TransformComponent, CameraComponent>());
        Entity* mainCamera = nullptr;
        for(auto e : cameras) {
            if(e->getComponent<CameraComponent>()->available) {
                mainCamera = e;
                break;
            }
        }
        glm::mat4 pvMatrix = glm::ortho(0.0f, viewportWidth, 0.0f, viewportHeight, -0.0f, 1.0f) * glm::inverse(mainCamera->getComponent<TransformComponent>()->getTransformMatrix());

        std::vector<Entity*> target(scene->getEntitiesWith<TransformComponent, SpriteComponent>());
        std::sort(target.begin(), target.end(), [](Entity* a, Entity* b) { return a->getComponent<SpriteComponent>()->alas->getId() < b->getComponent<SpriteComponent>()->alas->getId(); });
        Renderer2D::beginFrame(pvMatrix);
        for(auto entity : target) {
            SpriteComponent* sprite = entity->getComponent<SpriteComponent>();
            TransformComponent* transform = entity->getComponent<TransformComponent>();
            Renderer2D::drawQuad(transform->getTransformMatrix(), sprite->color, sprite->alas, {sprite->alas->getSubTexture(sprite->name).uvMin, sprite->alas->getSubTexture(sprite->name).uvMax});
        }
        Renderer2D::endFrame();
    }

    void RenderSystem::onAttach() {
        Renderer2D::init();
        Renderer2D::setViewport((int)viewportWidth, (int)viewportHeight);
    }
}  // namespace Cube
