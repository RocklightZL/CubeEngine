// TODO: 添加统一的测试框架
// test GameObject-Component
#define TEST08
#ifdef TEST08
#include "Cube.h"
#include "Cube/Animation/Animation.h"
#include "Cube/Core/Engine.h"
#include "Cube/Core/Input.h"
#include "Cube/Scene/Camera2D.h"
#include "Cube/Scene/Sprite.h"

using namespace Cube;

class Controller : public Component {
public:
    void update(float delta) override;
};

void Controller::update(float delta) {
    static bool flag = false;
    if(!flag) {
        entity->getComponent<Animation>()->play("flower");
        flag = true;
    }
    Transform& tr = entity->getTransform();
    glm::vec2 pos = tr.getPosition();
    if(Input::isKeyPressed(Key::D)) {
        pos.x += 500.0f * delta;
    }
    if(Input::isKeyPressed(Key::A)) {
        pos.x -= 500.0f * delta;
    }
    if(Input::isKeyPressed(Key::W)) {
        pos.y += 500.0f * delta;
    }
    if(Input::isKeyPressed(Key::S)) {
        pos.y -= 500.0f * delta;
    }
    tr.setPosition(pos);
}

int main() {
    Engine::init();
    Application app;
    Renderer2D::setVSync(true);
    SceneManager& sceneManager = app.getSceneManager();
    sceneManager.registerScene("scene", [&app]() {
        Scene* scene = new Scene("scene", true);

        auto a = scene->createEntity("a");
        Transform& tr = a->getTransform();
        tr.setPosition({400.0f, 300.0f});
        auto sc = a->addComponent<Sprite>();
        auto ac = a->addComponent<Animation>();
        ac->addClip("assets/texture/flower.anim");
        a->addComponent<Controller>();

        auto camera = scene->createEntity("camera");
        auto cc =  camera->addComponent<Camera2D>();
        cc->viewport = {app.getWindow()->getPros().width, app.getWindow()->getPros().height};

        return scene;
    });
    sceneManager.load("scene");
    sceneManager.setActive("scene");
    app.run();
    return 0;
}

#endif
