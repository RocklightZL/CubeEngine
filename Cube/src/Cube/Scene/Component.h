#pragma once

namespace Cube {
    class Entity;

	class Component {
	public:
        friend class Entity;
		Component() = default;
		explicit Component(Entity* entity) : entity(entity) {}
		virtual ~Component() = default;

		Entity* getEntity() const { return entity; }

	protected:
		Entity* entity;
	};
}