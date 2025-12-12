#include "Game.h"

#include "App/EditorApp.h"
#include "Cube/Core/Application.h"
#include "Cube/Core/Log.h"
#include "Project.h"

using namespace Cube;

extern EditorApp* app;
extern Project* proj;

void gameThreadFunction(bool* isGameOver) {
	CB_EDITOR_TRACE("gameThread begin");
    Scene* s = proj->selectedScene->scene;
    Application game({1280, 720, s->getName()});  // TODO: viewport size
    app->game = &game;
    game.getSceneManager().registerScene(s->getName(), proj->getConfig().sceneDirectory + "/" + s->getName() + ".scene");  // TODO: scene file path
    game.getSceneManager().load(s->getName());
    game.getSceneManager().setActive(s->getName());
	game.run();
	CB_EDITOR_TRACE("gameThread exit");
    *isGameOver = true;
    app->game = nullptr;
}