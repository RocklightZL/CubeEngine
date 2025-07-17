#pragma once

#include "Animator.h"

namespace Cube {

    class SpriteAnimator {
    public:
        SpriteAnimator() = default;
        ~SpriteAnimator();

        void update(float deltaTime);

        void addClip(AnimatorClip* clip);

        void play(const std::string& clipName);
        void stop();

    private:
        bool playing = false; 
        float currentTime = 0.0f;
        AnimatorClip* currentClip = nullptr;
        std::unordered_map<std::string, AnimatorClip*> clips;
    };

}