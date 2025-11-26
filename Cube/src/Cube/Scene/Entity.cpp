#include "pch.h"
#include "Entity.h"

namespace Cube {

    glm::mat4 Transform::getMatrix() const{
        glm::mat matrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        matrix = glm::scale(matrix, glm::vec3(scale, 1.0f));
        return matrix;
    }

    void Entity::addChild(Entity* entity) {
        children[entity->name] = entity;
        entity->parent = this;
    }

    void Entity::removeChild(const std::string& name) {
        auto it = children.find(name);
        if(it == children.end()) {
            CB_CORE_ERROR("Entity::removeChild(const std::string& name): child '{}' not found", name);
            return;
        }
        children.erase(it);
    }

    Entity* Entity::getChild(const std::string& name) const {
        auto it = children.find(name);
        if(it == children.end()) {
            CB_CORE_ERROR("Entity::getChild(const std::string& name): child '{}' not found", name);
            return nullptr;
        }
        return it->second;
    }

    const std::string& Entity::getName() const {
        return name;
    }

    const Transform& Entity::getTransform() const {
        return transform;
    }

    glm::vec2 Entity::getWorldPos() const {
        return parent ? transform.pos + parent->getWorldPos() : transform.pos;
    }

    glm::mat4 Entity::getWorldMatrix() const {
        return parent ? parent->getWorldMatrix() * transform.getMatrix();
    }

    Entity* Entity::getParent() const {
        return parent;
    }

    void Entity::setName(const std::string& name) {
        this->name = name;
    }

    void Entity::setTransform(const Transform& transform) {
        this->transform = transform;
    }

    void Entity::setPos(const glm::vec2& pos) {
        this->transform.pos = pos;
    }

    void Entity::setRotation(float rotation) {
        this->transform.rotation = rotation;
    }

    void Entity::setScale(const glm::vec2& scale) {
        this->transform.scale = scale;
    }

}  // namespace Cube