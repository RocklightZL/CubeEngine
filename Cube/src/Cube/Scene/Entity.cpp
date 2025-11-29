#include "pch.h"
#include "Entity.h"

namespace Cube {

    void Entity::start() {
        for(auto& [typeID, component] : components) {
            component->start();
        }
    }

    void Entity::update(float delta) {
        for(auto& [typeID, component] : components) {
            component->update(delta);
        }
    }

    const std::string& Entity::getName() const {
        return name;
    }
    void Entity::setName(const std::string& name) {
        this->name = name;
    }
    Transform& Entity::getTransform() {
        return transform;
    }

}  // namespace Cube