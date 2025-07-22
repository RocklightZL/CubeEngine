#pragma once
#include "View.h"

namespace Cube {

	class EntityPropertyPanel : public View{
	public:
	    EntityPropertyPanel() = default;
		~EntityPropertyPanel() override = default;

		void render(float deltaTime) override;
	};

}