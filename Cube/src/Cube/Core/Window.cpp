#include "Window.h"
#include "Log.h"

namespace Cube {

int Window::windowCnt = 0;

Window::Window(const WindowPros& pros) : pros(pros) {
    init();
}

Window::~Window() {
    glfwDestroyWindow(window);
    --windowCnt;
    if(!windowCnt){
        glfwTerminate();
    }
}

void Window::init() {
    if(!windowCnt){
        if(!glfwInit()){
            CB_CORE_ERROR("glfwInit failed!");
        }
        CB_CORE_INFO("glfw initialize");
    }
    window = glfwCreateWindow(pros.width, pros.height, pros.title.c_str(), nullptr, nullptr);
}
}  // namespace Cube
