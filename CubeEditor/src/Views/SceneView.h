#pragma once

#include "View.h"
#include "../Scene/TextureData.h"
#include "Cube/Renderer/FrameBuffer.h"
#include "Cube/Renderer/Texture.h"
#include "imgui/imgui.h"

#include <memory>

class SceneView : public View {
public:
    SceneView();
    ~SceneView() override;

    void render(float deltaTime) override;

    static Cube::SubTexture* selectSubTexturePopup(TextureData& data, bool* open);
private:
    ImVec2 sceneViewSize = {800, 600};
    Cube::FrameBuffer* frameBuffer = nullptr;

    std::unique_ptr<Cube::Texture2D> play_png = std::make_unique<Cube::Texture2D>("assets/icons/play.png");

    void sceneRender(float deltaTime);
};