#include "pch.h"
#include "Sprite.h"

#include "Cube/Renderer/Renderer.h"

namespace Cube {

    Sprite::Sprite(const std::string& texturePath, const TextureRegion& textureRegion, const Color& tintColor) : texRegion(textureRegion), tintColor(tintColor){
        texture = ResourceManager::get().load<Texture2D>(texturePath);
    }

    Sprite::~Sprite() {
        if(texture) {
            ResourceManager::get().release(texture);
        }
    }
}  // namespace Cube