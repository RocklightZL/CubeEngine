#pragma once
#include "Cube/Renderer/FrameBuffer.h"
#include "Cube/Scene/Scene.h"

namespace Cube {
    // store data
    class Model {
    public:
        Scene scene;
        FrameBuffer frameBuffer;
        Entity* selectedEntity = nullptr;
    };
}