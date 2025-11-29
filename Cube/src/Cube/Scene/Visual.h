#pragma once
#include "Component.h"

namespace Cube {

	// base class for visual components, which can be rendered by the RenderServer
	class Visual : public Component {
	public:
		int order = 0;
	};

}