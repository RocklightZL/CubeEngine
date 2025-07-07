#include "pch.h"
#include "Application.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Cube/Event/ApplicationEvent.h"
#include "Window.h"
#include "Cube/Renderer/Renderer.h"
#include "glad/glad.h"

namespace Cube {

    Application::Application() : mainWindow(nullptr), running(true) {}
    Application::~Application() = default;

    void Application::run() {
        running = true;
        init();
        CB_CORE_INFO("Application run");

        unsigned int VBO;
        float vertices[3 * 3]{-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        unsigned int EBO;
        unsigned int indices[]{0, 1, 2};
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        while(running) {
            glClearColor(1.0f, 0.75f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

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
        const auto ee = dynamic_cast<const WindowResizeEvent&>(e);
        Renderer::setViewport(ee.getWidth(), ee.getHeight());
        CB_CORE_INFO("mainWindow resize {} {}", mainWindow->getPros().width, mainWindow->getPros().height);
        return true;
    }

}  // namespace Cube