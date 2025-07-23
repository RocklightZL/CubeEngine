#include "pch.h"
#include "SceneSerializer.h"

#include <fstream>

namespace Cube {

    void SceneSerializer::serialize(Scene* scene, const std::string& filePath) {
        // to json
        nlohmann::json sceneData;

        // sceneData["name"] = "untitled"; // TODO: get scene name
        // sceneData["version"] = "1.0"; // TODO: get version
        // sceneData["date"] = "";
        sceneData["entities"] = nlohmann::json::array();
        for(Entity* entity : scene->entities) {
            sceneData["entities"].push_back(serializeEntity(entity));
        }

        std::ofstream file(filePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("SceneSerializer::serialize: failed to open {}", filePath);
            return;
        }
        file << sceneData.dump(4);
    }

    nlohmann::json SceneSerializer::serializeEntity(Entity* entity) {
        nlohmann::json entityData;
        entityData["name"] = entity->getName();
        entityData["ID"] = entity->getID();
        entityData["components"] = nlohmann::json::array();
        for(const auto& c : entity->getAllComponents()) {
            entityData["components"].push_back(serializers[c.first](entity));
        }
        return entityData;
    }

    nlohmann::json SceneSerializer::transformComponentSerializer(Entity* entity) {
        nlohmann::json componentData;
        TransformComponent* tc = entity->getComponent<TransformComponent>();
        componentData["position"] = nlohmann::json::array({tc->position.x, tc->position.y});
        componentData["scale"] = nlohmann::json::array({tc->scale.x, tc->scale.y});
        componentData["rotation"] = tc->rotation;
        return componentData;
    }

    nlohmann::json SceneSerializer::spriteComponentSerializer(Entity* entity) {
        nlohmann::json componentData;
        SpriteComponent* sc = entity->getComponent<SpriteComponent>();
        componentData["name"] = sc->name;
        componentData["alas"] = sc->alas->getFilePath();
        componentData["region"] = nlohmann::json::array({sc->region.uvMax.x, sc->region.uvMax.y, sc->region.uvMin.x, sc->region.uvMin.y});
        componentData["color"] = nlohmann::json::array({sc->color.r, sc->color.g, sc->color.b, sc->color.a});
        return componentData;
    }

    nlohmann::json SceneSerializer::cameraComponentSerializer(Entity* entity) {
        nlohmann::json componentData;
        CameraComponent* cc = entity->getComponent<CameraComponent>();
        componentData["available"] = cc->available;
        return componentData;
    }

    nlohmann::json SceneSerializer::animatorComponentSerializer(Entity* entity) {
        nlohmann::json componentData;
        // TODO
        return componentData;
    }

}  // namespace Cube