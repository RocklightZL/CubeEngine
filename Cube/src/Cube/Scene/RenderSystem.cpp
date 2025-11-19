#include "pch.h"
#include "RenderSystem.h"

#include "Cube/Renderer/Renderer.h"

namespace Cube {

    RenderSystem& RenderSystem::get() {
        static RenderSystem instance;
        return instance;
    }

    void RenderSystem::setCamera(Camera2D* camera) {
        this->camera = camera;
    }

    void RenderSystem::addDrawQuadCall(const DrawQuadCmd& cmd) {
        drawQuadCmds.push_back(cmd);
    }

    void RenderSystem::render() {
    }

}  // namespace Cube