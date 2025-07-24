#include "pch.h"
#include "SceneSerializer.h"

#include "Cube/Resource/ResourceManager.h"

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

    void SceneSerializer::deserialize(Scene* scene, const std::string& filePath) {
        std::ifstream file(filePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("SceneSerializer::deserialize: failed to open {}", filePath);
            return;
        }
        nlohmann::json sceneData;
        file >> sceneData;
        scene->clearAll();
        for(auto& entityData : sceneData["entities"]) {
            deserializeEntity(scene, entityData);
        }
    }

    nlohmann::json SceneSerializer::serializeEntity(Entity* entity) {
        nlohmann::json entityData;
        entityData["name"] = entity->getName();
        entityData["ID"] = entity->getID();
        entityData["components"] = nlohmann::json::object();
        for(const auto& c : entity->getAllComponents()) {
            entityData["components"][std::to_string(c.first)] = serializers[c.first](entity);
        }
        return entityData;
    }

    void SceneSerializer::deserializeEntity(Scene* scene, const nlohmann::json& entityData) {
        Entity* entity = scene->createEntity(entityData["name"]);
        for(auto& [typeStr, componentData] : entityData["components"].items()) {
            deserializers[std::stoul(typeStr)](entity, componentData);
        }
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
        if(sc->alas){
            componentData["alas"] = nlohmann::json::array({sc->alas->getFilePath(), sc->alas->getMetaDataPath()});
        }
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

    void SceneSerializer::transformComponentDeserializer(Entity* entity, const nlohmann::json& componentData) {
        TransformComponent* tc = entity->addComponent<TransformComponent>();
        tc->position = {componentData["position"][0], componentData["position"][1]};
        tc->scale = {componentData["scale"][0], componentData["scale"][1]};
        tc->rotation = componentData["rotation"];
    }

    void SceneSerializer::spriteComponentDeserializer(Entity* entity, const nlohmann::json& componentData) {
        SpriteComponent* sc = entity->addComponent<SpriteComponent>();
        sc->name = componentData["name"];
        if(componentData.find("alas") != componentData.end()) {
            sc->alas = ResourceManager::getInstance().load<TextureAlas>(componentData["alas"][0], componentData["alas"][1])->data;
        }
        sc->region = {{componentData["region"][0], componentData["region"][1]}, {componentData["region"][2], componentData["region"][3]}};
        sc->color = {componentData["color"][0], componentData["color"][1], componentData["color"][2], componentData["color"][3]};
    }

    void SceneSerializer::cameraComponentDeserializer(Entity* entity, const nlohmann::json& componentData) {
        CameraComponent* cc = entity->addComponent<CameraComponent>();
        cc->available = componentData["available"];
    }

    void SceneSerializer::animatorComponentDeserializer(Entity* entity, const nlohmann::json& componentData) {
        AnimatorComponent* ac = entity->addComponent<AnimatorComponent>();
    }

    std::unordered_map<ComponentID, std::function<nlohmann::json(Entity*)>> SceneSerializer::serializers = {
        {Component::getTypeID<TransformComponent>(), transformComponentSerializer},
        {Component::getTypeID<SpriteComponent>(), spriteComponentSerializer},
        {Component::getTypeID<CameraComponent>(), cameraComponentSerializer},
        {Component::getTypeID<AnimatorComponent>(), animatorComponentSerializer},
    };

    std::unordered_map<ComponentID, std::function<void(Entity*, const nlohmann::json&)>> SceneSerializer::deserializers = {
        {Component::getTypeID<TransformComponent>(), transformComponentDeserializer},
        {Component::getTypeID<SpriteComponent>(), spriteComponentDeserializer},
        {Component::getTypeID<CameraComponent>(), cameraComponentDeserializer},
        {Component::getTypeID<AnimatorComponent>(), animatorComponentDeserializer},
    };
}  // namespace Cube