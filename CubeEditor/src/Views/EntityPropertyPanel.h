#pragma once
#include "Cube/Scene/Entity.h"
#include "View.h"

class EntityPropertyPanel : public View{
public:
    EntityPropertyPanel(EditorPage& editorPage) : View(editorPage) {}
	~EntityPropertyPanel() override = default;

	void render(float deltaTime) override;
};