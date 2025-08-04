#pragma once
#include <glm/glm.hpp>

namespace Cube {

    class EditorCamera {
    public:
        glm::vec2 position = {0, 0};
        glm::vec2 scale = {1, 1};
        float rotation = 0.0f;
        glm::mat4 getTransformMatrix() const;
    };

}