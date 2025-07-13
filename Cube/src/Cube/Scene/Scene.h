#pragma once

#include "Entity.h"
#include "System.h"

#include <vector>

namespace Cube {

	// Э��������Ϸʵ�壨Entity���������Component����ϵͳ��System��
	// ����ʵ���ϵͳ����������
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