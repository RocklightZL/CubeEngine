#include "pch.h"
#include "Scene.h"

namespace Cube {

    Scene::~Scene() {
        for(auto e : entities) {
            delete e;
        }
        for(auto s : systems) {
            delete s;
        }
    }

    void Scene::onUpdate(float deltaTime) {
        for(auto& system : systems) {
            system->onUpdate(this, deltaTime);
        }
        processDestruction();
    }

    Entity* Scene::createEntity() {
        Entity* entity = new Entity();
        entities.push_back(entity);
        return entity;
    }

    Entity* Scene::createEntity(const std::string& name) {
        Entity* entity = new Entity(name);
        entities.push_back(entity);
        return entity;
    }

    void Scene::addSystem(System* system) {
        system->onAttach();
        systems.push_back(system);
    }

    void Scene::markToDestroy(Entity* entity) {
        if(entity) {
            entity->destroy();
        }
    }

    const std::vector<Entity*>& Scene::getEntities() {
        return entities;
    }

    void Scene::processDestruction() {
        auto it = std::remove_if(entities.begin(), entities.end(),
            [](Entity* entity) { return !entity->isAlive(); });
        for(auto iter = it; iter != entities.end(); ++iter) {
            delete *iter;
        }
        entities.erase(it, entities.end());
    }
}  // namespace Cube