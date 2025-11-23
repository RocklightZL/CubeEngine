#pragma once
#include "Component.h"
#include "Cube/Renderer/Color.h"
#include "Cube/Renderer/Texture.h"
#include "Cube/Renderer/TextureRegion.h"

namespace Cube {

    class Sprite : public Component {
    public:
        Sprite() = default;
        Sprite(const std::string& texturePath, const TextureRegion& textureRegion, const Color& tintColor);
        ~Sprite() override;

        TextureRegion texRegion = {{0.0f, 0.0f}, {1.0f, 1.0f}};
        Color tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
    private:
        Texture2D* texture = nullptr;
    };

}