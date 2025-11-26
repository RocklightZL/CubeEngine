#pragma once

namespace Cube {

	class System {
	public:
		System() = default;
		virtual ~System() = default;
        virtual void update(float deltaTime) = 0;
	};

}