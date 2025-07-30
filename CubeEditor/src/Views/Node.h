#pragma once

#include <json.hpp>
#include <string>
#include <vector>

namespace Cube {
    struct Node {
        std::string name = "Node";
        bool isGroup = false;
        bool isRenaming = false;
        bool justRenaming = false;
        std::vector<std::shared_ptr<Node>> children;
        std::weak_ptr<Node> parent;

        nlohmann::json toJson() const {
            nlohmann::json json;
            json["name"] = name;
            json["isGroup"] = isGroup;
            json["children"] = nlohmann::json::array();
            for(auto& c : children) {
                json["children"].push_back(c->toJson());
            }
            return json;
        }

        void fromJson(const nlohmann::json& json, const std::shared_ptr<Node>& p) {
            name = json["name"];
            isGroup = json["isGroup"];
            for(auto& c : json["children"]) {
                auto n = std::make_shared<Node>();
                n->fromJson(c, n);
                children.push_back(n);
                n->parent = p;
            }
        }
    };
}