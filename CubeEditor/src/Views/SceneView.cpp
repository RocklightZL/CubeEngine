#include "SceneView.h"

#include "Cube/Renderer/Renderer.h"

#include "../Model.h"
#include "imgui/imgui.h"

extern Cube::Model* data;

namespace Cube {

    SceneView::SceneView() {
        data->frameBuffer.bindAttachment((int)sceneViewSize.x, (int)sceneViewSize.y);
    }

    void SceneView::render(float deltaTime) {
        ImGui::Begin("Scene View");

        ImVec2 currentSize = ImGui::GetContentRegionAvail();
        if(currentSize.x <= 0) currentSize.x = 1;
        if(currentSize.y <= 0) currentSize.y = 1;
        if((int)currentSize.x != (int)sceneViewSize.x || (int)currentSize.y != (int)sceneViewSize.y) {
            sceneViewSize = currentSize;
            data->frameBuffer.resize((int)sceneViewSize.x, (int)sceneViewSize.y);
        }

        data->frameBuffer.bind();
        Renderer2D::setViewport((int)sceneViewSize.x, (int)sceneViewSize.y);
        Renderer2D::clearBuffer();
        // scene render
        data->scene.onUpdate(deltaTime);

        FrameBuffer::bindDefaultFrameBuffer();

        ImGui::Image(data->frameBuffer.getTexture(), sceneViewSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }
    
}