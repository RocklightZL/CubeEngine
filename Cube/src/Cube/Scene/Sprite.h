#pragma once
#include "Node2D.h"
#include "Cube/Renderer/Color.h"
#include "Cube/Renderer/Texture.h"
#include "Cube/Renderer/TextureRegion.h"

namespace Cube {

    class Sprite : public Node2D {
    public:
        Sprite() = default;
        ~Sprite() override;

        void update(float delta) override;

        nlohmann::json serialize() const override;
        void deserialize(const nlohmann::json& data) override;
        std::string getType() const override { return "Sprite"; }

    protected:
        Texture2D* texture = nullptr;
        TextureRegion texRegion = {{0.0f, 0.0f}, {1.0f, 1.0f}};
        Color tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
    };

}