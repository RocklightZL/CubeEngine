#pragma once

#include "Cube/Renderer/Texture.h"
#include "Cube/Animator/SpriteAnimator.h"
#include "Cube/Renderer/TextureRegion.h"

#include <cstdint>
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>

namespace Cube {
    using ComponentID = uint32_t;

	class ComponentFactory;

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

		static void registerComponentType(const std::string& typeName, ComponentFactory* componentFactory);
		static Component* createComponent(const std::string& typeName);
		static ComponentID getTypeID(const std::string& typeName);

	protected:
		Component() = default;
	private:
		static std::unordered_map<std::string, std::unique_ptr<ComponentFactory>>& components();

		static ComponentID currentID;
	};

	// reflection support
	class ComponentFactory {
	public:
		virtual ~ComponentFactory() = default;
		virtual Component* create() = 0;
		virtual ComponentID getTypeID() = 0;
	};

	template<typename T>
	class ComponentFactoryImpl : public ComponentFactory{
	public:
		T* create() override {
			return new T();
		}
		ComponentID getTypeID() override {
			return Component::getTypeID<T>();
		}
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

	class SpriteComponent : public Component {
	public:
		Texture2D* texture = nullptr;
		TextureRegion region = {{0.0f, 0.0f}, {1.0f, 1.0f}};
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	};

	class CameraComponent : public Component {
	public:
		bool available = false;
	};

    class AnimatorComponent : public Component{
    public:
		SpriteAnimator animator;
    };
}