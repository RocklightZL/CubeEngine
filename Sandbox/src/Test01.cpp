#include <memory>
#include <Cube.h>

class TestLayer : public Cube::Layer {
public:
    TestLayer();
    void onUpdate() override;

private:
    std::shared_ptr<Cube::Shader> shader;
    std::shared_ptr<Cube::VertexArray> vao;
    std::shared_ptr<Cube::Texture2D> texture;
};

TestLayer::TestLayer() {
    texture = std::make_shared<Cube::Texture2D>("assets/texture/test03.jpg");

    // clang-format off
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
    // clang-format on

    // shader = std::make_shared<Cube::Shader>(vertexShaderSrc, fragmentShaderSrc);
    // Shader shader(readFileToString("D:/mycode/vsProject/CubeEngine/bin/Debug-windows-x86_64/Sandbox/VertexShaderTest.vs"), readFileToString("D:/mycode/vsProject/CubeEngine/bin/Debug-windows-x86_64/Sandbox/FragmentShaderTest.fs"));
    shader = std::make_shared<Cube::Shader>(Cube::readFileToString("assets/shaders/VertexShaderTest.vs"), Cube::readFileToString("assets/shaders/FragmentShaderTest.fs"));

    std::vector<uint32_t> indices = {0, 1, 2, 0, 2, 3};

    auto vbo = std::make_shared<Cube::VertexBuffer>(vertices);
    auto ibo = std::make_shared<Cube::IndexBuffer>(indices);

    Cube::BufferLayout layout = {{Cube::ShaderDataType::Float3, "position"}, {Cube::ShaderDataType::Float2, "texture"}};
    vbo->setLayout(layout);
    vao = std::make_shared<Cube::VertexArray>();
    vao->addVertexBuffer(vbo);
    vao->setIndexBuffer(ibo);
}

void TestLayer::onUpdate() {
    Cube::Renderer::setClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    Cube::Renderer::beginFrame();

    texture->bind();
    vao->bind();
    shader->bind();
    glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

    Cube::Renderer::endFrame();
}

//=====================================================================