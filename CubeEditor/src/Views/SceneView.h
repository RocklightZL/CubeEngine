#pragma once

#include "View.h"
#include "../Scene/EditorRenderSystem.h"
#include "Cube/Renderer/FrameBuffer.h"
#include "imgui/imgui.h"

namespace Cube {

    class SceneView : public View {
    public:
        SceneView();
        ~SceneView() override;

        void render(float deltaTime) override;
    private:
        ImVec2 sceneViewSize = {800, 600};
        FrameBuffer* frameBuffer = nullptr;
        EditorRenderSystem* editorRenderSystem = nullptr;
    };

}