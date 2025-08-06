#include "pch.h"
#include "Window.h"

#include <iostream>

#include "Cube/Event/ApplicationEvent.h"
#include "Cube/Event/KeyEvent.h"
#include "Cube/Event/MouseEvent.h"
#include "Timer.h"
#include "Cube/Renderer/Renderer.h"

#include <GLFW/glfw3native.h>

namespace Cube {

    int Window::windowCnt = 0;

    Window::Window(const WindowPros& pros, EventDispatcher* dispatcher, GLFWwindow* shareContext) : pros(pros), dispatcher(dispatcher) { init(shareContext); }

    Window::~Window() {
        glfwDestroyWindow(window);
        --windowCnt;
        if(!windowCnt) {
            glfwTerminate();
        }
        if(Renderer2D::currentContext == context) {
            Renderer2D::currentContext = nullptr;
        }
        delete context;
    }

    void Window::init(GLFWwindow* shareContext) {
        if(!windowCnt) {
            if(!glfwInit()) {
                CB_CORE_ERROR("glfwInit failed!");
            }
            CB_CORE_INFO("glfw initialize");
        }
        window = glfwCreateWindow(pros.width, pros.height, pros.title.c_str(), nullptr, shareContext);

        glfwSetWindowUserPointer(window, this);

        glfwMakeContextCurrent(window);
        context = new Context();
        Renderer2D::currentContext = context;
        Renderer2D::init();
        Renderer2D::setViewport(pros.width, pros.height);

        glfwSetErrorCallback(windowErrorCallBack);

        glfwSetWindowCloseCallback(window, [](GLFWwindow* w) {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
            window->dispatcher->dispatch(WindowCloseEvent(window));
        });

        glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
            Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));

            window->pros.width = width;
            window->pros.height = height;

            window->dispatcher->dispatch(WindowResizeEvent(window, width, height));
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

    GLFWwindow* Window::getNativeWindow() const { return window; }

    HWND Window::getWin32Window() {
        return glfwGetWin32Window(window);
    }

    void Window::update() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void Window::makeContext() const {
        glfwMakeContextCurrent(window);
        Renderer2D::currentContext = context;
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
