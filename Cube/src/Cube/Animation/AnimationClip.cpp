#include "pch.h"
#include "Animation.h"

#include "Cube/Resource/ResourceManager.h"

#include <fstream>
#include <json.hpp>

namespace Cube {
    // .anim
    AnimationClip::AnimationClip(const AssetMeta& assetMeta) : ResourceBase(assetMeta) {
        std::ifstream file(assetMeta.sourcePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("Failed to open animation file {}", assetMeta.sourcePath);
            return;
        }
        nlohmann::json animData;
        file >> animData;
        file.close();
        name = animData["name"];
        looping = animData["looping"];
        speed = animData["speed"];
        duration = animData["duration"];
        texture = ResPtr<Texture2D>(animData["texture"]);
        for(auto& f : animData["frames"]) {
            AnimationFrame af;
            af.textureRegion = {
                {f["textureRegion"]["uvMin"]["x"], f["textureRegion"]["uvMin"]["y"]},
                {f["textureRegion"]["uvMax"]["x"], f["textureRegion"]["uvMax"]["y"]}
            };
            af.duration = f["duration"];
            frames.push_back(af);
        }
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
    const ResPtr<Texture2D>& AnimationClip::getTexture() const { return texture; }

}  // namespace Cube