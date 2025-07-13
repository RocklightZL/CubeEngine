#pragma once

#include "Cube/Scene/System.h"

namespace Cube {

    class RendererSystem : public System {
    public:
        void onUpdate(Scene* scene) override;
    };

}  // namespace Cube