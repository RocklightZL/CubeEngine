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
		std::shared_ptr<Shader> shader; // TODO: ��Ҫע������Ȩ���⣬Ŀǰ����Ȩ��Context�࣬����Shader����Ҫ��Ϊһ����Դ������Դ����������
		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<VertexBuffer> vbo;
		std::vector<QuadData> batchData;
		unsigned int batchCnt;
		Texture2D* currentTex = nullptr;
		Texture2D* whiteTex = nullptr;
	};

}