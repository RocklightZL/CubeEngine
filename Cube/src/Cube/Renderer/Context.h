#pragma once

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Cube/Resource/Resource.h"
#include "Cube/Resource/ResourceManager.h"

#include <vector>

namespace Cube {

    struct QuadData {
		glm::mat4 modelMatrix;
		glm::vec4 color;
		glm::vec4 textureCoord;
	};

	class Context {
	public:
		friend class Renderer2D;

		Context();
		~Context();

		ResourceManager& getResourceManager();
	private:
		ResourceManager resourceManager;
		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<VertexBuffer> vbo; // TODO: ∏ƒ”√¬„÷∏’Î
		std::vector<QuadData> batchData;
		unsigned int batchCnt = 0;
		Shader* shader = nullptr;
		Shader* defaultShader = nullptr;
		Texture2D* currentTex = nullptr;
		Texture2D* whiteTex = nullptr;
	};

}