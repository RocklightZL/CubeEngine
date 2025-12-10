#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

class EditorCamera {
public:
    glm::vec2 position = {0, 0};
    float zoom = 1;
    glm::vec2 viewport = {1280.0f, 720.0f};

    glm::mat4 getPVMatrix() const {
        glm::mat4 pvMatrix = glm::ortho(0.0f, viewport.x, 0.0f, viewport.y, -0.1f, 1.0f);
        pvMatrix = pvMatrix * glm::inverse(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)), glm::vec3(zoom, zoom, 1.0f)));
        return pvMatrix;
    }

    glm::mat4 getTransformMatrix() const {
        glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)), glm::vec3(zoom, zoom, 1.0f));
        return transform;
    }
};
