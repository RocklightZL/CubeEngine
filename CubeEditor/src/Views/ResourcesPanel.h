#pragma once
#include <memory>

#include "Cube/Renderer/Texture.h"
#include "View.h"

struct AssetNode;

class ResourcesPanel : public View {
public:
	ResourcesPanel() = default;
	~ResourcesPanel() override = default;

	void render(float deltaTime) override;

    static void importFromFileDialog();
    static void importResource(const std::string& path);

private:
    std::unique_ptr<Cube::Texture2D> back_png        = std::make_unique<Cube::Texture2D>("assets/icons/back.png");
    std::unique_ptr<Cube::Texture2D> icon_mode_png   = std::make_unique<Cube::Texture2D>("assets/icons/icon_mode.png");
    std::unique_ptr<Cube::Texture2D> list_mode_png   = std::make_unique<Cube::Texture2D>("assets/icons/list_mode.png");
    std::unique_ptr<Cube::Texture2D> directory_png   = std::make_unique<Cube::Texture2D>("assets/icons/directory.png");
    std::unique_ptr<Cube::Texture2D> file_png        = std::make_unique<Cube::Texture2D>("assets/icons/file.png");
};