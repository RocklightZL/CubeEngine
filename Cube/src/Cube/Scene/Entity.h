#pragma once
#include <glm/glm.hpp>

#include "Component.h"
#include "Transform.h"
#include "Cube/Reflection/ClassRegistry.h"
#include "Cube/Reflection/Type.h"

#include <json.hpp>

namespace Cube {

	// GameObject in scene
	class Entity {
	public:
		Entity() = default;
		Entity(const std::string& name) : name(name) {}
		~Entity() = default;

		void start();
		void update(float delta);

		template<typename T, typename... Args>
		T* addComponent(Args&&... args) {
			static_assert(std::is_base_of_v<Component, T>);
			TypeID typeID = getTypeID<T>();
			if(componentsMap.find(typeID) != componentsMap.end()) {
				CB_CORE_ERROR("Entity::addComponent<T>(): component of type '{}' already exists", ClassRegistry::get().getClass<T>()->getName());
				return static_cast<T*>(componentsMap[typeID]);
			}
			std::unique_ptr<Component> component = std::make_unique<T>(std::forward<Args>(args)...);
            Component* ptr = component.get();
            ptr->entity = this;
            componentsMap[typeID] = ptr;
            components.push_back(std::move(component));
			return static_cast<T*>(ptr);
        }

		// TODO: Lazy deletion may be considered later.
		void removeComponent(TypeID typeID) {
			auto it = componentsMap.find(typeID);
			if(it == componentsMap.end()) {
				CB_CORE_ERROR("Entity::removeComponent<T>(): component of type '{}' not found", ClassRegistry::get().getClass(typeID)->getName());
				return;
			}
			Component* compPtr = it->second;
			componentsMap.erase(it);
			components.erase(std::remove_if(components.begin(), components.end(), [compPtr](const std::unique_ptr<Component>& c) {
				return c.get() == compPtr;
			}), components.end());
		}

		template<typename T>
		void removeComponent() {
			static_assert(std::is_base_of_v<Component, T>);
            removeComponent(getTypeID<T>());
		}

		template<typename T>
		T* getComponent() const {
			static_assert(std::is_base_of_v<Component, T>);
			TypeID typeID = getTypeID<T>();
			auto it = componentsMap.find(typeID);
			if(it == componentsMap.end()) {
				return nullptr;
			}
			return static_cast<T*>(it->second);
        }

		const std::vector<std::unique_ptr<Component>>& getComponents() const { return components; }
        const std::unordered_map<TypeID, Component*>& getComponentsMap() const { return componentsMap; }

		template<typename T>
		bool hasComponent() const {
			static_assert(std::is_base_of_v<Component, T>);
			TypeID typeID = getTypeID<T>();
			return componentsMap.find(typeID) != componentsMap.end();
        }

		bool isAlive() const { return alive; }
        void destroy() { alive = false; }

		const std::string& getName() const;
		void setName(const std::string& name);
        Transform& getTransform();

		void deserialize(const nlohmann::json& data);
        nlohmann::json serialize() const;

	private:
        std::string name;  // Temporarily used as the unique identifier
		bool alive = true;
        std::vector<std::unique_ptr<Component>> components;  // for iteration
        std::unordered_map<TypeID, Component*> componentsMap;  // for lookup
		Transform transform = Transform(this);
	};

}