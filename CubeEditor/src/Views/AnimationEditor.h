#include "App/EditorApp.h"
#include "View.h"

#include "Cube/Event/Event.h"

class AnimationEditor : public View{
public:
    class TargetChangeEvent : public Cube::Event {
    public:
        EVENT_TYPE(TargetChangeEvent)

        TargetChangeEvent(const std::string& targetFilePath) : targetFilePath(targetFilePath) {}
        std::string targetFilePath;
    };

    AnimationEditor(EditorPage& editorPage) : View(editorPage) {
        EditorApp::get().getEventDispatcher().subscribe<TargetChangeEvent>([this](const Cube::Event& e) {
            const TargetChangeEvent& event = static_cast<const TargetChangeEvent&>(e);
            target = event.targetFilePath;
            return true;
        });
    }
    ~AnimationEditor() override = default;

    void render(float deltaTime) override;

private:
    std::string target;
};