#pragma once
#include "Cube/Core/Layer.h"
#include "Cube/Core/Window.h"

namespace Cube {

    class EditorApp {
    public:
        EditorApp(const WindowPros& windowPros);
        ~EditorApp();

        void run();
        void switchLayer(Layer* layer);

        void createGameWindow(const WindowPros& pros);

        Window* getWindow() const;

    private:
        void init();
        void imGuiInit();

        bool onWindowClose(const Event& e);
        bool onWindowResize(const Event& e);

        Window* mainWindow = nullptr;
        Window* gameWindow = nullptr;
        bool running = false;
        EventDispatcher dispatcher;
        Layer* currentLayer = nullptr;
    };

}