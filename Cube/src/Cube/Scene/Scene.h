#pragma once

#include "Entity.h"
#include "System.h"

#include <vector>

namespace Cube {

    class SceneSerializer;

	// Э��������Ϸʵ�壨Entity���������Component����ϵͳ��System��
	// ����ʵ���ϵͳ����������
	class Scene {
    public:
        Scene() = default;
        Scene(const glm::vec2& viewportSize, const std::string& name);
        virtual ~Scene();

        virtual void onUpdate(float deltaTime);

        Entity* createEntity();
        Entity* createEntity(const std::string& name);
        void addSystem(System* system);
        void markToDestroy(Entity* entity);

        void clearAll();

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

        const std::vector<Entity*>& getEntities() const;
        const std::vector<System*>& getSystems() const;

        const glm::vec2& getViewportSize() const;
        void setViewportSize(const glm::vec2& size);

        const std::string& getName() const;
        void setName(const std::string& name);

#ifdef EDITOR
        bool hasSystem(const std::string& systemName) const;
#endif

    private:
        void processDestruction();

        std::vector<Entity*> entities;
        std::vector<System*> systems;

        glm::vec2 viewportSize = {800, 600}; // TODO: �����Ӧ�ñ�����޸�
        std::string name = "Scene";

        friend SceneSerializer;
    };

    

}  // namespace Cube