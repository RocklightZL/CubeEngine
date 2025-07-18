#include "pch.h"
#include "SpriteAnimator.h"

namespace Cube {

    SpriteAnimator::~SpriteAnimator() {
        for(auto& clip : clips) {
            delete clip.second;
        }
    }

    void SpriteAnimator::update(float deltaTime) {
        if(!playing || !currentClip) return;
        currentTime += deltaTime * currentClip->getSpeed();
        if(currentTime > currentClip->getDuration()) {
            if(currentClip->isLooping()) {
                currentTime -= currentClip->getDuration();
            }else {
                currentTime = currentClip->getDuration();
            }
        }
    }

    TextureRegion SpriteAnimator::getCurrentFrame() {
        if(currentClip) {
            return currentClip->getFrameAtTime(currentTime);
        }
        return {};
    }

    void SpriteAnimator::addClip(AnimatorClip* clip) {
        clips.insert({clip->getName(), clip});
        if(currentClip == nullptr) {
            currentClip = clip;
        }
    }

    void SpriteAnimator::play(const std::string& clipName) {
        playing = true;
        currentTime = 0.0f;
        auto clip = clips.find(clipName);
        if(clip != clips.end()) {
            currentClip = clip->second;
        }else {
            CB_CORE_ERROR("Clip not found");
        }
    }

    void SpriteAnimator::stop() {
        playing = false;
    }

}  // namespace Cube