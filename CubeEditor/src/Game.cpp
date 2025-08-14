#include "Game.h"

#include "Project.h"
#include "Cube/Core/Application.h"
#include "Cube/Core/Log.h"
#include "Cube/Scene/SceneSerializer.h"

using namespace Cube;

extern Project* proj;

void gameThreadFunction(bool* isGameStarted) {
	CB_EDITOR_TRACE("gameThread begin");

	Scene* s = proj->selectedScene->scene;
	Application* game = new Application({(int)s->getViewportSize().x, (int)s->getViewportSize().y, s->getName()});

	Scene* mainScene = new Scene();
	SceneSerializer::deserialize(mainScene, proj->getConfig().sceneDirectory + "/" + s->getName() + ".scene");
	game->setMainScene(mainScene);
	game->run();

	delete game;
    *isGameStarted = false;
	CB_EDITOR_TRACE("gameThread exit");
}