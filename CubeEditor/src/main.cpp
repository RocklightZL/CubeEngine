#include <iostream>

#include "App/EditorApp.h"
#include "App/GuidancePage.h"
#include "Cube/Core/Engine.h"
#include "Cube/Core/Log.h"
#include "Project.h"

EditorApp* app = nullptr;
Project* proj = nullptr;

int main() {
    Cube::Engine::init();
    app = new EditorApp({1920, 1080, "Cube Editor"});
    app->switchPage(new GuidancePage);
    app->run();

    delete proj;
    delete app;
    return 0;
}
