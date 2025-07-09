#pragma once
#include "Cube/Event/Event.h"
#include "Window.h"
#include "LayerStack.h"

namespace Cube {
	class Application {
	public:
		Application();
		virtual ~Application();

		void run();
		void init();

		LayerStack* getLayers();
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