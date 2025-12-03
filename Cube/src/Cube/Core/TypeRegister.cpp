#include "pch.h"
#include "TypeRegister.h"

#include "Cube/Scene/Component.h"
#include "Cube/Scene/Camera2D.h"
#include "Cube/Scene/Sprite.h"
#include "Cube/Reflection/ClassBuilder.h"

#include <glm/glm.hpp>

namespace Cube {

	void registerTypes() {
		// glm
		ClassBuilder<glm::vec1>("vec1")
			.property("x", &glm::vec1::x)
	        .serializer();
		ClassBuilder<glm::vec2>("vec2")
			.property("x", &glm::vec2::x)
			.property("y", &glm::vec2::y)
	        .serializer();
		ClassBuilder<glm::vec3>("vec3")
			.property("x", &glm::vec3::x)
			.property("y", &glm::vec3::y)
			.property("z", &glm::vec3::z)
	        .serializer();
		ClassBuilder<glm::vec4>("vec4")
			.property("x", &glm::vec4::x)
			.property("y", &glm::vec4::y)
			.property("z", &glm::vec4::z)
			.property("w", &glm::vec4::w)
			.serializer();

		// TextureRegion
		ClassBuilder<TextureRegion>("TextureRegion")
			.property("uvMin", &TextureRegion::uvMin)
			.property("uvMax", &TextureRegion::uvMax)
	        .serializer();

		// Color
		ClassBuilder<Color>("Color")
			.property("r", &Color::r)
			.property("g", &Color::g)
			.property("b", &Color::b)
			.property("a", &Color::a)
            .serializer();

		// Component
	    ClassBuilder<Component>("Component");
		ClassBuilder<Sprite>("Sprite")
			.base<Component>()
			.property("texture", &Sprite::texture, [](const void* obj) {
                const Sprite* sprite = static_cast<const Sprite*>(obj);
			    return Any(sprite->texture.get());
			}, nullptr, [](void* obj, Any&& value) {
				Sprite* sprite = static_cast<Sprite*>(obj);
				sprite->texture = std::move(*value.move<ResPtr<Texture2D>>());
			})
			.property("texRegion", &Sprite::texRegion)
			.property("tintColor", &Sprite::tintColor)
			.property("order", &Sprite::order)
            .serializer();
		ClassBuilder<Camera2D>("Camera2D")
			.base<Component>()
			.property("viewport", &Camera2D::viewport)
			.property("zoom", &Camera2D::zoom)
		    .property("available", &Camera2D::available)
	        .serializer();

		// register serializer
		registerBasicSerializers();
        Serializer& serializer = Serializer::get();
		serializer.registerConverter(getTypeID<ResPtr<Texture2D>>(), {
			// toJson
			[](const Any& obj) {
				const Texture2D* resPtr = &obj.as<Texture2D>(); // TODO: 这个Any类可能还要优化一下
				nlohmann::json j;
				if(resPtr) {
					j = nlohmann::json(resPtr->getPath());
				}else {
					j = nullptr;
				}
				return j;
			},
			// fromJson
			[](const nlohmann::json& data) {
				if(data.is_null()) {
					return Any(ResPtr<Texture2D>(nullptr));
				}else {
					std::string path = data.get<std::string>();
					return Any(ResPtr<Texture2D>(path));
				}
            }
		});
	}
}