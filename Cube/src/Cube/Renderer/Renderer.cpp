#include "pch.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Cube {

    void Renderer::init() {
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            CB_CORE_ERROR("Failed to initialize GLAD!");
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
    void Renderer::beginFrame() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void Renderer::endFrame() {
        
    }

    void Renderer::setViewport(int width, int height) { glViewport(0, 0, width, height); }

    void Renderer::setClearColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
    }

}  // namespace Cube