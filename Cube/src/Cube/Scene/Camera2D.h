#pragma once
#include "Component.h"

namespace Cube {

    class Camera2D : public Component{
    public:
        Camera2D() = default;
        ~Camera2D() override = default;

        bool isAvailable() const { return available; }
        void setAvailable(bool val) { available = val; }

    private:
        bool available;
    };

}