#pragma once
#include "Cube/Scene/Entity.h"
#include "View.h"

class EntityPropertyPanel : public View{
public:
    EntityPropertyPanel(EditorPage& editorPage) : View(editorPage) {}
	~EntityPropertyPanel() override = default;

	void render(float deltaTime) override;

private:
	Cube::Entity* preEntity = nullptr;
	std::vector<std::pair<Cube::TypeID, Cube::Component*>> componentsCache;

	void updateCache();
};