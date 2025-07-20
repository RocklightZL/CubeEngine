#include "EditorLayer.h"

#include <Cube/Core/Application.h>

Cube::Application* app = nullptr;

int main() {
    app = new Cube::Application({1920, 1080, "Cube Editor"});
    app->pushLayer(new Cube::EditorLayer);
    app->run();
    return 0;
}