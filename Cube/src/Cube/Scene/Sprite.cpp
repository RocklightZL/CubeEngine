#include "pch.h"
#include "Sprite.h"

#include "Cube/Renderer/Renderer.h"

namespace Cube {

    Sprite::~Sprite() {
        if(texture) {
            ResourceManager::get().release(texture);
        }
    }

    void Sprite::update(float delta) {
		Renderer2D::drawQuad(getTransformMatrix(), tintColor, texture, texRegion.getUVCoord());
	    Node2D::update(delta);
	}

	nlohmann::json Sprite::serialize() const {
		nlohmann::json data = Node2D::serialize();
        data["texture"] = texture ? texture->getPath() : "";
		data["texRegion"] = {texRegion.uvMin.x, texRegion.uvMin.y, texRegion.uvMax.x, texRegion.uvMax.y};
        data["tintColor"] = {tintColor.r, tintColor.g, tintColor.b, tintColor.a};
		return data;
	}

	void Sprite::deserialize(const nlohmann::json& data) {
		std::string texPath = data["texture"];
		if(!texPath.empty()) {
			texture = ResourceManager::get().load<Texture2D>(texPath);
        }else {
            texture = nullptr;
        }
        texRegion.uvMin = {data["texRegion"][0], data["texRegion"][1]};
        texRegion.uvMax = {data["texRegion"][2], data["texRegion"][3]};
        tintColor = {(float)data["tintColor"][0], (float)data["tintColor"][1], (float)data["tintColor"][2], (float)data["tintColor"][3]};
		Node2D::deserialize(data);
	}
}  // namespace Cube