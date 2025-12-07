#include "Game.h"

#include "Project.h"
#include "Cube/Core/Application.h"
#include "Cube/Core/Log.h"

using namespace Cube;

extern Project* proj;

// class PreGame : public Application {
// public:
//     PreGame(const WindowPros& windowPros, const std::shared_ptr<Scene>& scene) : Application(windowPros) {
//         layers.pushLayer(std::make_shared<SceneLayer>(scene));
//     }
// };

void gameThreadFunction(bool* isGameStarted) {
	CB_EDITOR_TRACE("gameThread begin");

	// Scene* s = proj->selectedScene->scene;
 //
	// std::shared_ptr<Scene> mainScene = std::make_shared<Scene>();
	// SceneSerializer::deserialize(mainScene.get(), proj->getConfig().sceneDirectory + "/" + s->getName() + ".scene");
 //
	// PreGame* game = new PreGame({(int)s->getViewportSize().x, (int)s->getViewportSize().y, s->getName()}, mainScene);
 //
	// game->run();
 //
	// delete game;
 //    *isGameStarted = false;
	CB_EDITOR_TRACE("gameThread exit");
}