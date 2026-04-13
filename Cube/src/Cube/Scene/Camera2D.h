#pragma once
#include "Entity.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Cube {

    class Camera2D : public Component{
    public:
        glm::vec2 viewport = {1280.0f, 720.0f};
        float zoom = 1.0f; // no rotation for now
        bool available = true;

        Camera2D() = default;
        TypeID getType() const override { return getTypeID<Camera2D>(); }

        glm::mat4 getPVMatrix() const {
            glm::mat4 pvMatrix = glm::ortho(0.0f, viewport.x, 0.0f, viewport.y, -0.1f, 1.0f);
            glm::vec2 position = entity->getTransform().getWorldPos();
            pvMatrix = pvMatrix * glm::inverse(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)), glm::vec3(zoom, zoom, 1.0f)));
            return pvMatrix;
        }
    };
}