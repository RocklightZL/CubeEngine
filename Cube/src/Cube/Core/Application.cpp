#include "Application.h"
#include "Log.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Cube {

Application::Application() {}
Application::~Application() {}

void Application::run() {
	Log::init();
	CB_CORE_INFO("Application run");
	while (true);
}

}  // namespace Cube