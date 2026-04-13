#include "pch.h"
#include "Entity.h"

#include "Cube/Core/Log.h"

#include "Cube/Reflection/Serializer.h"

namespace Cube {

    void Entity::update(float delta) {
        for(auto& component : components) {
            component->update(delta);
        }
        processAddAndDestroy();
        processStart();
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
            components.push_back(std::unique_ptr<Component>(compPtr));
            componentsMap[classInfo->getTypeID()] = compPtr;
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
        for(const auto& [typeID, component] : componentsMap) {
            nlohmann::json c = Serializer::get().serialize(typeID, Any(component));
            c["type"] = ClassRegistry::get().getClass(typeID)->getName();
            data["components"].push_back(c);
        }
        return data;
    }

    void Entity::processAddAndDestroy() {
        int addIndex = 0;
        int destroyIndex = 0;
        for(auto& action : addOrDestroy) {
            if(action == 0) { // add
                auto& component = pendingAdd[addIndex++];
                Component* ptr = component.get();
                componentsMap[ptr->getType()] = ptr;
                components.push_back(std::move(component));
                pendingStart.push_back(ptr);
            } else { // destroy
                TypeID typeID = pendingDestroy[destroyIndex++];
                auto it = componentsMap.find(typeID);
                Component* compPtr = it->second;
                componentsMap.erase(it);
                components.erase(std::remove_if(components.begin(), components.end(), [compPtr](const std::unique_ptr<Component>& c) {
                    return c.get() == compPtr;
                }), components.end());
                pendingStart.erase(std::remove(pendingStart.begin(), pendingStart.end(), compPtr), pendingStart.end());
            }
        }
        CB_ASSERT(addIndex == pendingAdd.size() && destroyIndex == pendingDestroy.size() && "Entity::processAddAndDestroy(): add and destroy count mismatch");
        pendingAdd.clear();
        pendingDestroy.clear();
        addOrDestroy.clear();
    }

    void Entity::processStart() {
        for(Component* c : pendingStart) {
            c->start();
        }
        pendingStart.clear();
    }

}  // namespace Cube