/*
#include "Cube/Resource/ResourceManager.h"

#include <Cube.h>

static Cube::Application* app = nullptr;
static int width = 1920;
static int height = 1080;
static float x = 0.0f;
static float y = 0.0f;
static float cameraX = 0.0f;
static float cameraY = 0.0f;

class TransformSystem : public Cube::System {
public:
    void onUpdate(Cube::Scene* scene, float deltaTime) override {
        processInput(deltaTime);
        std::vector<Cube::Entity*> players = scene->getEntitiesWith<Cube::TransformComponent, Cube::SpriteComponent>();
        for(auto p : players) {
            p->getComponent<Cube::TransformComponent>()->position = {x, y};
        }
        std::vector<Cube::Entity*> cameras = scene->getEntitiesWith<Cube::TransformComponent, Cube::CameraComponent>();
        for(auto c : cameras) {
            c->getComponent<Cube::TransformComponent>()->position = {cameraX, cameraY};
            // c->getComponent<Cube::TransformComponent>()->scale = {cameraX + 1, cameraX + 1};
        }
    }

    std::string getName() const override {
        return "TransformSystem";
    }
private:
    void processInput(float deltaTime) {
        if(app->getWindow()->isKeyPressed(Cube::Key::Up)) {
            cameraY += 100.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Cube::Key::Down)) {
            cameraY -= 100.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Cube::Key::Right)) {
            cameraX += 100.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Cube::Key::Left)) {
            cameraX -= 100.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Cube::Key::D)) {
            x += 100.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Cube::Key::A)) {
            x -= 100.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Cube::Key::W)) {
            y += 100.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Cube::Key::S)) {
            y -= 100.0f * deltaTime;
        }
    }

};

class MainScene : public Cube::Scene {
public:
    MainScene() {
        // Cube::Renderer2D::setVSync(true);
        // ≥ı ºªØ≥°æ∞
        for(int i = 0; i < 250; i++){
            Cube::Entity* player = createEntity();
            auto tr = player->addComponent<Cube::TransformComponent>();
            tr->position = {100.0f, 100.0f};
            tr->scale = {100.0f, 100.0f};
            auto sp = player->addComponent<Cube::SpriteComponent>();
            sp->name = "player";
            // sp->alas = std::make_shared<Cube::TextureAlas>("assets/texture/test03.jpg");
            // sp->alas->addSubTexture("player", {"player", {0,0}, {1.0,1.0},{0,0}});
            // sp->alas = Cube::ResourceManager::getInstance().load<Cube::TextureAlas>("assets/texture/test03.jpg", "assets/texture/test03.json")->data;
        }
        setViewportSize({width, height});
        Cube::Entity* camera = createEntity();
        camera->addComponent<Cube::TransformComponent>();
        camera->addComponent<Cube::CameraComponent>()->available = true;
        addSystem(new Cube::RenderSystem());
        addSystem(new TransformSystem);
    }

    ~MainScene() override {
        Cube::ResourceManager::getInstance().releaseAll();
    };

    void onUpdate(float deltaTime) override {
        Scene::onUpdate(deltaTime);
        CB_INFO("FPS: {}", 1.0f/deltaTime);
    }
};

int main() {
    app = new Cube::Application({width, height, "Sandbox"});
    MainScene* scene = new MainScene;
    Cube::Renderer2D::init();
    app->setMainScene(scene);
    app->run();
    Cube::Renderer2D::shutdown();
    delete app;
    return 0;
}
*/