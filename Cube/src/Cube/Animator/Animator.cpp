#include "pch.h"
#include "Animator.h"

namespace Cube {

    AnimatorClip::AnimatorClip(const std::string& name, bool looping, float speed) : name(name), looping(looping), speed(speed) {}

    void AnimatorClip::addFrame(const TextureRegion& textureRegion, float duration) { frames.push_back({textureRegion, duration}); }

    const std::string& AnimatorClip::getName() const { return name; }
    bool AnimatorClip::isLooping() const { return looping; }
    float AnimatorClip::getSpeed() const { return speed; }

}  // namespace Cube