#include "Project.h"
#include "App/EditorApp.h"
#include "App/EditorLayer.h"
#include "App/GuidanceLayer.h"

Cube::EditorApp* app = nullptr;
Cube::Project* proj = nullptr;

int main() {
    app = new Cube::EditorApp({1920, 1080, "Cube Editor"});

    app->switchLayer(new Cube::GuidanceLayer());

    app->run();
    delete proj;
    delete app;
    return 0;
}