#include "glad/glad.h"
#include <Cube.h>
#include <glm/ext/matrix_transform.hpp>

Cube::Application* app;
float cameraX;
float cameraY = 1.0f;
float cameraR;
float cameraZoom = 1.0f;
float x = 100.0f;
float y = 100.0f;
float r;
float zoom = 1.0f;

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

class TestLayer2 : public Cube::Layer {
public:
    TestLayer2();
    void onUpdate() override;
    void processInput();

private:
    std::shared_ptr<Cube::Texture2D> texture;
    std::shared_ptr<Cube::Camera2D> camera;
};

TestLayer2::TestLayer2() {
    Cube::Renderer2D::init();
    texture = std::make_shared<Cube::Texture2D>("assets/texture/test03.jpg");
    camera = std::make_shared<Cube::Camera2D>(app->getWindow()->getPros().width, app->getWindow()->getPros().height);
}

void TestLayer2::onUpdate() {
    Cube::Renderer2D::setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    camera->setPosition({cameraX, cameraY});
    Cube::Renderer2D::beginFrame(camera->getPVMatrix());
    Cube::Renderer2D::drawQuad({x, y}, {100.0f, 100.0f}, texture, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f, {0.0f, 0.0f, 0.5f, 0.5f});
    // for(int i = 0; i < 50; i++) {
    //     for(int j = 0; j < 50; j++) {
    //         Cube::Renderer2D::drawQuad({x + 105 * i, y + 105 * j}, {100.0f, 100.0f}, texture, {1.0f, 1.0f, 1.0f, 1.0f}, 45.0f, {0.0f, 0.0f, 0.5f, 0.5f});
    //     }
    // }
    Cube::Renderer2D::endFrame();
    processInput();
}

void TestLayer2::processInput() {
    if(app->getWindow()->isKeyPressed(Cube::Key::Up)) {
        cameraY += 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::Down)) {
        cameraY -= 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::Right)) {
        cameraX += 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::Left)) {
        cameraX -= 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::D)) {
        x += 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::A)) {
        x -= 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::W)) {
        y += 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::S)) {
        y -= 1.0f;
    }
}

int main() {
    app = new Cube::Application({1920, 1080, "Sandbox"});
    TestLayer2* testLayer2 = new TestLayer2();
    app->getLayers()->pushLayer(testLayer2);
    app->run();
    return 0;
}