#pragma once

#include <string>
#include <vector>

#include "Cube/Renderer/Texture.h"
#include "Cube/Renderer/TextureRegion.h"
#include "Cube/Resource/ResPtr.h"

namespace Cube {

    struct AnimationFrame {
        TextureRegion textureRegion;
        float duration; // unit: second
    };

    class AnimationClip : public ResourceBase{
    public:
        AnimationClip(const AssetMeta& assetMeta);
        ~AnimationClip() = default;

        TextureRegion getFrameAtTime(float time) const;

        const std::string& getName() const;
        bool isLooping() const;
        float getSpeed() const;
        float getDuration() const;
        const ResPtr<Texture2D>& getTexture() const;

    private:
        ResPtr<Texture2D> texture;  // use ResPtr instead of resource path to preload the texture resource
        std::vector<AnimationFrame> frames;
        std::string name;
        bool looping;
        float speed;
        float duration;
    };

}