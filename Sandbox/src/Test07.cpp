// test GameObject-Component
// #define TEST07
#ifdef TEST07
#include "Cube.h"
#include "Cube/Scene/Camera2D.h"
#include "Cube/Scene/Sprite.h"

using namespace Cube;

int main() {
    Application app;
    Renderer2D::setVSync(true);
    SceneManager& sceneManager = app.getSceneManager();
    sceneManager.registerScene("scene", [&app]() {
        Scene* scene = new Scene("scene", true);
        auto a = scene->createEntity("a");
        auto sc = a->addComponent<Sprite>();
        sc->texture = ResPtr<Texture2D>("assets/texture/test07.jpg");
        Transform& tr = a->getTransform();
        // tr.setPosition({400.0f, 300.0f});
        // tr.setScale({1, 1});
        auto camera = scene->createEntity("camera");
        auto cc =  camera->addComponent<Camera2D>();
        cc->viewport = {app.getWindow()->getPros().width, app.getWindow()->getPros().height};
        // cc->zoom = 2;
        Transform& cameraTr = camera->getTransform();
        // cameraTr.setPosition({-200.0f, -200.0f});
        return scene;
    });
    // sceneManager.registerScene("Test01", "assets/scenes/Test01.scene");
    sceneManager.registerScene("Test01", "assets/scenes/out_scene.scene");
    sceneManager.load("Test01");
    sceneManager.setActive("Test01");
    Scene* scene = sceneManager.getActive();
    // scene->serialize("assets/scenes/out_scene.scene");
    app.run();
    return 0;
}

#endif
