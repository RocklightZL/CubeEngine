#include "pch.h"
#include "Animator.h"

namespace Cube {

    AnimatorClip::AnimatorClip(const std::string& name, bool looping, float speed) : name(name), looping(looping), speed(speed), duration(0.0f) {}
    AnimatorClip::AnimatorClip(std::initializer_list<AnimatorFrame> frameList, const std::string& name, bool looping, float speed) : frames(frameList), name(name), looping(looping), speed(speed), duration(0.0f) {
        for(const auto& f : frameList) {
            this->duration += f.duration;
        }
    }

    void AnimatorClip::addFrame(const TextureRegion& textureRegion, float duration) {
        frames.push_back({textureRegion, duration});
        this->duration += duration;
    }

    TextureRegion AnimatorClip::getFrameAtTime(float time) const {
        float accumulateTime = 0.0f;
        for(auto& frame : frames) {
            accumulateTime += frame.duration;
            if(accumulateTime >= time) {
                return frame.textureRegion;
            }
        }
        return frames.back().textureRegion;
    }

    const std::string& AnimatorClip::getName() const { return name; }
    bool AnimatorClip::isLooping() const { return looping; }
    float AnimatorClip::getSpeed() const { return speed; }
    float AnimatorClip::getDuration() const { return duration; }

}  // namespace Cube