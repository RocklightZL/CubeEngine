#pragma once
#include "ThumbnailManager.h"

#include "View.h"

struct AssetNode;

class ResourcesPanel : public View {
public:
    ResourcesPanel(EditorPage& editorPage) : View(editorPage) {}
	~ResourcesPanel() override = default;

	void render(float deltaTime) override;

private:
    ThumbnailManager thumbnailManager;
};