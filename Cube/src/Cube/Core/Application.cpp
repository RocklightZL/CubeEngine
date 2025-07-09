#include "pch.h"
#include "Application.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Cube/Event/ApplicationEvent.h"
#include "Window.h"
#include "Cube/Renderer/Buffer.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Renderer/Shader.h"
#include "Cube/Renderer/VertexArray.h"

namespace Cube {

    Application::Application() : mainWindow(nullptr), running(true) {
        init();
    }
    Application::~Application() = default;

    void Application::run()
    {
        running = true;
        CB_CORE_INFO("Application run");

        while(running) {
            for(Layer* layer : layers.getData()) {
                layer->onUpdate();
            }
            mainWindow->update();
        }
    }

    void Application::init() {
        Log::init();
        mainWindow = new Window(WindowPros(800, 600, "Cube Engine"), &dispatcher);
        Renderer::init();
        dispatcher.subscribe(std::bind(&Application::onWindowClose, this, std::placeholders::_1), EventType::WindowClose);
        dispatcher.subscribe(std::bind(&Application::onWindowResize, this, std::placeholders::_1), EventType::WindowResize);
    }

    LayerStack* Application::getLayers() {
        return &layers;
    }

    bool Application::onWindowClose(const Event& e) {
        delete mainWindow;
        running = false;
        CB_CORE_INFO("mainWindow close");
        return true;
    }

    bool Application::onWindowResize(const Event& e) {
        const auto ee = dynamic_cast<const WindowResizeEvent&>(e);
        Renderer::setViewport(ee.getWidth(), ee.getHeight());
        CB_CORE_INFO("mainWindow resize {} {}", mainWindow->getPros().width, mainWindow->getPros().height);
        return true;
    }

}  // namespace Cube