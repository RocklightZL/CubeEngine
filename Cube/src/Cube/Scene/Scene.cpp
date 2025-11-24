#include "pch.h"
#include "Scene.h"

namespace Cube {

    void Scene::update(float delta) {
        for(auto& entity : entities) {
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
        pendingDestroy.push_back(it->get());
    }

    const std::vector<std::unique_ptr<Entity>>& Scene::getAllEntities() const {
        return entities;
    }

    Entity* Scene::getEntity(const std::string& name) const {
        auto it = std::find_if(entities.begin(), entities.end(), [&name](const std::unique_ptr<Entity>& entity) {
            return entity->getName() == name; 
        });
        return it->get();
    }

    void Scene::processDestroy() {
        auto end = std::remove_if(entities.begin(), entities.end(), [this](const std::unique_ptr<Entity>& entity) {
            return std::find(pendingDestroy.begin(), pendingDestroy.end(), entity.get()) != pendingDestroy.end();
        });
        entities.erase(end, entities.end());
        pendingDestroy.clear();
    }
}  // namespace Cube