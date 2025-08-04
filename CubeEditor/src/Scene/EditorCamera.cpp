#include "EditorCamera.h"

#include <glm/ext/matrix_transform.hpp>

namespace Cube {

    glm::mat4 EditorCamera::getTransformMatrix() const {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scale, 1.0f));
        return transform;
    }
}  // namespace Cube