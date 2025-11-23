#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "Component.h"
#include "Cube/Reflection/ClassRegistry.h"
#include "Cube/Reflection/Type.h"

namespace Cube {
	struct Transform {
	    glm::vec2 pos = {0.0f, 0.0f};
		float rotation = 0.0f;
		glm::vec2 scale = {1.0f, 1.0f};

		Transform(const glm::vec2& pos, float rotation, const glm::vec2& scale)
	        : pos(pos), rotation(rotation), scale(scale){}
		Transform() = default;

		glm::mat4 getMatrix() const;
	};

	class Entity {
	public:
		Entity() = default;
		Entity(const std::string& name) : name(name){}
		virtual ~Entity() = default;

		virtual void start();
		virtual void update(float delta);

		void addChild(const std::string& name);
        void removeChild(const std::string& name);
        Entity* getChild(const std::string& name) const;

		template<typename T, typename... Args>
		Component* addComponent(Args&&... args) {
			TypeID typeID = getTypeID<T>();
			if(components.find(typeID) != components.end()) {
				CB_CORE_ERROR("Entity::addComponent<T>(): component of type '{}' already exists", ClassRegistry::get().getClass<T>()->getName());
				return nullptr;
			}
			components[typeID] = std::make_unique<T>(std::forward<Args>(args)...);
            components[typeID]->entity = this;
			return components[typeID].get();
        }

		template<typename T>
		void removeComponent() {
            TypeID typeID = getTypeID<T>();
            auto it = components.find(typeID);
			if(it == components.end()) {
				CB_CORE_ERROR("Entity::removeComponent<T>(): component of type '{}' not found", ClassRegistry::get().getClass<T>()->getName());
				return;
            }
			components.erase(it);
		}

		template<typename T>
		Component* getComponent() const {
			TypeID typeID = getTypeID<T>();
			auto it = components.find(typeID);
			if(it == components.end()) {
				CB_CORE_ERROR("Entity::getComponent<T>(): component of type '{}' not found", ClassRegistry::get().getClass<T>()->getName());
				return nullptr;
			}
			return it->second.get();
        }

		template<typename T>
		bool hasComponent() const {
			TypeID typeID = getTypeID<T>();
			return components.find(typeID) != components.end();
        }

		const std::string& getName() const;
		const Transform& getTransform() const;
		glm::vec2 getWorldPos() const;
		glm::mat4 getWorldMatrix() const;
		Entity* getParent() const;

		void setName(const std::string& name);
		void setTransform(const Transform& transform);
		void setPos(const glm::vec2& pos);
		void setRotation(float rotation);
		void setScale(const glm::vec2& scale);

	private:
		std::string name;
		Transform transform;
		Entity* parent = nullptr;
		std::unordered_map<std::string, std::unique_ptr<Entity>> children;
		std::unordered_map<TypeID, std::unique_ptr<Component>> components;
	};

}