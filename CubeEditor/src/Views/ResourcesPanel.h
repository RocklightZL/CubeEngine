#pragma once
#include "View.h"

namespace Cube {
    struct Node;

    class ResourcesPanel : public View {
	public:
		ResourcesPanel() = default;
		~ResourcesPanel() override = default;

		void render(float deltaTime) override;

        static void importResources();
	};

}