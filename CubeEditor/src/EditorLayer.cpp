#include "EditorLayer.h"

#include "Model.h"
#include "Cube/Core/Application.h"
#include "Cube/Renderer/RenderSystem.h"
#include "Cube/UI/FileDialog.h"
#include "Cube/Scene/SceneSerializer.h"
#include "Views/EntityPropertyPanel.h"
#include "Views/ScenePanel.h"
#include "Views/SceneView.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

extern Cube::Application* app;
extern Cube::Model* data;

namespace Cube {

    EditorLayer::~EditorLayer() {
        for(View* view : views) {
            delete view;
        }

        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
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
            if(ImGui::BeginMenu("Scene")) {
                if(ImGui::MenuItem("Add New Scene")) {
                }
                if(ImGui::MenuItem("Load Scene")) {
                    std::string filePath = FileDialog::openFile();
                    if(!filePath.empty()) {
                        SceneSerializer::deserialize(&data->scene, filePath);
                    }
                    data->scene.addSystem(new RenderSystem(800, 600)); // TODO: temporary line
                }
                if(ImGui::MenuItem("Save Scene")) {
                    std::string filePath = FileDialog::saveFile();
                    if(!filePath.empty()) {
                        SceneSerializer::serialize(&data->scene, filePath);
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        for(View* view : views) {
            view->render(deltaTime);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // initialize
    void EditorLayer::onAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        io.Fonts->AddFontFromFileTTF("assets/fonts/SourceHanSansSC/SourceHanSansSC-Normal.otf", 30.0f);

        ImGui_ImplGlfw_InitForOpenGL(app->getWindow()->getNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        views.push_back(new ScenePanel);
        views.push_back(new SceneView);
        views.push_back(new EntityPropertyPanel);

        data->scene.addSystem(new RenderSystem(800, 600));
    }

    void EditorLayer::onDetach() {}

}  // namespace Cube