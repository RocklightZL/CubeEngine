// // Animator System Test
//
// #include "Cube/Resource/ResourceManager.h"
//
// #include <Cube.h>
// #include <iostream>
//
// static Cube::Application* app = nullptr;
// static int width = 1920;
// static int height = 1080;
// static float x = 0.0f;
// static float y = 0.0f;
// static float cameraX = 0.0f;
// static float cameraY = 0.0f;
// static Cube::Timer timer;
//
// class TransformSystem : public Cube::System {
// public:
//     void onUpdate(Cube::Scene* scene, float deltaTime) override {
//         processInput(deltaTime);
//         std::vector<Cube::Entity*> players = scene->getEntitiesWith<Cube::TransformComponent, Cube::SpriteComponent>();
//         for(auto p : players) {
//             p->getComponent<Cube::TransformComponent>()->position = {x, y};
//         }
//         std::vector<Cube::Entity*> cameras = scene->getEntitiesWith<Cube::TransformComponent, Cube::CameraComponent>();
//         for(auto c : cameras) {
//             c->getComponent<Cube::TransformComponent>()->position = {cameraX, cameraY};
//             // c->getComponent<Cube::TransformComponent>()->scale = {cameraX + 1, cameraX + 1};
//         }
//     }
// private:
//     void processInput(float deltaTime) {
//         if(app->getWindow()->isKeyPressed(Cube::Key::Up)) {
//             cameraY += 1000.0f * deltaTime;
//         }
//         if(app->getWindow()->isKeyPressed(Cube::Key::Down)) {
//             cameraY -= 1000.0f * deltaTime;
//         }
//         if(app->getWindow()->isKeyPressed(Cube::Key::Right)) {
//             cameraX += 1000.0f * deltaTime;
//         }
//         if(app->getWindow()->isKeyPressed(Cube::Key::Left)) {
//             cameraX -= 1000.0f * deltaTime;
//         }
//         if(app->getWindow()->isKeyPressed(Cube::Key::D)) {
//             x += 1000.0f * deltaTime;
//         }
//         if(app->getWindow()->isKeyPressed(Cube::Key::A)) {
//             x -= 1000.0f * deltaTime;
//         }
//         if(app->getWindow()->isKeyPressed(Cube::Key::W)) {
//             y += 1000.0f * deltaTime;
//         }
//         if(app->getWindow()->isKeyPressed(Cube::Key::S)) {
//             y -= 1000.0f * deltaTime;
//         }
//         if(app->getWindow()->isKeyPressed(Cube::Key::B)) {
//             std::cout << "FPS: " << 1/deltaTime << "\n";
//         }
//     }
//
// };
//
// class MainScene : public Cube::Scene {
// public:
//     MainScene() {
//         Cube::Entity* player = createEntity();
//         auto* tr = player->addComponent<Cube::TransformComponent>();
//         tr->scale = {130 * 3, 114 * 3};
//         tr->position = {500, 500};
//
//         Cube::SpriteComponent* sc = player->addComponent<Cube::SpriteComponent>();
//         sc->atlas = new Cube::TextureAtlas("assets/texture/flower.png");
//
//         Cube::AnimatorClip* animatorClip = new Cube::AnimatorClip("idle");
//         float iWidth = 130.0f;
//         float iHeight = 114.0f;
//         for(int j = 2; j >= 0; j--) {
//             for(int i = 0; i < 4; i++) {
//                 animatorClip->addFrame({{i * iWidth / 520.0f, j * iHeight / 342.0f}, {(i + 1) * iWidth / 520.0f, (j + 1) * iHeight / 342.0f}}, 0.1f);
//             }
//         }
//         Cube::AnimatorComponent* ac = player->addComponent<Cube::AnimatorComponent>();
//         ac->animator.addClip(animatorClip);
//         ac->animator.play("idle");
//
//         Cube::Entity* camera = createEntity();
//         camera->addComponent<Cube::TransformComponent>();
//         camera->addComponent<Cube::CameraComponent>()->available = true;
//
//         addSystem(new Cube::AnimatorSystem);
//         addSystem(new Cube::RenderSystem(width, height));
//         addSystem(new TransformSystem);
//         Cube::Renderer2D::setVSync(true);
//     }
//
//     ~MainScene() override {
//         Cube::ResourceManager::getInstance().releaseAll();
//     };
//
//     void onUpdate(float deltaTime) override {
//         Scene::onUpdate(deltaTime);
//         CB_INFO("FPS: {}", 1.0f/deltaTime);
//     }
// };
//
// int main() {
//     app = new Cube::Application({width, height, "Sandbox"});
//     Cube::Renderer2D::init();
//     MainScene* scene = new MainScene;
//     app->setMainScene(scene);
//     app->run();
//     Cube::Renderer2D::shutdown();
//     delete app;
//     return 0;
// }
