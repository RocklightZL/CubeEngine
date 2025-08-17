#pragma once
#include "Cube/Core/Layer.h"
#include "Cube/Core/Window.h"

namespace Cube {

    class EditorApp {
    public:
        static const std::string userConfigDir;

        EditorApp(const WindowPros& windowPros);
        ~EditorApp();

        void run();
        void switchLayer(Layer* layer);

        Window* getWindow() const;

        // global
        std::unordered_map<std::string, Texture2D*> icons;

    private:
        void init();
        void imGuiInit();

        void loadAssets();
        void releaseAssets();

        void setDarkTheme();

        bool onWindowClose(const Event& e);
        bool onWindowResize(const Event& e);

        Window* mainWindow = nullptr;
        bool running = false;
        EventDispatcher dispatcher;
        Layer* currentLayer = nullptr;
    };

}