#include "pch.h"
#include "Camera2D.h"

namespace Cube {

    void Camera2D::update(float delta) { Node2D::update(delta); }

    nlohmann::json Camera2D::serialize() const {
        nlohmann::json data = Node2D::serialize();
        data["available"] = available;
        return data;
    }

    void Camera2D::deserialize(const nlohmann::json& data) {
        Node2D::deserialize(data);
        available = data["available"];
    }

}  // namespace Cube