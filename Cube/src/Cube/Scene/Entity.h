#pragma once
#include <glm/glm.hpp>

#include "Component.h"
#include "Transform.h"
#include "Cube/Reflection/ClassRegistry.h"
#include "Cube/Reflection/Type.h"

namespace Cube {

	class Entity {
	public:
		Entity() = default;
		Entity(const std::string& name) : name(name){}
		virtual ~Entity() = default;

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

		const std::unordered_map<TypeID, std::unique_ptr<Component>>& getComponents() const { return components; }

		template<typename T>
		bool hasComponent() const {
			TypeID typeID = getTypeID<T>();
			return components.find(typeID) != components.end();
        }

		bool isAlive() const { return alive; }
        void destroy() { alive = false; }

		const std::string& getName() const;
		void setName(const std::string& name);

	private:
        std::string name;  // Temporarily used as the unique identifier
		bool alive = true;
		std::unordered_map<TypeID, std::unique_ptr<Component>> components;
		Transform transform = Transform(this);
	};

}