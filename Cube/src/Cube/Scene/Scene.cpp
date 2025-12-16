#include "pch.h"
#include "Scene.h"

#include <fstream>
#include <json.hpp>

namespace Cube {

    Scene::Scene(const std::string& sceneFilePath) {
        std::ifstream file(sceneFilePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("Scene::Scene(): Failed to open scene file '{}'", sceneFilePath);
            return;
        }
        nlohmann::json data;
        file >> data;
        file.close();
        name = data["name"];
        std::vector<std::pair<std::string, std::vector<std::string>>> transformTree;
        for(auto& entityData : data["entities"]) {
            auto entity = createEntity(entityData["name"]);
            entity->deserialize(entityData);
            transformTree.emplace_back(entity->getName(), entityData["transform"]["children"]);
        }
        for(auto& [entityName, childrenNames] : transformTree) {
            Entity* parentEntity = getEntity(entityName);
            Transform* parentTransform = &parentEntity->getTransform();
            for(auto& childName : childrenNames) {
                Entity* childEntity = getEntity(childName);
                Transform* childTransform = &childEntity->getTransform();
                parentTransform->addChild(childTransform);
            }
        }
    }

    void Scene::start() {
        for(auto& entity: entities) {
            entity->start();
        }
    }

    void Scene::update(float delta) {
        if(!started) {
            start();
            started = true;
        }
        for(auto& entity: entities) {
            entity->update(delta);
        }
        processDestroy();
    }

    Entity* Scene::createEntity(const std::string& name) {
        auto e = std::make_unique<Entity>(name);
        Entity* entity = e.get();
        entities.push_back(std::move(e));
        return entity;
    }

    void Scene::destroyEntity(const std::string& name) {
        auto it = std::find_if(entities.begin(), entities.end(), [&name](const std::unique_ptr<Entity>& entity) {
            return entity->getName() == name;
        });
        it->get()->destroy();
    }

    void Scene::destroyEntity(Entity* entity) {
        entity->destroy();
    }

    const std::vector<std::unique_ptr<Entity>>& Scene::getAllEntities() const {
        return entities;
    }

    Entity* Scene::getEntity(const std::string& name) const {
        auto it = std::find_if(entities.begin(), entities.end(), [&name](const std::unique_ptr<Entity>& entity) { return entity->getName() == name; });
        return it->get();
    }

    void Scene::serialize(const std::string& sceneFilePath) const {
        nlohmann::json data;
        data["name"] = name;
        data["entities"] = nlohmann::json::array();
        for(const auto& entity : entities) {
            data["entities"].push_back(entity->serialize());
        }
        std::ofstream file(sceneFilePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("Scene::serialize(): Failed to open scene file '{}'", sceneFilePath);
            return;
        }
        file << data.dump(4);
        file.close();
    }

    const std::string& Scene::getName() const {
        return name;
    }

    void Scene::processDestroy() {
        auto end = std::remove_if(entities.begin(), entities.end(), [this](const std::unique_ptr<Entity>& entity) {
            return !entity->isAlive();
        });
        entities.erase(end, entities.end());
    }
}  // namespace Cube