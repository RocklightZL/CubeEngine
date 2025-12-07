#pragma once

namespace Cube {
    class Application;

    class RenderServer {
    public:
		RenderServer() = default;
		~RenderServer() = default;

	private:
		Application* app;
	};

}