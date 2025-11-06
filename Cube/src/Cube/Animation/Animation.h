#pragma once

#include "Cube/Renderer/Texture.h"
#include "Cube/Renderer/TextureRegion.h"

#include <string>
#include <vector>

namespace Cube {

    struct AnimationFrame {
        TextureRegion textureRegion;
        float duration; // unit: second
    };

    class AnimationClip : public ResourceBase{
    public:
        AnimationClip(const std::string& animationFilePath);
        ~AnimationClip();

        TextureRegion getFrameAtTime(float time) const;

        const std::string& getName() const;
        bool isLooping() const;
        float getSpeed() const;
        float getDuration() const;
    private:
        Texture2D* texture;
        std::vector<AnimationFrame> frames;
        std::string name;
        bool looping;
        float speed;
        float duration;
    };

}