#pragma once

#include "Cube/Reflection/Type.h"

namespace Cube {
    class Entity;

	// Game runtime data is declared private or protected;
    // Data requiring persistent storage should be declared public for reflection serialization.
	class Component {
	public:
        friend class Entity;
		Component() = default;
		explicit Component(Entity* entity) : entity(entity) {}
		virtual ~Component() = default;

		virtual void start(){}
		virtual void update(float delta){}
		virtual TypeID getType() const = 0;

		Entity* getEntity() const { return entity; }

	protected:
		Entity* entity;
	};
}