#include "pch.h"
#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace Cube {

    Transform::~Transform() {
        if(parent) {
            parent->removeChild(this);
        }
        for(auto& c : children) {
            c->parent = nullptr;
        }
    }

    const glm::mat4& Transform::getLocalMatrix() {
        if(localDirty()) {
            localMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
            localMatrix = glm::rotate(localMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            localMatrix = glm::scale(localMatrix, glm::vec3(scale, 1.0f));
            clearLocalDirty();
            for(auto& child : children) {
                child->markWorldDirty();
            }
        }
        return localMatrix;
    }

    const glm::mat4& Transform::getWorldMatrix() {
        if(worldDirty()) {
            worldMatrix = parent ? parent->getWorldMatrix() * getLocalMatrix() : getLocalMatrix();
            clearWorldDirty();
        }
        return worldMatrix;
    }

    void Transform::setPosition(const glm::vec2& position) {
        this->pos = position;
        markLocalDirty();
    }
    const glm::vec2& Transform::getPosition() const {
        return pos;
    }
    void Transform::setRotation(float degrees) {
        this->rotation = degrees;
        markLocalDirty();
    }
    float Transform::getRotation() const {
        return rotation;
    }
    void Transform::setScale(const glm::vec2& scale) {
        this->scale = scale;
        markWorldDirty();
    }
    const glm::vec2& Transform::getScale() const {
        return scale;
    }

    glm::vec2 Transform::getWorldPos() const {
        return parent ? parent->getWorldPos() + pos : pos;
    }
    glm::vec2 Transform::getWorldRotation() const {
        return parent ? parent->getWorldRotation() + glm::vec2(rotation) : glm::vec2(rotation);
    }
    glm::vec2 Transform::getWorldScale() const {
        return parent ? parent->getWorldScale() * scale : scale;
    }

    void Transform::addChild(Transform* child) {
        children.push_back(child);
    }
    void Transform::removeChild(const Transform* child) {
        auto it = std::find(children.begin(), children.end(), child);
        if(it != children.end()) {
            children.erase(it);
        }else {
            CB_CORE_ERROR("Transform::removeChild: child not found");
        }
    }
}  // namespace Cube