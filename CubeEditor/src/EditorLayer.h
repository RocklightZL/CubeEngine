#pragma once
#include "Cube/Core/Layer.h"
#include "Views/View.h"

#include <vector>

namespace Cube {

    class EditorLayer : public Layer {
    public:
        EditorLayer() = default;
        ~EditorLayer() override;

        void onUpdate(float deltaTime) override;
        void onAttach() override;
        void onDetach() override;
    private:
        std::vector<View*> views;
    };

}