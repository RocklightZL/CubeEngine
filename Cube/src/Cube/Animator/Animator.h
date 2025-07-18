#pragma once

#include "Cube/Renderer/TextureRegion.h"

#include <string>
#include <vector>

namespace Cube {

    struct AnimatorFrame {
        TextureRegion textureRegion;
        float duration; // 持续时间，单位为秒
    };

    class AnimatorClip {
    public:
        AnimatorClip(const std::string& name, bool looping = true, float speed = 1.0f);
        AnimatorClip(std::initializer_list<AnimatorFrame> frameList, const std::string& name, bool looping = true, float speed = 1.0f);
        ~AnimatorClip() = default;

        void addFrame(const TextureRegion& textureRegion, float duration);

        TextureRegion getFrameAtTime(float time) const;

        const std::string& getName() const;
        bool isLooping() const;
        float getSpeed() const;
        float getDuration() const;
    private:
        std::vector<AnimatorFrame> frames;
        std::string name;
        bool looping;
        float speed;
        float duration;
    };

}