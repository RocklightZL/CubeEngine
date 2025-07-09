#include "glad/glad.h"
#include <Cube.h>

class TestLayer : public Cube::Layer{
public:
    TestLayer();
    void onUpdate() override;
private:
    std::shared_ptr<Cube::Shader> shader;
    std::shared_ptr<Cube::VertexArray> vao;
};

TestLayer::TestLayer() {
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
    shader = std::make_shared<Cube::Shader>(vertexShaderSrc, fragmentShaderSrc);
    // Shader shader(readFileToString("D:/mycode/vsProject/CubeEngine/bin/Debug-windows-x86_64/Sandbox/VertexShaderTest.vs"), readFileToString("D:/mycode/vsProject/CubeEngine/bin/Debug-windows-x86_64/Sandbox/FragmentShaderTest.fs"));
    // Shader shader(readFileToString("VertexShaderTest.vs"), readFileToString("FragmentShaderTest.fs"));


    std::vector<uint32_t> indices = {0, 1, 2};

    auto vbo = std::make_shared<Cube::VertexBuffer>(vertices);
    auto ibo = std::make_shared<Cube::IndexBuffer>(indices);

    Cube::BufferLayout layout = {
        {Cube::ShaderDataType::Float3, "position"}
    };
    vbo->setLayout(layout);
    vao = std::make_shared<Cube::VertexArray>();
    vao->addVertexBuffer(vbo);
    vao->setIndexBuffer(ibo);
}


void TestLayer::onUpdate() {
    Cube::Renderer::setClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    Cube::Renderer::beginFrame();

    shader->bind();
    glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

    Cube::Renderer::endFrame();

}

int main(){
    Cube::Application app;
    TestLayer* testLayer = new TestLayer();
    app.getLayers()->pushLayer(testLayer);
    app.run();
    system("pause");
    return 0;
}