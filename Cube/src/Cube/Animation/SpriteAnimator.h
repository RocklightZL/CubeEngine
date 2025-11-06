#pragma once

#include "Animation.h"
#include "Cube/Scene/Component.h"

#include <unordered_map>
#include <string>

namespace Cube {

    class SpriteAnimator {
    public:
        SpriteAnimator() = default;
        ~SpriteAnimator();

        void update(float deltaTime);
        TextureRegion getCurrentFrame();

        void addClip(AnimationClip* clip);

        void play(const std::string& clipName);
        void stop();

    private:
        bool playing = false;
        float currentTime = 0.0f;
        AnimationClip* currentClip = nullptr;
        std::unordered_map<std::string, AnimationClip*> clips;
    };

    class AnimatedSpriteComponent : public Component{
    public:
        
    };

}  // namespace Cube