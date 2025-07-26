#include "EditorApp.h"

#include "EditorLayer.h"
#include "Cube/Event/ApplicationEvent.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Scene/Component.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

namespace Cube {

    EditorApp::EditorApp(const WindowPros& windowPros) : mainWindow(nullptr), running(false), currentLayer(nullptr){
        Log::init();
        mainWindow = new Window(windowPros, &dispatcher);
        init();
    }

    EditorApp::~EditorApp() {
        Renderer2D::shutdown();

        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();

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

            Renderer::clearBuffer();

            if(currentLayer) {
                currentLayer->onUpdate(deltaTime);
            }
            mainWindow->update();
        }
    }

    void EditorApp::switchLayer(Layer* layer) {
        delete currentLayer;
        currentLayer = layer;
    }

    Window* EditorApp::getWindow() const {
        return mainWindow;
    }

    void EditorApp::init() {
        Renderer2D::init();
        Renderer2D::setViewport(800, 600);

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
        running = false;
        return true;
    }

    bool EditorApp::onWindowResize(const Event& e) {
        // const auto ee = dynamic_cast<const WindowResizeEvent&>(e);
        // Renderer2D::setViewport(ee.getWidth(), ee.getHeight());
        return true;
    }
}  // namespace Cube