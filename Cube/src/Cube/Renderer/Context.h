#pragma once

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

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

		Context() = default;
		~Context();
	private:
		std::shared_ptr<Shader> shader; // TODO: 需要注意所有权问题，目前所有权在Context类，后续Shader可能要作为一种资源，由资源管理器管理。
		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<VertexBuffer> vbo;
		std::vector<QuadData> batchData;
		unsigned int batchCnt;
		Texture2D* currentTex = nullptr;
		Texture2D* whiteTex = nullptr;
	};

}