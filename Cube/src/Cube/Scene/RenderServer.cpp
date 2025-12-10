#include "pch.h"
#include "RenderServer.h"

#include "Camera2D.h"
#include "Scene.h"
#include "Sprite.h"
#include "Cube/Renderer/Renderer.h"

namespace Cube {

    void RenderServer::renderScene(const Scene* scene) {
        Camera2D* camera = nullptr;
        for(auto& entity : scene->getAllEntities()) {
            Camera2D* cam = entity->getComponent<Camera2D>();
            if(cam != nullptr && cam->available) {
                camera = cam;
                break;
            }
        }
        if(camera == nullptr) {
            CB_CORE_ERROR("RenderServer::renderScene(): no available Camera2D found in the scene");
            return;
        }
        auto entities = scene->getEntitiesWith<Sprite>();
        std::sort(entities.begin(), entities.end(), [](const Entity* a, const Entity* b) {
            Sprite* spriteA = a->getComponent<Sprite>();
            Sprite* spriteB = b->getComponent<Sprite>();
            if(spriteA->order != spriteB->order) {
                return spriteA < spriteB;
            }
            return (spriteA->texture.get() ? spriteA->texture->getId() : -1) < (spriteB->texture.get() ? spriteB->texture->getId() : -1);
        });
        Renderer2D::beginFrame(camera->getPVMatrix());
        for(auto& entity : entities) {
            Sprite* sprite = entity->getComponent<Sprite>();
            Renderer2D::drawQuad(entity->getTransform().getWorldMatrix(), sprite->tintColor, sprite->texture.get(), sprite->texRegion.getUVCoord());
        }
        Renderer2D::endFrame();
    }
}  // namespace Cube