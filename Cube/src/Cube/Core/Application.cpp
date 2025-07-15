#include "pch.h"
#include "Application.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Cube/Event/ApplicationEvent.h"
#include "Window.h"
#include "Cube/Renderer/Buffer.h"
#include "Cube/Renderer/Renderer.h"

namespace Cube {

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

        while(running) {
            for(Layer* layer : layers.getData()) {
                layer->onUpdate();
            }
            if(mainScene) {
                mainScene->onUpdate();
            }
            mainWindow->update();
        }
    }

    void Application::init() {
        Renderer::init();
        dispatcher.subscribe(std::bind(&Application::onWindowClose, this, std::placeholders::_1), EventType::WindowClose);
        dispatcher.subscribe(std::bind(&Application::onWindowResize, this, std::placeholders::_1), EventType::WindowResize);
    }

    LayerStack* Application::getLayers() { return &layers; }

    Window* Application::getWindow() { return mainWindow; }

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