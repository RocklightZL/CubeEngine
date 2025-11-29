// test GameObject-Component
#define TEST07
#ifdef TEST07
#include "Cube.h"
#include "Cube/Scene/Sprite.h"

using namespace Cube;

int main() {
    Application app;
    app.getSceneManager().registerScene("scene", []() {
        Scene* scene = new Scene("scene");
        auto a = scene->createEntity("a");
        auto sc = a->addComponent<Sprite>();
        sc->texture = ResPtr<Texture2D>("assets/texture/test02.jpg");
        return scene;
    });
    app.run();
}

#endif
