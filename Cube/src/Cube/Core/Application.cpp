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
#include "glad/glad.h"

namespace Cube {

    Application::Application() : mainWindow(nullptr), running(true) {}
    Application::~Application() = default;

    void Application::run()
    {
        running = true;
        init();
        CB_CORE_INFO("Application run");

        // clang-format off
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        std::string vertexShaderSrc = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            void main(){
                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            }
        )";
        std::string fragmentShaderSrc = R"(
            #version 330 core
            out vec4 FragColor;
            void main(){
                FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
            }
        )";
        // clang-format on
        Shader shader(vertexShaderSrc, fragmentShaderSrc);
        // Shader shader(readFileToString("D:/mycode/vsProject/CubeEngine/bin/Debug-windows-x86_64/Sandbox/VertexShaderTest.vs"), readFileToString("D:/mycode/vsProject/CubeEngine/bin/Debug-windows-x86_64/Sandbox/FragmentShaderTest.fs"));
        // Shader shader(readFileToString("VertexShaderTest.vs"), readFileToString("FragmentShaderTest.fs"));


        std::vector<uint32_t> indices = {0, 1, 2};

        auto vbo = std::make_shared<VertexBuffer>(vertices);
        auto ibo = std::make_shared<IndexBuffer>(indices);

        BufferLayout layout = {
            {ShaderDataType::Float3, "position"}
        };
        vbo->setLayout(layout);
        VertexArray vao;
        vao.addVertexBuffer(vbo);
        vao.setIndexBuffer(ibo);

        while(running) {
            Renderer::setClearColor(1.0f, 0.0f, 1.0f, 1.0f);
            Renderer::beginFrame();

            shader.bind();
            glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, nullptr);

            Renderer::endFrame();
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