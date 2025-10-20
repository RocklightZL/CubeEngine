#pragma once
#include "Cube/Core/Application.h"
#include "Cube/Core/Layer.h"
#include "Cube/Core/Window.h"

namespace Cube {

    class EditorApp : public Application{
    public:
        static const std::string userConfigDir;

        EditorApp(const WindowPros& windowPros);
        ~EditorApp();

        void switchLayer(const std::shared_ptr<Layer>& layer);

        Window* getWindow() const;

        // global
        std::unordered_map<std::string, Texture2D*> icons;

    private:
        void imGuiInit();

        void loadAssets();
        void releaseAssets();

        void setDarkTheme();
    };

}