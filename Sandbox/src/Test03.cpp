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
    void onUpdate(Cube::Scene* scene) override {
        processInput();
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
private:
    void processInput() {
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

};

class MainScene : public Cube::Scene {
public:
    MainScene() {
        // ³õÊ¼»¯³¡¾°
        Cube::Entity* player = createEntity();
        auto tr = player->addComponent<Cube::TransformComponent>();
        tr->position = {100.0f, 100.0f};
        tr->scale = {100.0f, 100.0f};
        auto sp = player->addComponent<Cube::SpriteComponent>();
        sp->name = "player";
        sp->alas = std::make_shared<Cube::TextureAlas>("assets/texture/test03.jpg");
        sp->alas->addSubTexture("player", {"player", {0,0}, {1.0,1.0},{0,0}});

        Cube::Entity* camera = createEntity();
        camera->addComponent<Cube::TransformComponent>();
        camera->addComponent<Cube::CameraComponent>()->available = true;
        addSystem(new Cube::RenderSystem(width, height));
        addSystem(new TransformSystem);
    }

    ~MainScene() override {
        
    };

    void onUpdate() override {
        Scene::onUpdate();
    }
};

int main() {
    app = new Cube::Application({width, height, "Sandbox"});
    MainScene* scene = new MainScene;
    app->setMainScene(scene);
    app->run();
    delete app;
    return 0;
}