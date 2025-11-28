#pragma once
#include "Entity.h"

namespace Cube {
    class System;

    class Scene {
    public:
        Scene(const std::string& name) : name(name){}
        virtual ~Scene() = default;

        void update(float delta);

        Entity* createEntity(const std::string& name);
        void destroyEntity(const std::string& name);

        const std::vector<std::unique_ptr<Entity>>& getAllEntities() const;
        Entity* getEntity(const std::string& name) const;

        template<typename... Types>
        std::vector<Entity*> getEntitiesWith() const {
            static_assert((std::is_base_of_v<Component, Types> && ...));
            std::vector<Entity*> result;
            for(const auto& entity : entities) {
                bool hasAll = true;
                ((hasAll = hasAll && entity->hasComponent<Types>()), ...);
                if(hasAll) {
                    result.push_back(entity.get());
                }
            }
            return result;
        }

    private:
        std::string name;
        std::vector<std::unique_ptr<Entity>> entities;
        std::vector<std::unique_ptr<System>> systems;

        void processDestroy();
    };

}