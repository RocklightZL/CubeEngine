#pragma once

#include "Cube/Renderer/Texture.h"

#include <cstdint>
#include <glm/glm.hpp>

namespace Cube {

	using ComponentID = uint32_t;

	class Component {
	public:
	    virtual ~Component() = default;

		// ����ģ�庯���ھ�̬���������Թ���������ID
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
	// only entities with both TransformComponent and SpriteComponent can be rendered
	class TransformComponent : public Component {
	public:
		glm::vec2 position = {0.0f, 0.0f};
		float rotation = 0.0f;
		glm::vec2 scale = {1.0f, 1.0f}; // size

		// = modelMatrix
		glm::mat4 getTransformMatrix();
	};

    struct TextureRegion {
        glm::vec2 uvMin;
		glm::vec2 uvMax;
    };

	class SpriteComponent : public Component {
	public:
		std::string name;
		TextureAlas* alas = nullptr;
		TextureRegion region;
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	};

	class CameraComponent : public Component {
	public:
		bool available = false;
	};

    class AnimatorComponent {
    public:

    };
}