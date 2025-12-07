#pragma once
#include "Entity.h"

namespace Cube {

    // GameObject-Component
    class Scene {
    public:
        Scene(const std::string& name, bool tagNoSceneFile) : name(name){}
        Scene(const std::string& sceneFilePath);
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

        void serialize(const std::string& sceneFilePath) const;

        const std::string& getName() const;

    private:
        std::string name;
        std::vector<std::unique_ptr<Entity>> entities;
        bool started = false;

        void start();
        void processDestroy();
    };

}