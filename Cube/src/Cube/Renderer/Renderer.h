#pragma once

#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glm/glm.hpp>
#include <memory>

namespace Cube {

	class Renderer {
	public:
		static void init();
		static void beginFrame();
		static void endFrame();
		static void setViewport(int width, int height);
		static void setClearColor(float r, float g, float b, float a);
		static void setVSync(bool val);
	};

    class Renderer2D : public Renderer {
    public:
		static void init();
		static void beginFrame(const Camera2D& camera);
		static void endFrame();
		static void shutdown();
		static void drawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), const glm::mat4& transform = glm::mat4(1.0f));
		static void drawQuad(const glm::vec2& pos, const glm::vec2& size, const Texture2D& texture, const glm::vec4& tintColor = glm::vec4(1.0f), const glm::mat4& transform = glm::mat4(1.0f));
		static void setShader(const std::shared_ptr<Shader>& inShader); // 自定义着色器
    private:
		static std::shared_ptr<Shader> shader;
		static std::shared_ptr<VertexArray> vao;
    };

	// 默认着色器
	// clang-format off
	const std::string DEFAULT_2D_VERTEX_SHADER_SRC = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTextureCoord;

        uniform mat4 u_ViewProjectMatrix;
        uniform mat4 u_ModelMatrix;

        out vec2 textureCoord;

        void main(){
            textureCoord = aTextureCoord;
            gl_Position = u_ViewProjectMatrix * u_ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }

    )";
	const std::string DEFAULT_2D_FRAGMENT_SHADER_SRC = R"(
        #version 330 core

        in vec2 textureCoord;

        uniform sampler2D u_Texture;
        uniform int u_UseTexture;
        uniform vec4 u_Color;

        out vec4 color;

        void main(){
            if(bool(u_UseTexture)){
                color = texture(u_Texture, textureCoord) * u_Color;
            } else{
                color = u_Color;
            }
        }
    )";
	// clang-format on
}