#pragma once

#include "Scene.h"

#include <string>
#include <unordered_map>
#include <json.hpp>

namespace Cube {

	// static class
	class SceneSerializer {
	public:
		SceneSerializer() = delete;
		~SceneSerializer() = delete;

		static void serialize(Scene* scene, const std::string& filePath);
		static void deserialize(Scene* scene, const std::string& filePath);

		template<typename T>
		static void registerComponentSerializer(const std::function<void(Entity*, const nlohmann::json&)>& serializer) {
		    serializers[Component::getTypeID<T>()] = serializer;
		}

		template<typename T>
		static void registerComponentDeserializer(const std::function<void(Entity*, const nlohmann::json&)>& deserializer) {
			deserializers[Component::getTypeID<T>()] = deserializer;
		}
	private:

		static nlohmann::json serializeEntity(Entity* entity);
		static void deserializeEntity(Scene* scene, const nlohmann::json& entityData);

		static std::unordered_map<ComponentID, std::function<nlohmann::json(Entity*)>> serializers;
		static std::unordered_map<ComponentID, std::function<void(Entity*, const nlohmann::json&)>> deserializers;

		// built-in component serializer
		static nlohmann::json transformComponentSerializer(Entity* entity);
		static nlohmann::json spriteComponentSerializer(Entity* entity);
		static nlohmann::json cameraComponentSerializer(Entity* entity);
		static nlohmann::json animatorComponentSerializer(Entity* entity);
	};

}