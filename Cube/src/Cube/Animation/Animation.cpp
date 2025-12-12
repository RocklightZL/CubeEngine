#include "pch.h"
#include "Animation.h"

#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Sprite.h"

namespace Cube {

    void Animation::start() {
        sprite = entity->getComponent<Sprite>();
        CB_ASSERT(sprite && "No Sprite component!");
    }

    void Animation::update(float deltaTime) {
        if(!playing || !currentClip) return;
        currentTime += deltaTime * currentClip->getSpeed();
        if(currentTime > currentClip->getDuration()) {
            if(currentClip->isLooping()) {
                currentTime -= currentClip->getDuration();
            }else {
                currentTime = currentClip->getDuration();
            }
        }
        sprite->texRegion = getCurrentFrame();
    }

    TextureRegion Animation::getCurrentFrame() {
        if(currentClip) {
            return currentClip->getFrameAtTime(currentTime);
        }
        return {};
    }

    void Animation::addClip(RUID animClipRuid) {
        ResPtr<AnimationClip> clip(animClipRuid);
        if(clip) {
            clips[clip->getName()] = std::move(clip);
        }
    }

    void Animation::play(const std::string& clipName) {
        playing = true;
        currentTime = 0.0f;
        auto clip = clips.find(clipName);
        if(clip != clips.end()) {
            currentClip = clip->second.get();
            sprite->texture = clip->second->getTexture();
        }else {
            CB_CORE_ERROR("Clip not found");
        }
    }

    void Animation::stop() {
        playing = false;
    }

}  // namespace Cube