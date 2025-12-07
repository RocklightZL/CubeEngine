#pragma once
#include "Cube/Core/Window.h"
#include "Page.h"

class EditorApp{
public:
    static const std::string userConfigDir;

    EditorApp(const Cube::WindowPros& windowPros);
    ~EditorApp();

    void switchPage(Page* page);
    void run();

    Cube::Window* getWindow() const;

    // global
    std::unordered_map<std::string, Cube::Texture2D*> icons;

private:
    std::unique_ptr<Cube::Window> mainWindow;
    std::unique_ptr<Page> currentPage;
    bool running;

    void imGuiInit();
    void loadAssets();
    void releaseAssets();
    void setDarkTheme();
    bool onWindowClose(const Cube::Event& e);
};