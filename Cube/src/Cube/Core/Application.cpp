#include "pch.h"
#include "Application.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Window.h"

namespace Cube {

Application::Application() : dispatcher(){}
Application::~Application() {
	
}

void Application::run() {
	running = true;
	init();
	CB_CORE_INFO("Application run");
	while(running) {
		glfwPollEvents();
	}
}

void Application::init() {
	Log::init();
	mainWindow = new Window(WindowPros(800, 600, "Cube Engine"), &dispatcher);

	dispatcher.subscribe(std::bind(&Application::onWindowClose, this, std::placeholders::_1), EventType::WindowClose);
}

bool Application::onWindowClose(const Event& e) {
	delete mainWindow;
	running = false;
	CB_CORE_INFO("mainWindow close");
	return true;
}

}  // namespace Cube