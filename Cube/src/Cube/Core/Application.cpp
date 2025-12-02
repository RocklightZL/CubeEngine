#include "pch.h"
#include "Application.h"

#include "Window.h"
#include "TypeRegister.h"
#include "Cube/Event/ApplicationEvent.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Scene/Scene.h"

#include <chrono>
#include <iostream>

namespace Cube {

    bool Application::isInitialized = false;

    void Application::init() {
        Log::init();
        registerTypes();
    }

    Application::Application() : Application({1920, 1080, "Cube Engine"}){}

    Application::Application(const WindowPros& windowPros) : mainWindow(nullptr), running(true){
        if(!isInitialized) {
            init();
            isInitialized = true;
        }
        mainWindow = new Window(windowPros);

        EventDispatcher::get().subscribe<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
        EventDispatcher::get().subscribe<WindowResizeEvent>(std::bind(&Application::onWindowResize, this, std::placeholders::_1));
    }

    Application::~Application() {
        delete mainWindow;
    }

    void Application::run(){
        running = true;
        CB_CORE_INFO("Application run");

        std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

        while(running) {
            std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> frameDuration = currentTime - lastTime;
            lastTime = currentTime;
            float deltaTime = frameDuration.count();

            Renderer::clearBuffer();

            if(Scene* scene = sceneManager.getActive()) {
                scene->update(deltaTime);
                renderServer.renderScene(scene);
            }

            mainWindow->update();
        }
    }

    Window* Application::getWindow() {
        return mainWindow;
    }
    RenderServer& Application::getRenderServer() {
        return renderServer;
    }
    SceneManager& Application::getSceneManager() {
        return sceneManager;
    }

    bool Application::onWindowClose(const Event& e) {
        running = false;
        CB_CORE_INFO("mainWindow close");
        return true;
    }

    bool Application::onWindowResize(const Event& e) {
        const auto ee = static_cast<const WindowResizeEvent&>(e);
        Renderer::setViewport(ee.width, ee.width);
        return true;
    }

}  // namespace Cube