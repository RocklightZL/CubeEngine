#pragma once
#include "Cube/Event/Event.h"
#include "Window.h"
#include "LayerStack.h"
#include "Cube/Scene/RenderServer.h"
#include "Cube/Scene/Scene.h"

namespace Cube {
    class Application {
	public:
		Application();
		Application(const WindowPros& windowPros);
		virtual ~Application();

		virtual void run();

		Window* getWindow();

		bool onWindowClose(const Event& e);
		bool onWindowResize(const Event& e);

	protected:
		Window* mainWindow;
		bool running;
		LayerStack layers;
		RenderServer renderServer;

	private:
		static bool isInitialized;
		void init();
	};
}