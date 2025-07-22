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

        virtual void onUpdate(float deltaTime);

        Entity* createEntity();
        Entity* createEntity(const std::string& name);
        void addSystem(System* system);
        void markToDestroy(Entity* entity);

        template <typename... Components>
        std::vector<Entity*> getEntitiesWith() {
            std::vector<Entity*> res;

            for(auto e : entities) {
                bool hasAll = true;

                // ʹ�ó�ʼ���б�չ��������
                using expander = bool[];
                (void)expander{ (hasAll = hasAll && e->hasComponent<Components>(), false)... };

                if(hasAll) {
                    res.push_back(e);
                }
            }
            return res;
        }

        const std::vector<Entity*>& getEntities();

    private:
        void processDestruction();

        std::vector<Entity*> entities;
        std::vector<System*> systems;
    };

    

}  // namespace Cube