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

    void Renderer::clearBuffer() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void Renderer::beginFrame() {}

    void Renderer::endFrame() {}

    void Renderer::setViewport(int width, int height) { glViewport(0, 0, width, height); }

    void Renderer::setClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

    void Renderer::setVSync(bool val) { glfwSwapInterval(val); }

    //=========Renderer2D=================================

    std::shared_ptr<Shader> Renderer2D::shader = nullptr;
    std::shared_ptr<VertexArray> Renderer2D::vao = nullptr;
    std::shared_ptr<VertexBuffer> Renderer2D::vbo = nullptr;
    std::vector<QuadData> Renderer2D::batchData;
    unsigned int Renderer2D::batchCnt = 0;
    Texture2D* Renderer2D::currentTex = nullptr;
    bool Renderer2D::useTexture = false;
    Texture2D* Renderer2D::whiteTex = nullptr;

    int Renderer2D::drawCallCnt = 0;

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

        vbo = std::make_shared<VertexBuffer>();

        uint32_t* indices = new uint32_t[MAX_INDICES_PER_BATCH];
        uint32_t offset = 0;
        for(unsigned int i = 0; i < MAX_INDICES_PER_BATCH; i += 6) {
            indices[i] = 0 + offset;
            indices[i+1] = 1 + offset;
            indices[i+2] = 2 + offset;
            indices[i+3] = 0 + offset;
            indices[i+4] = 2 + offset;
            indices[i+5] = 3 + offset;
            offset += 4;
        }
        auto ibo = std::make_shared<IndexBuffer>(indices, MAX_INDICES_PER_BATCH * sizeof(uint32_t));
        delete[] indices;

        BufferLayout layout = {
            {ShaderDataType::Float4, "position"},
            {ShaderDataType::Float4, "color"},
            {ShaderDataType::Float2, "texCoord"}
        };
        vbo->setLayout(layout);
        vao = std::make_shared<VertexArray>();
        vao->setIndexBuffer(ibo);
        vao->addVertexBuffer(vbo);

        uint32_t data = 0xFFFFFFFF;
        whiteTex = new Texture2D(1, 1, &data);
    }

    void Renderer2D::beginFrame(const glm::mat4& pvMatrix) {
        startNewBatch();

        shader->bind();
        shader->setMat4("u_ViewProjectMatrix", pvMatrix);
    }

    void Renderer2D::endFrame() {
        flushBatch();
        CB_CORE_INFO("Drawcall count: {}", drawCallCnt);
        drawCallCnt = 0;
    }

    void Renderer2D::shutdown() {
        delete whiteTex;
    }

    void Renderer2D::drawQuad(const glm::mat4& modelMatrix, const glm::vec4& tintColor, Texture2D* texture, const glm::vec4& texCoord) {
        if(texture == nullptr) {
            texture = whiteTex;
        }
        if(texture != currentTex) {
            flushBatch();
            startNewBatch();
            currentTex = texture;
        }else if(batchCnt >= MAX_QUADS_PER_BATCH) {
            flushBatch();
            startNewBatch();
        }
        batchData.push_back({modelMatrix, tintColor, texCoord});
        batchCnt++;
    }

    void Renderer2D::drawQuad(const glm::vec2& pos, const glm::vec2& size, Texture2D* texture, const glm::vec4& tintColor, float degree, const glm::vec4& texCoord) {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(degree), {0.0f, 0.0f, 1.0f});
        modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));
        drawQuad(modelMatrix, tintColor, texture, texCoord);
    }

    void Renderer2D::drawQuad(const glm::vec2& pos, const glm::vec2& size, Texture2D* texture, const glm::vec4& texCoord, const glm::vec4& color, const glm::mat4 transform) {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));
        modelMatrix = transform * modelMatrix;
        drawQuad(modelMatrix, color, texture, texCoord);
    }

    void Renderer2D::setShader(const std::shared_ptr<Shader>& inShader) { shader = inShader; }

    void Renderer2D::startNewBatch() {
        batchData.clear();
        batchCnt = 0;
    }

    void Renderer2D::flushBatch() {
        std::vector<float> data;
        for(const QuadData& qd : batchData) {
            glm::vec4 pos[4] = {
                {-0.5f, -0.5f, 0.0f, 1.0f},
                {0.5f, -0.5f, 0.0f, 1.0f},
                {0.5f, 0.5f, 0.0f, 1.0f},
                {-0.5f, 0.5f, 0.0f, 1.0f}
            };
            glm::vec2 texCoords[4] = {
                {qd.textureCoord.x, qd.textureCoord.y},
                {qd.textureCoord.z, qd.textureCoord.y},
                {qd.textureCoord.z, qd.textureCoord.w},
                {qd.textureCoord.x, qd.textureCoord.w}
            };
            for(int i = 0; i < 4; ++i) {
                glm::vec4 p = qd.modelMatrix * pos[i];
                data.push_back(p.x);
                data.push_back(p.y);
                data.push_back(p.z);
                data.push_back(p.w);
                data.push_back(qd.color.r);
                data.push_back(qd.color.g);
                data.push_back(qd.color.b);
                data.push_back(qd.color.a);
                data.push_back(texCoords[i].x);
                data.push_back(texCoords[i].y);
            }
        }
        vbo->uploadData(data, GL_DYNAMIC_DRAW);
        if(currentTex != nullptr){
            currentTex->bind();
        }
        shader->bind();
        vao->bind();
        glDrawElements(GL_TRIANGLES, batchData.size() * 6, GL_UNSIGNED_INT, nullptr);
        drawCallCnt++;
    }

}  // namespace Cube
