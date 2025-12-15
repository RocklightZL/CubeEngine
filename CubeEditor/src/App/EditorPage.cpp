#include "EditorPage.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/ext/matrix_clip_space.hpp>

#include "../Project.h"
#include "../Views/EntityPropertyPanel.h"
#include "../Views/ResourcesPanel.h"
#include "../Views/ScenePanel.h"
#include "../Views/SceneSelectPanel.h"
#include "../Views/SceneView.h"
#include "Cube/Core/Application.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/UI/FileDialog.h"
#include "Cube/Utils/Utils.h"
#include "EditorApp.h"
#include "imgui/imgui_internal.h"

using namespace Cube;

extern EditorApp* app;
extern Project* proj;

EditorPage::EditorPage() {
    views.push_back(std::make_unique<ScenePanel>());
    views.push_back(std::make_unique<SceneView>());
    views.push_back(std::make_unique<EntityPropertyPanel>());
    views.push_back(std::make_unique<SceneSelectPanel>());
    views.push_back(std::make_unique<ResourcesPanel>());

    glfwSetDropCallback(app->getWindow()->getNativeWindow(), [](GLFWwindow* window, int path_count, const char* paths[]) {
        for(int i = 0; i < path_count; ++i) {
            ResourcesPanel::importResource(paths[i]);
        }
    });
}

void EditorPage::render(float deltaTime) {
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
                std::string filePath = FileDialog::openFile("Scene File(.scene)\0*.scene\0" ,app->getWindow()->getWin32Window());
                if(!filePath.empty()) {
                    Scene* scene = new Scene(filePath);
                    if(Utils::getFileName(filePath) == scene->getName()){
                        if(!proj->hasScene(scene->getName())){
                            proj->addScene(scene);
                            if(!Utils::isFileInDirectory(filePath, proj->getConfig().sceneDirectory)) {
                                Utils::copyFile(filePath, proj->getConfig().sceneDirectory + "/" + scene->getName() + ".scene");
                            }
                        }else {
                            delete scene;
                            CB_WARN("The scene has existed"); // TODO: 提醒用户
                        }
                    }else {
                        delete scene;
                        CB_ERROR("The scene file name does not match the scene name"); // TODO: 提醒用户
                    }
                }
            }
            if(ImGui::MenuItem("Save Scene") && proj->selectedScene) {
                if(!proj->selectedScene->isSaved){
                    proj->selectedScene->scene->serialize(proj->getConfig().sceneDirectory + "/" + proj->selectedScene->scene->getName() + ".scene");
                    proj->selectedScene->isSaved = true;
                }
            }
            if(ImGui::MenuItem("Save All Scene")) {
                for(auto& scene : proj->scenes){
                    if(!scene.isSaved){
                        scene.scene->serialize(proj->getConfig().sceneDirectory + "/" + scene.scene->getName() + ".scene");
                        scene.isSaved = true;
                    }
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

            static bool showTip = false;
            if(showTip) ImGui::Text("This scene has existed!");

            if(ImGui::Button("Add##3")) {
                if(!proj->hasScene(name)){
                    proj->addScene(new Scene(name, true));
                    memset(name, '\0', sizeof(name));
                    showAddNewScene = false;
                    showTip = false;
                    ImGui::CloseCurrentPopup();
                }else {
                    showTip = true;
                }
            }
            ImGui::SameLine();
            if(ImGui::Button("Cancel##3")) {
                memset(name, '\0', sizeof(name));
                showAddNewScene = false;
                showTip = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if(ImGui::BeginMenu("Resources")) {
            if(ImGui::MenuItem("Import Resources##1")) {
                ResourcesPanel::importFromFileDialog();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    for(auto& view : views) {
        view->render(deltaTime);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}