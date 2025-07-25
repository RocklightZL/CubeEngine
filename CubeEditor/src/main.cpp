#include "Model.h"
#include "App/EditorApp.h"
#include "App/EditorLayer.h"

Cube::EditorApp* app = nullptr;
Cube::Model* data = nullptr;

int main() {
    app = new Cube::EditorApp({1920, 1080, "Cube Editor"});
    data = new Cube::Model();
    app->switchLayer(new Cube::EditorLayer());
    app->run();
    delete data;
    return 0;
}