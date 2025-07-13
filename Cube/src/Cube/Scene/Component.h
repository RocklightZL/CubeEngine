#pragma once

#include "Cube/Renderer/Texture.h"

#include <cstdint>
#include <glm/glm.hpp>

namespace Cube {

	using ComponentID = uint32_t;

	class Component {
	public:
	    virtual ~Component() = default;

		// 利用模板函数内静态变量的特性关联类型与ID
		template <typename T>
		static uint32_t getTypeID(){
			static_assert(std::is_base_of<Component, T>::value);
			static uint32_t typeID = currentID++;
			return typeID;
		}
	protected:
		Component() = default;
	private:
		static ComponentID currentID;
	};

	// basic built-in components
	class TransformComponent : public Component {
	public:
		glm::vec2 position = {0.0f, 0.0f};
		float rotation = 0.0f;
		glm::vec2 scale = {1.0f, 1.0f};

		glm::mat4 getTransformMatrix();
	};

	class SpriteComponent : public Component {
	public:
		std::string name;
		std::shared_ptr<TextureAlas> alas = nullptr;
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	};

}