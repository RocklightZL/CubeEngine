#include "pch.h"
#include "Application.h"

#include "Cube/Event/ApplicationEvent.h"
#include "Window.h"
#include "Cube/Renderer/Buffer.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Scene/Component.h"

#include <chrono>
#include <iostream>

namespace Cube {

    Application::Application() : Application({1920, 1080, "Cube Engine"}){}

    Application::Application(const WindowPros& windowPros) : mainWindow(nullptr), running(true), mainScene(nullptr) {
        Log::init();
        mainWindow = new Window(windowPros, &dispatcher);
        init();
    }

    Application::~Application() {
        delete mainWindow;
        delete mainScene;
    }

    void Application::run()
    {
        running = true;
        CB_CORE_INFO("Application run");

        std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
        while(running) {
            // º∆À„÷° ±º‰
            std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> frameDuration = currentTime - lastTime;
            lastTime = currentTime;
            float deltaTime = frameDuration.count();

            // CB_CORE_INFO("FPS: {}", 1/deltaTime);
            // std::cout << "FPS: " << 1 / deltaTime << "\n";

            Renderer::clearBuffer();

            for(Layer* layer : layers.getData()) {
                layer->onUpdate(deltaTime);
            }
            if(mainScene) {
                mainScene->onUpdate(deltaTime);
            }
            mainWindow->update();
        }
    }

    void Application::init() {
        Renderer::init();
        dispatcher.subscribe(std::bind(&Application::onWindowClose, this, std::placeholders::_1), EventType::WindowClose);
        dispatcher.subscribe(std::bind(&Application::onWindowResize, this, std::placeholders::_1), EventType::WindowResize);

        // register built-in component
        Component::registerComponentType("TransformComponent", new ComponentFactoryImpl<TransformComponent>());
        Component::registerComponentType("SpriteComponent", new ComponentFactoryImpl<SpriteComponent>());
        Component::registerComponentType("CameraComponent", new ComponentFactoryImpl<CameraComponent>());
        Component::registerComponentType("AnimatorComponent", new ComponentFactoryImpl<AnimatorComponent>());
    }

    LayerStack* Application::getLayers() { return &layers; }

    Window* Application::getWindow() { return mainWindow; }

    void Application::pushLayer(Layer* layer) {
        layers.pushLayer(layer);
    }

    void Application::setMainScene(Scene* scene) {
        mainScene = scene;
    }

    bool Application::onWindowClose(const Event& e) {
        running = false;
        CB_CORE_INFO("mainWindow close");
        return true;
    }

    bool Application::onWindowResize(const Event& e) {
        const auto ee = dynamic_cast<const WindowResizeEvent&>(e);
        Renderer::setViewport(ee.getWidth(), ee.getHeight());
        return true;
    }

}  // namespace Cube