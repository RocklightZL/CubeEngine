#include "EditorApp.h"

#include "EditorLayer.h"
#include "../Project.h"
#include "Cube/Core/Log.h"
#include "Cube/Event/ApplicationEvent.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Scene/Component.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

extern Cube::Project* proj;

namespace Cube {

    EditorApp::EditorApp(const WindowPros& windowPros) {
        Log::init();
        mainWindow = new Window(windowPros, &dispatcher);
        init();
    }

    EditorApp::~EditorApp() {

        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();

        delete gameWindow;
        delete mainWindow;
        delete currentLayer;
    }

    void EditorApp::run() {
        running = true;

        std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
        while(running) {
            std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> frameDuration = currentTime - lastTime;
            lastTime = currentTime;
            float deltaTime = frameDuration.count();

            mainWindow->makeContext();
            Renderer::clearBuffer();
            if(currentLayer) {
                currentLayer->onUpdate(deltaTime);
            }
            mainWindow->update();

            if(gameWindow) {
                gameWindow->makeContext();
                Renderer::clearBuffer();
                if(proj->selectedScene) {
                    proj->selectedScene->scene->onUpdate(deltaTime);
                }
                gameWindow->update();
            }
        }
    }

    void EditorApp::switchLayer(Layer* layer) {
        delete currentLayer;
        currentLayer = layer;
    }

    void EditorApp::createGameWindow(const WindowPros& pros) {
        delete gameWindow;
        gameWindow = new Window(pros, &dispatcher);
    }

    Window* EditorApp::getWindow() const {
        return mainWindow;
    }

    void EditorApp::init() {
        mainWindow->makeContext();

        dispatcher.subscribe(std::bind(&EditorApp::onWindowClose, this, std::placeholders::_1), EventType::WindowClose);
        dispatcher.subscribe(std::bind(&EditorApp::onWindowResize, this, std::placeholders::_1), EventType::WindowResize);

        // register built-in component
        Component::registerComponentType("TransformComponent", new ComponentFactoryImpl<TransformComponent>());
        Component::registerComponentType("SpriteComponent", new ComponentFactoryImpl<SpriteComponent>());
        Component::registerComponentType("CameraComponent", new ComponentFactoryImpl<CameraComponent>());
        Component::registerComponentType("AnimatorComponent", new ComponentFactoryImpl<AnimatorComponent>());

        imGuiInit();
    }

    void EditorApp::imGuiInit() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        io.Fonts->AddFontFromFileTTF("assets/fonts/SourceHanSansSC/SourceHanSansSC-Normal.otf", 30.0f);

        ImGui_ImplGlfw_InitForOpenGL(mainWindow->getNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    bool EditorApp::onWindowClose(const Event& e) {
        const auto ee = static_cast<const WindowCloseEvent&>(e);
        if(ee.getWindow() == mainWindow){
            running = false;
        }else if(ee.getWindow() == gameWindow) {
            delete gameWindow;
            gameWindow = nullptr;
        }
        return true;
    }

    bool EditorApp::onWindowResize(const Event& e) {
        // const auto ee = static_cast<const WindowResizeEvent&>(e);
        // if(ee.getWindow() == gameWindow) {
        // }
        return true;
    }
}  // namespace Cube