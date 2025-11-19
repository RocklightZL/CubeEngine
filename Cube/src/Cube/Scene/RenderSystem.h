#pragma once
#include "Camera2D.h"
#include "Cube/Renderer/Color.h"
#include "Cube/Renderer/TextureRegion.h"

namespace Cube {
    class Texture2D;

    struct DrawQuadCmd {
        glm::mat4 modalMatrix;
        Color tintColor;
        Texture2D* texture;
        TextureRegion textureRegion;
    };

    // render the scene
    class RenderSystem {
    public:
        RenderSystem& get();
        void setCamera(Camera2D* camera);
        void addDrawQuadCall(const DrawQuadCmd& cmd);
        void render();

    private:
        RenderSystem() = default;
        ~RenderSystem() = default;

        Camera2D* camera;
        std::vector<DrawQuadCmd> drawQuadCmds;
    };

}