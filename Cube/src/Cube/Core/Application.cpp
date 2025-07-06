#include "Application.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Cube/Event/ApplicationEvent.h"
#include "pch.h"
#include "Window.h"
#include "glad/glad.h"

namespace Cube {

    Application::Application() : mainWindow(nullptr), running(true) {}
    Application::~Application() = default;

    void Application::run() {
        running = true;
        init();
        CB_CORE_INFO("Application run");
        while(running) {
            glClearColor(1.0f, 0.75f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            mainWindow->update();
        }
    }

    void Application::init() {
        Log::init();
        mainWindow = new Window(WindowPros(800, 600, "Cube Engine"), &dispatcher);

        dispatcher.subscribe(std::bind(&Application::onWindowClose, this, std::placeholders::_1), EventType::WindowClose);
        dispatcher.subscribe(std::bind(&Application::onWindowResize, this, std::placeholders::_1), EventType::WindowResize);
    }

    bool Application::onWindowClose(const Event& e) {
        delete mainWindow;
        running = false;
        CB_CORE_INFO("mainWindow close");
        return true;
    }

    bool Application::onWindowResize(const Event& e) {
        CB_CORE_INFO("mainWindow resize {} {}", mainWindow->getPros().width, mainWindow->getPros().height);
        return true;
    }

}  // namespace Cube