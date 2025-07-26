#include "GuidanceLayer.h"

#include "EditorApp.h"
#include "EditorLayer.h"
#include "../Project.h"
#include "Cube/Core/Log.h"
#include "Cube/UI/FileDialog.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

extern Cube::Project* proj;
extern Cube::EditorApp* app;

namespace Cube {

    void GuidanceLayer::onUpdate(float deltaTime) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->Flags);


        ImGui::Begin("Guidance");

        static bool openNewProjectPopup = false;
        if(ImGui::Button("New Project##1")) {
            openNewProjectPopup = true;
        }
        if(openNewProjectPopup) {
            ImGui::OpenPopup("New Project##2");
        }
        if(ImGui::BeginPopupModal("New Project##2")) {
            ImGui::Text("Project Name: ");
            ImGui::SameLine();
            static char name[50] = {};
            ImGui::InputText("##ProjectName", name, IM_ARRAYSIZE(name));

            ImGui::Text("Project Path");
            ImGui::SameLine();
            static char path[256] = {};
            ImGui::InputText("##ProjectPath", path, IM_ARRAYSIZE(path));
            ImGui::SameLine();
            if(ImGui::Button("...##1")) {
                std::string pathStr = FileDialog::selectDir(app->getWindow()->getWin32Window());
                strcpy_s(path, pathStr.c_str());
            }

            if(ImGui::Button("OK##2")) {
                delete proj;
                proj = new Project(name, path);
                memset(name, '\0', sizeof(name));
                memset(path, '\0', sizeof(path));
                openNewProjectPopup = false;
                ImGui::CloseCurrentPopup();

                app->switchLayer(new EditorLayer());
            }
            ImGui::SameLine();
            if(ImGui::Button("Cancel##2")) {
                memset(name, '\0', sizeof(name));
                memset(path, '\0', sizeof(path));
                openNewProjectPopup = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if(ImGui::Button("Open Project##1")) {
            delete proj;
            std::string path = FileDialog::openFile("Cube Project File(.cbproj)\0*.cbproj\0", app->getWindow()->getWin32Window());
            if(!path.empty()) {
                proj = new Project(path);
                app->switchLayer(new EditorLayer());
            }
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}  // namespace Cube