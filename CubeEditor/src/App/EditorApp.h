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

        Window* getWindow() const;

    private:
        void init();
        void imGuiInit();

        bool onWindowClose(const Event& e);
        bool onWindowResize(const Event& e);

        Window* mainWindow;
        bool running;
        EventDispatcher dispatcher;
        Layer* currentLayer;
    };

}