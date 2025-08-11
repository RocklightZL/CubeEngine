#pragma once

#include "Cube/Renderer/Texture.h"

#include <string>
#include <vector>
#include <glm/vec2.hpp>

namespace Cube {

	// ����TexturePacker���ɵ�Json(Array)�����ļ�
	class TextureData {
	public:
		std::string filePath;
		std::string imagePath;
		glm::vec2 size;
		std::vector<SubTexture> textures;

	    TextureData(const std::string& filePath, const std::string& imagePath);
	};

}