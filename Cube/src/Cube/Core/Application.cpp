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

    bool Application::isInitialized = false;

    void Application::init() {
        Log::init();
        // register built-in component //TODO: 可以放到场景类那里
        Component::registerComponentType("TransformComponent", new ComponentFactoryImpl<TransformComponent>());
        Component::registerComponentType("SpriteComponent", new ComponentFactoryImpl<SpriteComponent>());
        Component::registerComponentType("CameraComponent", new ComponentFactoryImpl<CameraComponent>());
        Component::registerComponentType("AnimatorComponent", new ComponentFactoryImpl<AnimatorComponent>());
    }

    Application::Application() : Application({1920, 1080, "Cube Engine"}){}

    Application::Application(const WindowPros& windowPros) : mainWindow(nullptr), running(true){
        if(!isInitialized) {
            init();
            isInitialized = true;
        }
        mainWindow = new Window(windowPros, &dispatcher);

        dispatcher.subscribe(std::bind(&Application::onWindowClose, this, std::placeholders::_1), EventType::WindowClose);
        dispatcher.subscribe(std::bind(&Application::onWindowResize, this, std::placeholders::_1), EventType::WindowResize);
    }

    Application::~Application() {
        delete mainWindow;
    }

    void Application::run(){
        running = true;
        CB_CORE_INFO("Application run");

        std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

        while(running) {
            // 计算帧时间
            std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> frameDuration = currentTime - lastTime;
            lastTime = currentTime;
            float deltaTime = frameDuration.count();

            Renderer::clearBuffer();

            for(const auto& layer : layers.getLayers()) {
                layer->onUpdate(deltaTime);
            }
            mainWindow->update();
        }
    }

    Window* Application::getWindow() { return mainWindow; }

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