#include "pch.h"
#include "Entity.h"

namespace Cube {

    const std::string& Entity::getName() const {
        return name;
    }
    void Entity::setName(const std::string& name) {
        this->name = name;
    }

}  // namespace Cube