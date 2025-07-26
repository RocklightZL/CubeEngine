#pragma once
#include "View.h"

namespace Cube {

	class ResourcesPanel : public View {
	public:
		ResourcesPanel() = default;
		~ResourcesPanel() override = default;

		void render(float deltaTime) override;
	};

}