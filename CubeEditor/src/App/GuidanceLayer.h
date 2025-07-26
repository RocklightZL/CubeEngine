#pragma once
#include "Cube/Core/Layer.h"

namespace Cube {

	class GuidanceLayer : public Layer {
	public:
		GuidanceLayer() = default;
		~GuidanceLayer() override = default;

		void onUpdate(float deltaTime) override;
	};

}