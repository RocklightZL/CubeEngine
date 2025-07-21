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

    void Entity::destroy() { alive = false; }

    bool Entity::isAlive() {
        return alive;
    }

}  // namespace Cube