#include "ToolBar.h"

#include "../Game.h"
#include "../Project.h"
#include "../App/EditorApp.h"
#include "imgui/imgui.h"

#include <thread>

extern Cube::EditorApp* app;
extern Cube::Project* proj;

namespace Cube {

    void ToolBar::render(float deltaTime) {
        ImGui::Begin("ToolBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        if(ImGui::Button("Play")) {
            app->createGameWindow({(int)proj->selectedScene->scene->getViewportSize().x, (int)proj->selectedScene->scene->getViewportSize().y, "Test"});

            std::thread gameThread(gameThreadFunction);
            gameThread.detach();
        }
        ImGui::End();
    }
}  // namespace Cube