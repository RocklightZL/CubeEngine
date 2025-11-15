#pragma once

#include <string>
#include <json.hpp>

namespace Cube {
    class Node2D;

    // Node base class: leaf-only, no child nodes allowed.
    class Node {
    public:
        Node() = default;
        Node(const std::string& name, Node2D* parent = nullptr) : name(name), parent(parent){}
        virtual ~Node() = default;

        virtual void update(float delta) = 0;

        virtual nlohmann::json serialize() const;
        virtual void deserialize(const nlohmann::json& data);
        virtual std::string getType() const { return "Node"; }

        std::string getName() const { return name; }
        Node2D* getParent() const { return parent; }

        void setParent(Node2D* p) { parent = p; }

        using CreateFunc = std::function<Node*()>;
        static void registerConstructor(const std::string& NodeType, const CreateFunc& func);
        static Node* createNode(const std::string& NodeType);

    protected:
        std::string name;
        Node2D* parent;

    private:
        static std::unordered_map<std::string, CreateFunc> constructorMap;
    };

}