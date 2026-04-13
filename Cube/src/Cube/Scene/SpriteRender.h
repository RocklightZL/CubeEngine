#pragma once
#include "Component.h"
#include "Cube/Renderer/Color.h"
#include "Cube/Renderer/Texture.h"
#include "Cube/Renderer/TextureRegion.h"
#include "Cube/Resource/ResPtr.h"
#include "Cube/Resource/Sprite.h"
#include "Transform.h"

namespace Cube {

    class SpriteRender : public Component {
    public:
        ResPtr<Sprite> sprite;
        Color tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
        int order = 0;

        SpriteRender() = default;
        SpriteRender(const std::string& sprite, const Color& tintColor)
            : sprite(sprite), tintColor(tintColor){}
        TypeID getType() const override { return getTypeID<SpriteRender>(); }
    };

}