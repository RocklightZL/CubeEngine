#pragma once

#include "Cube/Scene/System.h"

namespace Cube {

	class AnimatorSystem : public System {
	public:
		AnimatorSystem() = default;
        ~AnimatorSystem() override = default;

		void onUpdate(Scene* scene, float deltaTime) override;

		std::string getName() const override;
	};

}
