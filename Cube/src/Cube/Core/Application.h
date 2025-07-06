#pragma once
#include "Cube/Event/Event.h"
#include "Window.h"

namespace Cube {
	class Application {
	public:
		Application();
		virtual ~Application();

		void run();
		void init();
		// �¼�����
		bool onWindowClose(const Event& e);
		bool onWindowResize(const Event& e);
	private:
		EventDispatcher dispatcher;
		Window* mainWindow;
		bool running;
	};
}