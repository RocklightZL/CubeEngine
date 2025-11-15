#pragma once
#include "Node.h"

#include <glm/glm.hpp>

namespace Cube {

    // visual node; have child nodes.
    class Node2D : public Node{
    public:
        Node2D() = default;
        ~Node2D() override = default;
        
        void update(float delta) override;

        nlohmann::json serialize() const override;
        void deserialize(const nlohmann::json& data) override;
        std::string getType() const override{ return "Node2D"; }

        void addChild(Node* child);
        void removeChild(Node* child);
        void setRelPos(const glm::vec2& relPos);
        void setRotation(float rotation);
        void setScale(const glm::vec2& scale);
        glm::vec2 getRelPos() const;
        glm::vec2 getPos() const;
        float getRotation() const;
        glm::vec2 getScale() const;
        glm::mat4 getTransformMatrix() const;

    protected:
        std::vector<std::unique_ptr<Node>> children;

        glm::vec2 relPos = {0.0f, 0.0f}; // position relative to parent node
        glm::vec2 pos = {0.0f, 0.0f};
        float rotation = 0.0f;
        glm::vec2 scale = {1.0f, 1.0f};

        void updatePos();
    };

}