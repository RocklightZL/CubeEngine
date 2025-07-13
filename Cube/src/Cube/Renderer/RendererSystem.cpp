#include "pch.h"
#include "RendererSystem.h"

#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"
#include "Cube/Scene/Component.h"
#include "Renderer.h"

namespace Cube {

    void RendererSystem::onUpdate(Scene* scene) {
        std::vector<Entity*> target(std::move(scene->getEntitiesWith<TransformComponent, SpriteComponent>()));
        std::sort(target.begin(), target.end(), [](Entity* a, Entity* b) {
            return a->getComponent<SpriteComponent>()->alas->getId() < b->getComponent<SpriteComponent>()->alas->getId();
        });
        for(auto entity : target) {
            SpriteComponent* sprite = entity->getComponent<SpriteComponent>();
            TransformComponent* transform = entity->getComponent<TransformComponent>();
        }
    }
}  // namespace Cube