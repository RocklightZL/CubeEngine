#pragma once
#include "Any.h"
#include "ClassRegistry.h"
#include "Type.h"

#include <json.hpp>

namespace Cube {

    struct Converter {
        std::function<nlohmann::json(const Any& obj)> toJson;
        std::function<Any(const nlohmann::json&)> fromJson;
    };

    // depend on nlohmann::json
    class Serializer {
    public:
        static Serializer& get() {
            static Serializer instance;
            return instance;
        }

        void registerConverter(TypeID typeID, const Converter& converter) {
            registry[typeID] = converter;
        }

        nlohmann::json serialize(TypeID typeID, const Any& obj) {
            auto it = registry.find(typeID);
            if(it == registry.end()) {
                CB_CORE_ERROR("Serializer::serialize(): No converter registered for type ID {}", typeID);
                return nullptr;
            }
            return it->second.toJson(obj);
        }

        Any deserialize(TypeID typeID, const nlohmann::json& data) {
            auto it = registry.find(typeID);
            if(it == registry.end()) {
                CB_CORE_ERROR("Serializer::deserialize(): No converter registered for type ID {}", typeID);
                return nullptr;
            }
            return it->second.fromJson(data);
        }

    private:
        std::unordered_map<TypeID, Converter> registry;

        Serializer() = default;
        ~Serializer() = default;
    };

    template<typename T>
    void registerSerializer() {
        Converter converter;
        if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, std::string>) {
            converter.toJson = [](const Any& obj) {
                return nlohmann::json(obj.as<T>());
            };
            converter.fromJson = [](const nlohmann::json& data) {
                return Any(data.get<T>());
            };
        }else {
            static_assert(std::is_class_v<T>);
            converter.toJson = [](const Any& obj) {
                nlohmann::json j;
                Class* classInfo = ClassRegistry::get().getClass<T>();
                if(!classInfo) {
                    CB_CORE_ERROR("Serializer::toJson(): Class info not found for type ID {}", getTypeID<T>());
                    return j;
                }
                for(auto& property : classInfo->getAllProperties()) {
                    j[property->getName()] = Serializer::get().serialize(property->getTypeID(), property->getValue(obj.getData()));
                }
                return j;
            };
            converter.fromJson = [](const nlohmann::json& data) {
                Class* classInfo = ClassRegistry::get().getClass<T>();
                if(!classInfo) {
                    CB_CORE_ERROR("Serializer::fromJson(): Class info not found for type ID {}", getTypeID<T>());
                    return Any();
                }
                Any instance = classInfo->createInstance();
                for(auto& property : classInfo->getAllProperties()) {
                    nlohmann::json propData = data[property->getName()];
                    Any propValue = Serializer::get().deserialize(property->getTypeID(), propData);
                    property->setValue(instance.getData(), propValue);
                }
                return instance;
            };
        }
        Serializer::get().registerConverter(getTypeID<T>(), converter);
    }
}