#pragma once

#include "App/EditorApp.h"
#include "View.h"

#include "Cube/Event/Event.h"

#include <string>
#include <vector>

class AnimationEditor : public View{
public:
    class TargetChangeEvent : public Cube::Event {
    public:
        EVENT_TYPE(TargetChangeEvent)

        TargetChangeEvent(const std::string& targetFilePath) : targetFilePath(targetFilePath) {}
        std::string targetFilePath;
    };

    AnimationEditor(EditorPage& editorPage) : View(editorPage) {
        EditorApp::get().getEventDispatcher().subscribe<TargetChangeEvent>(std::bind(&AnimationEditor::onTargetChange, this, std::placeholders::_1));
    }
    ~AnimationEditor() override = default;

    void render(float deltaTime) override;

private:
    struct FrameViewData {
        std::string frame;
        float duration = 0.0f;
    };

    bool loadTargetAnim();
    bool createNewAnimationClip(const std::string& fileName);

    bool onTargetChange(const Cube::Event& e);

    std::string target;
    std::string name;
    bool looping = false;
    float speed = 1.0f;
    float duration = 0.0f;
    std::vector<FrameViewData> frames;
};