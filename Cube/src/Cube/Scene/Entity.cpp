#include "pch.h"
#include "Entity.h"

namespace Cube {

    void Entity::start() {
        for(auto& [typeID, component] : components) {
            component->start();
        }
    }

    void Entity::update(float delta) {
        for(auto& [typeID, component] : components) {
            component->update(delta);
        }
    }

    const std::string& Entity::getName() const {
        return name;
    }
    void Entity::setName(const std::string& name) {
        this->name = name;
    }
    Transform& Entity::getTransform() {
        return transform;
    }

    void Entity::deserialize(const nlohmann::json& data) {
        name = data["name"];
        auto tr = data["transform"];
        transform.setPosition({tr["pos"][0], tr["pos"][1]});
        transform.setRotation(tr["rotation"]);
        transform.setScale({tr["scale"][0], tr["scale"][1]});
        for(auto& c : data["components"]) {
            std::string typeName = c["type"];
            Class* classInfo = ClassRegistry::get().getClass(typeName);
            if(!classInfo) {
                CB_CORE_ERROR("Entity::deserialize(): Unknown component type '{}'", typeName);
                continue;
            }
            void* component = classInfo->createInstance();
            if(!component) {
                CB_CORE_ERROR("Entity::deserialize(): Failed to create component of type '{}'", typeName);
                continue;
            }
            for(auto& property : classInfo->getAllProperties()) {
                property->setValue(component, Any(c[property->getName()]));
            }
            components[classInfo->getTypeID()] = std::unique_ptr<Component>(component);
        }])
    }

    nlohmann::json Entity::serialize() const {
    }

}  // namespace Cube