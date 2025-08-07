#pragma once
#include "Cube/Event/Event.h"
#include "Window.h"
#include "LayerStack.h"
#include "Cube/Scene/Scene.h"

namespace Cube {
	class Application {
	public:
		static void init();

		Application();
		Application(const WindowPros& windowPros);
		virtual ~Application();

		virtual void run();

		LayerStack* getLayers();
		Window* getWindow();

		void pushLayer(Layer* layer);

		void setMainScene(Scene* scene);
		// �¼�����
		bool onWindowClose(const Event& e);
		bool onWindowResize(const Event& e);
	protected:
		EventDispatcher dispatcher;
		Window* mainWindow;
		bool running;
		LayerStack layers;
		Scene* mainScene;
	};
}