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
        for(auto& entityData : data["entities"]) {
            auto entity = createEntity(entityData["name"]);
            entity->deserialize(entityData);
        }
    }

    void Scene::start() {
        for(auto& entity: entities) {
            entity->start();
        }
    }

    void Scene::update(float delta) {
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

    const std::vector<std::unique_ptr<Entity>>& Scene::getAllEntities() const {
        return entities;
    }

    Entity* Scene::getEntity(const std::string& name) const {
        auto it = std::find_if(entities.begin(), entities.end(), [&name](const std::unique_ptr<Entity>& entity) { return entity->getName() == name; });
        return it->get();
    }

    void Scene::processDestroy() {
        auto end = std::remove_if(entities.begin(), entities.end(), [this](const std::unique_ptr<Entity>& entity) {
            return !entity->isAlive();
        });
        entities.erase(end, entities.end());
    }
}  // namespace Cube