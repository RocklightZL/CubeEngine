#pragma once

#include "Cube/Scene/Component.h"

#include <string>
#include <vector>

namespace Cube {

    struct AnimatorFrame {
        TextureRegion textureRegion;
        float duration; // ����ʱ�䣬��λΪ��
    };

    class AnimatorClip {
    public:
        AnimatorClip(const std::string& name, bool looping = true, float speed = 1.0f);
        ~AnimatorClip() = default;

        void addFrame(const TextureRegion& textureRegion, float duration);

        const std::string& getName() const;
        bool isLooping() const;
        float getSpeed() const;
    private:
        std::string name;
        bool looping;
        float speed;
        std::vector<AnimatorFrame> frames; 
    };

}