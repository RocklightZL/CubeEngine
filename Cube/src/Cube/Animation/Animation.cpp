#include "pch.h"
#include "Animation.h"

#include "Cube/Resource/ResourceManager.h"

#include <fstream>
#include <json.hpp>

namespace Cube {
    // .anim.res
    AnimationClip::AnimationClip(const std::string& animationFilePath) : ResourceBase(animationFilePath) {
        std::ifstream file(animationFilePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("Failed to open animation file {}", animationFilePath);
            return;
        }
        nlohmann::json animData;
        file >> animData;
        file.close();
        name = animData["name"];
        looping = animData["looping"];
        speed = animData["speed"];
        duration = animData["duration"];
        texture = ResourceManager::get().load<Texture2D>(animData["texture"]);
        for(auto& f : animData["frames"]) {
            AnimationFrame af;
            af.textureRegion = {{f["textureRegion"][0], f["textureRegion"][1]}, {f["textureRegion"][2], f["textureRegion"][3]}};
            af.duration = animData["duration"];
            frames.push_back(af);
        }
    }

    AnimationClip::~AnimationClip() {
        ResourceManager::get().release(texture);
    }

    TextureRegion AnimationClip::getFrameAtTime(float time) const {
        float accumulateTime = 0.0f;
        for(auto& frame : frames) {
            accumulateTime += frame.duration;
            if(accumulateTime >= time) {
                return frame.textureRegion;
            }
        }
        return frames.back().textureRegion;
    }

    const std::string& AnimationClip::getName() const { return name; }
    bool AnimationClip::isLooping() const { return looping; }
    float AnimationClip::getSpeed() const { return speed; }
    float AnimationClip::getDuration() const { return duration; }

}  // namespace Cube