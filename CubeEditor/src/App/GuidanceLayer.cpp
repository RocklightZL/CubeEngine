#include "GuidanceLayer.h"

#include "EditorApp.h"
#include "EditorLayer.h"
#include "../Project.h"
#include "Cube/Core/Log.h"
#include "Cube/UI/FileDialog.h"
#include "../Utils/misc.h"
#include "imgui/imgui_internal.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

extern Cube::Project* proj;
extern Cube::EditorApp* app;

namespace Cube {

    GuidanceLayer::GuidanceLayer() {
        openProjectIcon = ResourceManager::getInstance().load<Texture2D>("assets/icons/open_project.png")->data;
        newProjectIcon = ResourceManager::getInstance().load<Texture2D>("assets/icons/new_project.png")->data;
    }

    GuidanceLayer::~GuidanceLayer() {
        ResourceManager::getInstance().release(openProjectIcon->getFilePath());
        ResourceManager::getInstance().release(newProjectIcon->getFilePath());
    }

    void GuidanceLayer::onUpdate(float deltaTime) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->Flags);


        ImGui::Begin("Guidance");
        constexpr ImVec2 buttonSize = {128, 128};
        ImGui::SetCursorPos({ImGui::GetWindowWidth() / 2 - (buttonSize.x * 2 + 100 + ImGui::GetStyle().FramePadding.x * 2 * 2) / 2, 300.0f});
        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(CB_COLOR(70, 77, 88), 0.5f));

        static bool openNewProjectPopup = false;
        ImGui::BeginGroup();
        if(ImGui::ImageButton("New Project##1", newProjectIcon->getId(), buttonSize, {0, 1}, {1, 0})) {
            openNewProjectPopup = true;
        }
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (buttonSize.x + ImGui::GetStyle().FramePadding.x * 2) / 2 - ImGui::CalcTextSize("New Project").x / 2);
        ImGui::Text("New Project");
        ImGui::EndGroup();

        ImGui::SameLine(0.0f, 100.0f);

        ImGui::BeginGroup();
        if(ImGui::ImageButton("Open Project##1", openProjectIcon->getId(), buttonSize, {0, 1}, {1, 0})) {
            delete proj;
            std::string path = FileDialog::openFile("Cube Project File(.cbproj)\0*.cbproj\0", app->getWindow()->getWin32Window());
            if(!path.empty()) {
                proj = new Project(path);
                app->switchLayer(new EditorLayer());
            }
        }
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (buttonSize.x + ImGui::GetStyle().FramePadding.x * 2) / 2 - ImGui::CalcTextSize("Open Project").x / 2);
        ImGui::Text("Open Project");
        ImGui::EndGroup();

        ImGui::PopStyleColor();
        ImGui::EndGroup();

        static bool isNameValid = true;
        static bool isPathValid = true;
        static char name[50] = {};
        static char path[256] = {};
        if(openNewProjectPopup) {
            ImGui::OpenPopup("New Project##2");
        }else {
            isNameValid = true;
            isPathValid = true;
            memset(name, '\0', sizeof(name));
            memset(path, '\0', sizeof(path));
        }
        if(ImGui::BeginPopupModal("New Project##2", &openNewProjectPopup, ImGuiWindowFlags_AlwaysAutoResize)) {

            ImGui::Text("Project Name:");
            if(!isNameValid) {
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Text("This name is not valid!");
                ImGui::PopStyleColor();
            }
            ImGui::InputText("##ProjectName", name, IM_ARRAYSIZE(name));

            ImGui::Text("Project Path:");
            if(!isPathValid) {
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Text("This path is not valid!");
                ImGui::PopStyleColor();
            }
            ImGui::InputText("##ProjectPath", path, IM_ARRAYSIZE(path));
            ImGui::SameLine();
            if(ImGui::Button("...##1")) {
                std::string pathStr = FileDialog::selectDir(app->getWindow()->getWin32Window());
                strcpy_s(path, pathStr.append("/").append(name).c_str());
            }

            if(ImGui::Button("OK##2")) {
                isNameValid = !std::string(name).empty();
                isPathValid = std::filesystem::exists(path);
                if(isNameValid && isPathValid){
                    delete proj;
                    proj = new Project(name, path);
                    openNewProjectPopup = false;
                    ImGui::CloseCurrentPopup();

                    app->switchLayer(new EditorLayer());
                }
            }
            ImGui::SameLine();
            if(ImGui::Button("Cancel##2")) {
                openNewProjectPopup = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}  // namespace Cube