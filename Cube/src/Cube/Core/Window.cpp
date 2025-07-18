#include "pch.h"
#include "Window.h"

#include <iostream>

#include "Cube/Event/ApplicationEvent.h"
#include "Cube/Event/KeyEvent.h"
#include "Cube/Event/MouseEvent.h"

namespace Cube {

    int Window::windowCnt = 0;

    Window::Window(const WindowPros& pros, EventDispatcher* dispatcher) : pros(pros), dispatcher(dispatcher) { init(); }

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

        glfwMakeContextCurrent(window);

        glfwSetErrorCallback(windowErrorCallBack);

        glfwSetWindowCloseCallback(window, [](GLFWwindow* w) {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
            window->getDispatcher()->dispatch(WindowCloseEvent());
        });

        glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));

            window->pros.width = width;
            window->pros.height = height;

            window->getDispatcher()->dispatch(WindowResizeEvent(width, height));
        });

        glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
            switch(action) {
            case GLFW_PRESS:
                window->getDispatcher()->dispatch(KeyPressedEvent(key, false));
                break;
            case GLFW_REPEAT:
                window->getDispatcher()->dispatch(KeyPressedEvent(key, true));
                break;
            case GLFW_RELEASE:
                window->getDispatcher()->dispatch(KeyReleasedEvent(key));
                break;
            }
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
            double xPos, yPos;
            glfwGetCursorPos(w, &xPos, &yPos);
            switch(action) {
            case GLFW_PRESS:
                window->getDispatcher()->dispatch(MousePressedEvent(xPos, yPos, button));
                break;
            case GLFW_RELEASE:
                window->getDispatcher()->dispatch(MouseReleasedEvent(xPos, yPos, button));
                break;
            }
        });

        glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
            double xPos, yPos;
            glfwGetCursorPos(w, &xPos, &yPos);
            window->getDispatcher()->dispatch(MouseScrolledEvent(xPos, yPos, xoffset, yoffset));
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
            window->getDispatcher()->dispatch(MouseMovedEvent(xpos, ypos));
        });
    }

    EventDispatcher* Window::getDispatcher() const { return dispatcher; }

    const WindowPros& Window::getPros() const { return pros; }

    void Window::update() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool Window::isKeyPressed(KeyCode keyCode) {
        return glfwGetKey(window, keyCode) == GLFW_PRESS;
    }

    bool Window::isMouseButtonPressed(MouseCode mouseCode) {
        return glfwGetMouseButton(window, mouseCode) == GLFW_PRESS;
    }

    MousePos Window::getMousePosition() {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return {x, y};
    }

    void Window::windowErrorCallBack(int error_code, const char* description) { CB_CORE_ERROR("glfwWindowError: {}", description); }
}  // namespace Cube
