#include "EditorLayer.h"
#include "Model.h"

#include <Cube/Core/Application.h>

Cube::Application* app = nullptr;
Cube::Model* data = nullptr;

int main() {
    app = new Cube::Application({1920, 1080, "Cube Editor"});
    data = new Cube::Model();
    app->pushLayer(new Cube::EditorLayer);
    app->run();
    delete data;
    return 0;
}