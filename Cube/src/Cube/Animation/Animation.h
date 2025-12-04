#pragma once

#include <string>
#include <unordered_map>

#include "AnimationClip.h"
#include "Cube/Renderer/TextureRegion.h"
#include "Cube/Scene/Component.h"

namespace Cube {

    // Frame animation controller for sprites
    class Animation : public Component{
    public:
        std::unordered_map<std::string, ResPtr<AnimationClip>> clips;

        Animation() = default;
        ~Animation() override = default;

        void update(float deltaTime) override;
        TextureRegion getCurrentFrame();

        void addClip(const std::string& animClipFilePath);

        void play(const std::string& clipName);
        void stop();

    private:
        bool playing = false;
        float currentTime = 0.0f;
        AnimationClip* currentClip = nullptr;
    };

}  // namespace Cube