#pragma once
#include <thread>

#include "Cube/Core/Window.h"
#include "Page.h"

namespace Cube {
    class Application;
}

class EditorApp{
public:
    static EditorApp& get();

    static const std::string userConfigDir;

    void switchPage(Page* page);
    void run();

    Cube::Window* getWindow() const;
    Cube::EventDispatcher& getEventDispatcher(){ return eventDispatcher; }

    // global
    std::thread gameThread;
    Cube::Application* game = nullptr;
    std::vector<std::string> projectsPathCache;

private:
    std::unique_ptr<Cube::Window> mainWindow;
    std::unique_ptr<Page> currentPage;
    bool running;
    Cube::EventDispatcher eventDispatcher;

    EditorApp(const Cube::WindowPros& windowPros);
    ~EditorApp();

    void imGuiInit();
    void loadConfig();
    void saveConfig();
    void setDarkTheme();
    bool onWindowClose(const Cube::Event& e);
};