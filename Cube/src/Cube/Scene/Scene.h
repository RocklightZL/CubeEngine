#pragma once

#include "Entity.h"
#include "System.h"

#include <vector>

namespace Cube {

    class SceneSerializer;

	// 协调所有游戏实体（Entity）、组件（Component）和系统（System）
	// 管理实体和系统的生命周期
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

                // 使用初始化列表展开参数包
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

        glm::vec2 viewportSize = {800, 600}; // TODO: 这个不应该被随便修改
        std::string name = "Scene";

        friend SceneSerializer;
    };

    

}  // namespace Cube