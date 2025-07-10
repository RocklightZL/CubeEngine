#include "pch.h"
#include "Renderer.h"

#include "Buffer.h"
#include "BufferLayout.h"
#include "VertexArray.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

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

    void Renderer::setClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

    void Renderer::setVSync(bool val) { glfwSwapInterval(val); }

    //=========Renderer2D=================================

    std::shared_ptr<Shader> Renderer2D::shader = nullptr;
    std::shared_ptr<VertexArray> Renderer2D::vao = nullptr;

    void Renderer2D::init() {

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            CB_CORE_ERROR("Failed to initialize GLAD!");
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        // ∆Ù”√ªÏ∫œ
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader = std::make_shared<Shader>(DEFAULT_2D_VERTEX_SHADER_SRC, DEFAULT_2D_FRAGMENT_SHADER_SRC);
        // clang-format off
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };
        // clang-format on
        std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};
        auto vbo = std::make_shared<VertexBuffer>(vertices);
        auto ibo = std::make_shared<IndexBuffer>(indices);
        BufferLayout layout = {
            {ShaderDataType::Float3, "position"},
            {ShaderDataType::Float2, "texCoord"}
        };
        vbo->setLayout(layout);
        vao = std::make_shared<VertexArray>();
        vao->setIndexBuffer(ibo);
        vao->addVertexBuffer(vbo);
    }

    void Renderer2D::beginFrame(const Camera2D& camera) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->bind();
        shader->setMat4("u_ViewProjectMatrix", camera.getPVMatrix());
    }

    void Renderer2D::endFrame() {
    }

    void Renderer2D::shutdown() {}

    void Renderer2D::drawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const glm::mat4& transform) {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));

        shader->setMat4("u_ModelMatrix", modelMatrix);
        shader->setVec4("u_Color", color);
        shader->setInt("u_UseTexture", 0);

        vao->bind();
        glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer2D::drawQuad(const glm::vec2& pos, const glm::vec2& size, const Texture2D& texture, const glm::vec4& tintColor, const glm::mat4& transform) {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));
        modelMatrix = transform * modelMatrix;

        shader->setMat4("u_ModelMatrix", modelMatrix);
        shader->setVec4("u_Color", tintColor);
        shader->setInt("u_UseTexture", 1);

        texture.bind();
        vao->bind();
        glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer2D::setShader(const std::shared_ptr<Shader>& inShader) {
        shader = inShader;
    }

}  // namespace Cube
