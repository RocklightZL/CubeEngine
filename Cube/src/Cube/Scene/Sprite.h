#pragma once
#include "Component.h"
#include "Cube/Renderer/Color.h"
#include "Cube/Renderer/Texture.h"
#include "Cube/Renderer/TextureRegion.h"
#include "Cube/Resource/ResPtr.h"

namespace Cube {

    class Sprite : public Component {
    public:
        ResPtr<Texture2D> texture = nullptr;
        TextureRegion texRegion = {{0.0f, 0.0f}, {1.0f, 1.0f}};
        Color tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
        int order = 0;

        Sprite() = default;
        Sprite(const std::string& texturePath, const TextureRegion& textureRegion, const Color& tintColor)
            : texture(texturePath), texRegion(textureRegion), tintColor(tintColor){}
    };

}