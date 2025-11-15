#include "pch.h"
#include "Node2D.h"

#include <glm/ext/matrix_transform.hpp>

namespace Cube {

    void Node2D::update(float delta) {
        for(auto& c : children) {
            c->update(delta);
        }
    }

    nlohmann::json Node2D::serialize() const {
        nlohmann::json data = Node::serialize();
        data["relPos"] = {relPos.x, relPos.y};
        data["pos"] = {pos.x, pos.y};
        data["rotation"] = rotation;
        data["scale"] = {scale.x, scale.y};
        for(auto& c : children) {
            data["children"].push_back(c->serialize());
        }
        return data;
    }

    void Node2D::deserialize(const nlohmann::json& data) {
        Node::deserialize(data);
        relPos = {data["relPos"][0], data["relPos"][1]};
        pos = {data["pos"][0], data["pos"][1]};
        rotation = data["rotation"];
        scale = {data["scale"][0], data["scale"][1]};
        for(auto& c : data["children"]) {
            Node* n = Node::createNode(c["type"]);
            n->deserialize(c);
            n->setParent(this);
            children.push_back(std::unique_ptr<Node>(n));
        }
    }

    void Node2D::addChild(Node* child) {
        child->setParent(this);
        children.push_back(std::unique_ptr<Node>(child));
    }

    void Node2D::removeChild(Node* child) {
        auto it = std::find_if(children.begin(), children.end(), [child](std::unique_ptr<Node> n) { return n.get() == child; });
        if(it == children.end()) {
            CB_CORE_ERROR("Node::eraseChild(Node* child): child not found");
            return;
        }
        children.erase(it);
    }

    void Node2D::setRelPos(const glm::vec2& relPos) {
        this->relPos = relPos;
        updatePos();
    }

    void Node2D::setRotation(float rotation) {
        this->rotation = rotation;
    }

    void Node2D::setScale(const glm::vec2& scale) {
        this->scale = scale;
    }

    glm::vec2 Node2D::getRelPos() const {
        return relPos;
    }
    glm::vec2 Node2D::getPos() const {
        return pos;
    }
    float Node2D::getRotation() const {
        return rotation;
    }
    glm::vec2 Node2D::getScale() const { return scale; }

    glm::mat4 Node2D::getTransformMatrix() const {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scale, 1.0f));
        return transform;
    }

    void Node2D::updatePos() {
        pos = parent->pos + relPos;
    }

}  // namespace Cube