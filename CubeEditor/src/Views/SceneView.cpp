#include "SceneView.h"

#include "../Project.h"
#include "Cube/Renderer/Renderer.h"

#include "imgui/imgui.h"

extern Cube::Project* proj;

namespace Cube {

    SceneView::SceneView() {
        frameBuffer = new FrameBuffer;
        frameBuffer->bindAttachment((int)sceneViewSize.x, (int)sceneViewSize.y);
    }

    SceneView::~SceneView() {
        delete frameBuffer;
    }

    void SceneView::render(float deltaTime) {
        ImGui::Begin("Scene View");
        if(proj->selectedScene){
            ImVec2 currentSize = ImGui::GetContentRegionAvail();
            if(currentSize.x <= 0) currentSize.x = 1;
            if(currentSize.y <= 0) currentSize.y = 1;
            if((int)currentSize.x != (int)sceneViewSize.x || (int)currentSize.y != (int)sceneViewSize.y) {
                sceneViewSize = currentSize;
                frameBuffer->resize((int)sceneViewSize.x, (int)sceneViewSize.y);
            }
            proj->selectedScene->scene->setViewportSize({sceneViewSize.x, sceneViewSize.y});

            frameBuffer->bind();
            Renderer2D::setViewport((int)sceneViewSize.x, (int)sceneViewSize.y);
            Renderer2D::clearBuffer();
            // scene render
            proj->selectedScene->scene->onUpdate(deltaTime);

            FrameBuffer::bindDefaultFrameBuffer();

            ImGui::Image(frameBuffer->getTexture(), sceneViewSize, ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();
    }
    
}