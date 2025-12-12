#pragma once
#include <thread>

#include "Cube/Core/Window.h"
#include "Page.h"

namespace Cube {
    class Application;
}
class EditorApp{
public:
    static const std::string userConfigDir;

    EditorApp(const Cube::WindowPros& windowPros);
    ~EditorApp();

    void switchPage(Page* page);
    void run();

    Cube::Window* getWindow() const;

    // global
    std::thread gameThread;
    Cube::Application* game = nullptr;

private:
    std::unique_ptr<Cube::Window> mainWindow;
    std::unique_ptr<Page> currentPage;
    bool running;
    Cube::EventDispatcher eventDispatcher;

    void imGuiInit();
    void setDarkTheme();
    bool onWindowClose(const Cube::Event& e);
};