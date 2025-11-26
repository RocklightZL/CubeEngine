#pragma once

#include <unordered_map>
#include "Component.h"

namespace Cube {
	// 空容器，不会被继承，拥有组件所有权
	class Entity {
	public:
		Entity();
		Entity(const std::string& name);
		virtual ~Entity();

		template<typename T, typename... Args>
		T* addComponent(Args&&... args) {
		    if(hasComponent<T>()) {
		        removeComponent<T>();
		    }
			T* component = new T(std::forward<Args>(args)...);
			components[Component::getTypeID<T>()] = component;
			return component;
		}

		template<typename T>
		bool hasComponent() {
		    return components.find(Component::getTypeID<T>()) != components.end();
		}

		template<typename T>
		void removeComponent() {
		    auto it = components.find(Component::getTypeID<T>());
			if(it != components.end()) {
			    delete it->second;
				components.erase(it);
			}
		}

		template<typename T>
		T* getComponent() {
            auto it = components.find(Component::getTypeID<T>());
            if(it != components.end()) {
                return static_cast<T*>(it->second);
            }
            return nullptr;
        }

        uint32_t getID() const;

		const std::string& getName() const;
		void setName(const std::string& name);

		void destroy();

	    bool isAlive();

		const std::unordered_map<ComponentID, Component*>& getAllComponents() const;
		//-----------------------------------------------------
		// based on the component type name
        Component* addComponent(const std::string& componentTypeName);
		Component* getComponent(const std::string& componentTypeName);
		bool hasComponent(const std::string& componentTypeName);
		void removeComponent(const std::string& componentTypeName);

    private:
		uint32_t id;
		std::string name;
		bool alive = true;
		std::unordered_map<ComponentID, Component*> components;
		static uint32_t currentID;

	};

}