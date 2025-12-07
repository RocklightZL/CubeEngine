#pragma once

#include "View.h"
#include "../Scene/TextureData.h"
#include "Cube/Renderer/FrameBuffer.h"
#include "Cube/Renderer/Texture.h"
#include "imgui/imgui.h"

class SceneView : public View {
public:
    SceneView();
    ~SceneView() override;

    void render(float deltaTime) override;

    static Cube::SubTexture* selectSubTexturePopup(TextureData& data, bool* open);
private:
    ImVec2 sceneViewSize = {800, 600};
    Cube::FrameBuffer* frameBuffer = nullptr;

    void sceneRender(float deltaTime);
};