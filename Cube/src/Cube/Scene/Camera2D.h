#pragma once
#include "Node2D.h"

namespace Cube {

    class Camera2D : public Node2D{
    public:
        Camera2D() = default;
        ~Camera2D() override = default;

        void update(float delta) override;

        nlohmann::json serialize() const override;
        void deserialize(const nlohmann::json& data) override;
        std::string getType() const override { return "Camera2D"; }

        bool isAvailable() const { return available; }
        void setAvailable(bool val) { available = val; }
    protected:
        bool available;
    };

}