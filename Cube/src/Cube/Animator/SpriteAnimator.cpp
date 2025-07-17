#include "pch.h"
#include "SpriteAnimator.h"

namespace Cube {

    SpriteAnimator::~SpriteAnimator() {
        for(auto& clip : clips) {
            delete clip.second;
        }
    }

    void SpriteAnimator::update(float deltaTime) {
        if(playing && currentClip) {
            currentTime += deltaTime;
        }
    }

    void SpriteAnimator::addClip(AnimatorClip* clip) { clips.insert({clip->getName(), clip}); }

    void SpriteAnimator::stop() {
        playing = false;
    }

}  // namespace Cube