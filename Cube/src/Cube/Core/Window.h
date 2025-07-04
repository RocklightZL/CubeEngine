#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace Cube{

struct WindowPros{
	int width = 800;
	int height = 600;
	std::string title = "Cube Engine";
	WindowPros(int width, int height, std::string title) : width(width), height(height), title(title){}
};

class Window {
public:
	Window(const WindowPros& pros);
	virtual ~Window();
	virtual void init();
private:
	static int windowCnt;
	WindowPros pros;
	GLFWwindow* window;
};
}
