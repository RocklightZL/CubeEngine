#include "pch.h"
#include "RenderSystem.h"

#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"
#include "Cube/Scene/Component.h"
#include "Renderer.h"

#include <glm/ext/matrix_clip_space.hpp>

namespace Cube {

    void RenderSystem::onUpdate(Scene* scene, float deltaTime) {
        std::vector<Entity*> cameras(scene->getEntitiesWith<TransformComponent, CameraComponent>());
        Entity* mainCamera = nullptr;
        for(auto e : cameras) {
            if(e->getComponent<CameraComponent>()->available) {
                mainCamera = e;
                break;
            }
        }

        if(!mainCamera) {
            return;
        }

        glm::mat4 pvMatrix = glm::ortho(0.0f, scene->getViewportSize().x, 0.0f, scene->getViewportSize().y, 0.0f, 1.0f) * glm::inverse(mainCamera->getComponent<TransformComponent>()->getTransformMatrix());

        std::vector<Entity*> target(scene->getEntitiesWith<TransformComponent, SpriteComponent>());
        std::sort(target.begin(), target.end(), [](Entity* a, Entity* b) {
            auto* aa = a->getComponent<SpriteComponent>()->atlas;
            auto* ba = b->getComponent<SpriteComponent>()->atlas;
            if(!aa) {
                return ba != nullptr;
            }
            if(!ba) {
                return false;
            }
            return aa->getId() < ba->getId();
        });
        Renderer2D::beginFrame(pvMatrix);
        for(auto entity : target) {
            SpriteComponent* sprite = entity->getComponent<SpriteComponent>();
            TransformComponent* transform = entity->getComponent<TransformComponent>();
            Renderer2D::drawQuad(transform->getTransformMatrix(), sprite->color, sprite->atlas, {sprite->region.uvMin, sprite->region.uvMax});
        }
        Renderer2D::endFrame();
    }

    void RenderSystem::onAttach() {}

    std::string RenderSystem::getName() const { return "RenderSystem"; }
}  // namespace Cube
