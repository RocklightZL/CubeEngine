#include "pch.h"
#include "Entity.h"

namespace Cube {

	uint32_t Entity::currentID = 0;

    Entity::Entity() { id = currentID++; }
    Entity::Entity(const std::string& name) : Entity() {
        this->name = name;
    }

    Entity::~Entity() {
        for(auto& c : components) {
            delete c.second;
        }
    }

    uint32_t Entity::getID() const { return id; }

    const std::string& Entity::getName() const { return name; }
    void Entity::setName(const std::string& name) { this->name = name; }

    void Entity::destroy() { alive = false; }

    bool Entity::isAlive() { return alive; }

    const std::unordered_map<ComponentID, Component*>& Entity::getAllComponents() const {
        return components;
    }

    Component* Entity::addComponent(const std::string& componentTypeName) {
        ComponentID id = Component::getTypeID(componentTypeName);
        if(hasComponent(componentTypeName)) {
            removeComponent(componentTypeName);
        }
        Component* component = Component::createComponent(componentTypeName);
        components[id] = component;
        return component;
    }

    Component* Entity::getComponent(const std::string& componentTypeName) {
        ComponentID id = Component::getTypeID(componentTypeName);
        auto it = components.find(id);
        if(it != components.end()) {
            return it->second;
        }
        CB_CORE_ERROR("{} not found in {}", componentTypeName, name);
        return nullptr;
    }

    bool Entity::hasComponent(const std::string& componentTypeName) {
        return components.find(Component::getTypeID(componentTypeName)) != components.end(); 
    }

    void Entity::removeComponent(const std::string& componentTypeName) {
        auto it = components.find(Component::getTypeID(componentTypeName));
        if(it != components.end()) {
            delete it->second;
            components.erase(it);
        }
    }

}  // namespace Cube