#pragma once
#include "Cube/Event/Event.h"
#include "Window.h"
#include "Cube/Scene/RenderServer.h"
#include "Cube/Scene/SceneManager.h"

namespace Cube {

	// TODO: 考虑改成单例模式，或者在Application外再包一层GameEngine类，GameEngine用单例管理Application，这样保证了Application被继承的可能。

    class Application {
	public:
		Application();
		Application(const WindowPros& windowPros);
		virtual ~Application();

		virtual void run();

		Window* getWindow();

		RenderServer& getRenderServer();
        SceneManager& getSceneManager();

		bool onWindowClose(const Event& e);

	protected:
		Window* mainWindow;
		bool running;
		RenderServer renderServer;
        SceneManager sceneManager;
	};
}