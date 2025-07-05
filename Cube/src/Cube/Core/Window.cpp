#include "pch.h"
#include "Window.h"
#include "Cube/Event/ApplicationEvent.h"

namespace Cube {

    int Window::windowCnt = 0;

    Window::Window(const WindowPros& pros, EventDispatcher* dispatcher) : pros(pros), dispatcher(dispatcher){
        init();
    }

    Window::~Window() {
        glfwDestroyWindow(window);
        --windowCnt;
        if(!windowCnt) {
            glfwTerminate();
        }
    }

    void Window::init() {
        if(!windowCnt) {
            if(!glfwInit()) {
                CB_CORE_ERROR("glfwInit failed!");
            }
            CB_CORE_INFO("glfw initialize");
        }
        window = glfwCreateWindow(pros.width, pros.height, pros.title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetWindowCloseCallback(window, [](GLFWwindow* w) {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
            window->getDispatcher()->dispatch(WindowCloseEvent());
        });
    }
}  // namespace Cube
