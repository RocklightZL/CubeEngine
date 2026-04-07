#include <iostream>

#include "App/EditorApp.h"
#include "App/GuidancePage.h"
#include "Cube/Core/Engine.h"
#include "Cube/Core/Log.h"

int main() {
    Cube::Engine::init();
    EditorApp& app = EditorApp::get();
    app.switchPage(new GuidancePage);
    app.run();
    return 0;
}
