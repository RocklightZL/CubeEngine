#include "pch.h"
#include "Scene.h"

namespace Cube {

    Scene::Scene(const glm::vec2& viewportSize, const std::string& name) : viewportSize(viewportSize), name(name){}

    Scene::~Scene() {
        clearAll();
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

    void Scene::clearAll() {
        for(auto e : entities) {
            delete e;
        }
        for(auto s : systems) {
            delete s;
        }
        entities.clear();
        systems.clear();
    }

    const std::vector<Entity*>& Scene::getEntities() const{ return entities; }

    const std::vector<System*>& Scene::getSystems() const { return systems; }

    const glm::vec2& Scene::getViewportSize() const { return viewportSize; }

    void Scene::setViewportSize(const glm::vec2& size) { viewportSize = size; }

    const std::string& Scene::getName() const { return name; }

    void Scene::processDestruction() {
        auto it = std::remove_if(entities.begin(), entities.end(),
            [](Entity* entity) { return !entity->isAlive(); });
        for(auto iter = it; iter != entities.end(); ++iter) {
            delete *iter;
        }
        entities.erase(it, entities.end());
    }
}  // namespace Cube