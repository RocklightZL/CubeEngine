#pragma once

namespace Cube {
    class Entity;

	class Component {
	public:
        friend class Entity;
		Component() = default;
		explicit Component(Entity* entity) : entity(entity) {}
		virtual ~Component() = default;

		// called when the first frame is ready for rendering
		virtual void start(){}
		virtual void update(float delta){}

		Entity* getEntity() const { return entity; }

	protected:
		Entity* entity;
	};
}