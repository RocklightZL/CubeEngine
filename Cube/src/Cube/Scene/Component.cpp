#include "pch.h"
#include "Component.h"

#include <glm/ext/matrix_transform.hpp>

namespace Cube {

    ComponentID Component::currentID = 0;

    void Component::registerComponentType(const std::string& typeName, ComponentFactory* componentFactory) { components()[typeName] = std::unique_ptr<ComponentFactory>(componentFactory); }

    Component* Component::createComponent(const std::string& typeName) {
        auto it = components().find(typeName);
        if(it != components().end()) {
            return it->second->create();
        }
        CB_CORE_ERROR("Failed to create component! Unknown component type: {}", typeName);
        return nullptr;
    }

    ComponentID Component::getTypeID(const std::string& typeName) {
        auto it = components().find(typeName);
        if(it != components().end()) {
            return it->second->getTypeID();
        }
        CB_CORE_ERROR("Failed to get type ID! Unknown component type: {}", typeName);
        return 0;
    }

    std::unordered_map<std::string, std::unique_ptr<ComponentFactory>>& Component::components() {
        static std::unordered_map<std::string, std::unique_ptr<ComponentFactory>> c;
        return c;
    }

    // TransformComponent
    glm::mat4 TransformComponent::getTransformMatrix() const{
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scale, 1.0f));
        return transform;
    }
}  // namespace Cube