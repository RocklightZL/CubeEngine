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
			.property("texture", &Sprite::texture)
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
				const ResPtr<Texture2D>& resPtr = obj.as<ResPtr<Texture2D>>();
				nlohmann::json j;
				if(resPtr) {
					j = resPtr->getPath();
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