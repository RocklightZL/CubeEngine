#pragma once
#include "Node.h"
#include "Scene.h"

namespace Cube {

    class SceneNode : public Node {
    public:
        SceneNode() = default;
        ~SceneNode() override;

        void update(float delta) override;

        nlohmann::json serialize() const override;
        void deserialize(const nlohmann::json& data) override;
        std::string getType() const override { return "SceneNode"; }

    protected:
        Scene* scene;
    };

}