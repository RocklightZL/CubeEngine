#pragma once
#include "Cube/Event/Event.h"
#include "Window.h"
#include "LayerStack.h"

namespace Cube {
	class Application {
	public:
		Application(const WindowPros& windowPros);
		virtual ~Application();

		void run();
		void init();

		LayerStack* getLayers();
		Window* getWindow();
		// 事件处理
		bool onWindowClose(const Event& e);
		bool onWindowResize(const Event& e);
	private:
		EventDispatcher dispatcher;
		Window* mainWindow;
		bool running;
		LayerStack layers;
	};
}