#include "pch.h"
#include "AnimatorSystem.h"

#include "Cube/Scene/Scene.h"

namespace Cube {

    void AnimatorSystem::onUpdate(Scene* scene, float deltaTime) {
        auto target = scene->getEntitiesWith<SpriteComponent, AnimatorComponent>();
        for(auto& entity: target) {
            AnimatorComponent* animatorComponent = entity->getComponent<AnimatorComponent>();
            SpriteComponent* spriteComponent = entity->getComponent<SpriteComponent>();

            animatorComponent->animator.update(deltaTime);
            spriteComponent->region = animatorComponent->animator.getCurrentFrame();
        }
    }

}  // namespace Cube