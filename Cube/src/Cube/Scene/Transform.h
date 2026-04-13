#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Component.h"

namespace Cube {

	class Transform : public Component {
	public:
		Transform() = default;
		explicit Transform(Entity* entity) : Component(entity) {}
		~Transform() override;
		TypeID getType() const override { return getTypeID<Transform>(); }

		const glm::mat4& getLocalMatrix();
        const glm::mat4& getWorldMatrix();

		void setPosition(const glm::vec2& position);
        const glm::vec2& getPosition() const;
        void setRotation(float degrees);
        float getRotation() const;
        void setScale(const glm::vec2& scale);
        const glm::vec2& getScale() const;

		glm::vec2 getWorldPos() const;
		glm::vec2 getWorldRotation() const;
		glm::vec2 getWorldScale() const;

		void addChild(Transform* child);
		void removeChild(Transform* child);
        const std::vector<Transform*>& getChildren() const;

	private:
		glm::vec2 pos = {0.0f, 0.0f};
		float rotation = 0.0f; // degrees
		glm::vec2 scale = {1.0f, 1.0f};
		glm::mat4 localMatrix = glm::mat4(0);
		glm::mat4 worldMatrix = glm::mat4(0);
        uint8_t dirty = 0b11;  // bit 0: localMatrix dirty, bit 1: worldMatrix dirty

		Transform* parent = nullptr;
		std::vector<Transform*> children;

		bool localDirty() const { return dirty & (uint8_t)0b01; }
        bool worldDirty() const { return dirty & (uint8_t)0b10; }
		void clearLocalDirty() { dirty &= ~(uint8_t)0b01; }
		void clearWorldDirty() { dirty &= ~(uint8_t)0b10; }
		void markLocalDirty() {
		    dirty |= (uint8_t)0b01;
			markWorldDirty();
		}
        void markWorldDirty() {
		    dirty |= (uint8_t)0b10;
			for(auto& child : children) {
				child->markWorldDirty();
            }
		}
    };
}
