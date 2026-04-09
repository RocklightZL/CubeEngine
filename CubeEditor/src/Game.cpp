#include "Game.h"

#include "App/EditorApp.h"
#include "Cube/Core/Application.h"
#include "Cube/Core/Log.h"
#include "Cube/Resource/ResourceManager.h"
#include "Cube/Scene/Scene.h"
#include "Project/Project.h"

using namespace Cube;

void gameThreadFunction(EditorApp* app, Project* project, Scene* scene, bool* isGameOver) {
	CB_EDITOR_TRACE("gameThread begin");
    Application game({1280, 720, scene->getName()});  // TODO: viewport size
    app->game = &game;
    ResourceManager::init(project->getConfig().assetPathMapFilePath);
    const std::string sceneName = scene->getName();
    game.getSceneManager().registerScene(sceneName, project->getConfig().sceneDirectory + "/" + sceneName + ".scene");  // TODO: scene file path
    game.getSceneManager().load(sceneName);
    game.getSceneManager().setActive(sceneName);
	game.run();
	CB_EDITOR_TRACE("gameThread exit");
    *isGameOver = true;
    app->game = nullptr;
}