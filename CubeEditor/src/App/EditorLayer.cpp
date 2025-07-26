#include "EditorLayer.h"

#include "EditorApp.h"
#include "../Project.h"
#include "Cube/Core/Application.h"
#include "Cube/Renderer/RenderSystem.h"
#include "Cube/UI/FileDialog.h"
#include "Cube/Scene/SceneSerializer.h"
#include "../Views/EntityPropertyPanel.h"
#include "../Views/ResourcesPanel.h"
#include "../Views/ScenePanel.h"
#include "../Views/SceneSelectPanel.h"
#include "../Views/SceneView.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

extern Cube::EditorApp* app;
extern Cube::Project* proj;

namespace Cube {

    EditorLayer::EditorLayer() {

        views.push_back(new ScenePanel);
        views.push_back(new SceneView);
        views.push_back(new EntityPropertyPanel);
        views.push_back(new SceneSelectPanel);
        views.push_back(new ResourcesPanel);
    }

    EditorLayer::~EditorLayer() {
        for(View* view : views) {
            delete view;
        }
    }

    void EditorLayer::onUpdate(float deltaTime) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->Flags);
        // content
        ImGui::ShowDemoWindow();

        // MenuBar
        if(ImGui::BeginMainMenuBar()) {
            if(ImGui::BeginMenu("Project")) {
                if(ImGui::MenuItem("New Project")) {
                }
                if(ImGui::MenuItem("Open Project")) {
                }
                ImGui::EndMenu();
            }
            static bool showAddNewScene = false;
            if(ImGui::BeginMenu("Scene")) {
                if(ImGui::MenuItem("Add New Scene")) {
                    showAddNewScene = true;
                }

                if(ImGui::MenuItem("Load Scene")) {
                    std::string filePath = FileDialog::openFile("Scene File\0*.scene\0" ,app->getWindow()->getWin32Window());
                    if(!filePath.empty()) {
                        Scene* scene = new Scene();
                        proj->addScene(scene);
                        SceneSerializer::deserialize(scene, filePath);
                        scene->addSystem(new RenderSystem()); // TODO: temporary line
                    }
                }
                if(ImGui::MenuItem("Save Scene") && proj->selectedScene) {
                    std::string filePath = FileDialog::saveFile("Scene File\0*.scene\0" ,app->getWindow()->getWin32Window());
                    if(!filePath.empty()) {
                        SceneSerializer::serialize(proj->selectedScene, filePath);
                    }
                }
                ImGui::EndMenu();
            }
            if(showAddNewScene) ImGui::OpenPopup("Add New Scene##1");
            if(ImGui::BeginPopupModal("Add New Scene##1")) {
                static char name[50] = {};
                ImGui::Text("Name: ");
                ImGui::SameLine();
                ImGui::InputText("##NameInputText", name, IM_ARRAYSIZE(name));

                if(ImGui::Button("Add##3")) {
                    proj->addScene(new Scene({800, 600}, name));
                    memset(name, '\0', sizeof(name));
                    showAddNewScene = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if(ImGui::Button("Cancel##3")) {
                    memset(name, '\0', sizeof(name));
                    showAddNewScene = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            ImGui::EndMainMenuBar();
        }

        for(View* view : views) {
            view->render(deltaTime);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void EditorLayer::onAttach() {}

    void EditorLayer::onDetach() {}

}  // namespace Cube