#pragma once
namespace Cube {

	class Renderer {
	public:
		static void init();
		static void beginFrame();
		static void endFrame();
		static void setViewport(int width, int height);
		static void setClearColor(float r, float g, float b, float a);

	};

}