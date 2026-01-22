#include "pch.h"
#include "RenderServer.h"

#include "Camera2D.h"
#include "Cube/Renderer/Renderer.h"
#include "Scene.h"
#include "SpriteRender.h"

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
        auto entities = scene->getEntitiesWith<SpriteRender>();
        std::sort(entities.begin(), entities.end(), [](const Entity* a, const Entity* b) {
            SpriteRender* spriteA = a->getComponent<SpriteRender>();
            SpriteRender* spriteB = b->getComponent<SpriteRender>();
            if(spriteA->order != spriteB->order) {
                return spriteA < spriteB;
            }
            return (spriteA->sprite->getTexture() ? spriteA->sprite->getTexture()->getId() : -1) < (spriteB->sprite->getTexture() ? spriteB->sprite->getTexture()->getId() : -1);
        });
        Renderer2D::beginFrame(camera->getPVMatrix());
        for(auto& entity : entities) {
            SpriteRender* sprite = entity->getComponent<SpriteRender>();
            Renderer2D::drawQuad(entity->getTransform().getWorldMatrix(), sprite->tintColor, sprite->sprite->getTexture(), sprite->sprite->getTexRegion().getUVCoord());
        }
        Renderer2D::endFrame();
    }
}  // namespace Cube