#pragma once

#include "Entity.h"
#include "System.h"

#include <vector>

namespace Cube {

	// 协调所有游戏实体（Entity）、组件（Component）和系统（System）
	// 管理实体和系统的生命周期
	class Scene {
    public:
        Scene() = default;
        virtual ~Scene();

        void onUpdate();

        Entity* createEntity();
        void addSystem(System* system);
        void markToDestroy(Entity* entity);

        template <typename... Components>
        std::vector<Entity*> getEntitiesWith() {
            std::vector<Entity*> res;
            for(auto e : entities) {
                if(e->hasComponent<Components>() && ...) {
                    res.push_back(e);
                }
            }
            return res;
        }

    private:
        void processDestruction();

        std::vector<Entity*> entities;
        std::vector<System*> systems;
    };

    

}  // namespace Cube