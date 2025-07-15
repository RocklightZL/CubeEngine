#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Cube/Event/Event.h"

namespace Cube{

struct WindowPros{
	int width = 800;
	int height = 600;
	std::string title = "Cube Engine";
	WindowPros(int width, int height, const std::string& title) : width(width), height(height), title(title){}
};

struct MousePos {
    double x;
	double y;
};

class Window {
public:
	Window(const WindowPros& pros, EventDispatcher* dispatcher);
	virtual ~Window();
	void init();
	EventDispatcher* getDispatcher() const;
    const WindowPros& getPros() const;
	void update();

	//  ‰»Î¬÷—Ø
	bool isKeyPressed(KeyCode keyCode);
	bool isMouseButtonPressed(MouseCode mouseCode);
	const MousePos& getMousePosition();

private:
	static void windowErrorCallBack(int error_code, const char* description);\

	static int windowCnt;
	WindowPros pros;
	GLFWwindow* window;
	EventDispatcher* dispatcher;
};
}
