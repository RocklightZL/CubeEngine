#pragma once
#include "Cube/Core/Layer.h"

namespace Cube {
    class Texture2D;

    class GuidanceLayer : public Layer {
	public:
		GuidanceLayer();
		~GuidanceLayer() override;

		void onUpdate(float deltaTime) override;
	private:
		Texture2D* openProjectIcon;
		Texture2D* newProjectIcon;
	};

}