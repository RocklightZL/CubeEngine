#pragma once

#include "Cube/Renderer/Texture.h"

#include <string>
#include <vector>
#include <glm/vec2.hpp>

// 解析TexturePacker生成的Json(Array)数据文件
class TextureData {
public:
	std::string filePath;
	std::string imagePath;
	glm::vec2 size;
	std::vector<Cube::SubTexture> textures;

	TextureData(const std::string& filePath, const std::string& imagePath);
};