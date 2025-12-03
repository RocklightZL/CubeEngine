#include "pch.h"
#include "Entity.h"

#include "Cube/Reflection/Serializer.h"

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
            Any component = Serializer::get().deserialize(classInfo->getTypeID(), c);
            Component* compPtr = component.moveToBase<Component>();
            compPtr->entity = this;
            components[classInfo->getTypeID()] = std::unique_ptr<Component>(compPtr);
        }
    }

    nlohmann::json Entity::serialize() const {
        nlohmann::json data;
        data["name"] = name;
        nlohmann::json tr;
        tr["pos"] = {transform.getPosition().x, transform.getPosition().y};
        tr["rotation"] = transform.getRotation();
        tr["scale"] = {transform.getScale().x, transform.getScale().y};
        tr["children"] = nlohmann::json::array();
        for(auto& child : transform.getChildren()) {
            Entity* childEntity = child->getEntity();
            tr["children"].push_back(childEntity->getName());
        }
        data["transform"] = tr;
        data["components"] = nlohmann::json::array();
        for(const auto& [typeID, component] : components) {
            nlohmann::json c = Serializer::get().serialize(typeID, Any(component.get()));
            c["type"] = ClassRegistry::get().getClass(typeID)->getName();
            data["components"].push_back(c);
        }
        return data;
    }

}  // namespace Cube