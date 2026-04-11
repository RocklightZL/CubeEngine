#pragma once
#include <memory>
#include <vector>

#include "../Views/View.h"
#include "Page.h"
#include "Cube/Scene/Entity.h"
#include "../Scene/EditorCamera.h"
#include "../Project/AssetExplorer.h"
#include "Views/ThumbnailManager.h"

namespace Cube {
    class Scene;
}
struct SceneData;
class Project;
class EditorApp;

class EditorPage : public Page {
public:
    EditorPage(Project* project);
    ~EditorPage() override;

    void render(float deltaTime) override;
    Page::Type getType() const override { return Page::Type::Editor; }

    void importFromFileDialog();

    Project* getProject() const { return project.get(); }

    // Editor state
    SceneData* selectedScene = nullptr;
    Cube::Entity* selectedEntity = nullptr;
    AssetNode* selectedAssetNode = nullptr;
    EditorCamera editorCamera;

    ThumbnailManager thumbnailManager;

private:
    std::vector<std::unique_ptr<View>> views;
    std::unique_ptr<Project> project;
};