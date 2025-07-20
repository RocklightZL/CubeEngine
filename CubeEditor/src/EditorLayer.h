#pragma once
#include "Cube/Core/Layer.h"

namespace Cube {

    class EditorLayer : public Layer {
    public:
        EditorLayer() = default;
        ~EditorLayer() override;

        void onUpdate() override;
        void onAttach() override;
        void onDetach() override;
    };

}