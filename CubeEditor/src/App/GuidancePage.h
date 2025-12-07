#pragma once
#include "Page.h"

class GuidancePage : public Page {
public:
	GuidancePage() = default;
	~GuidancePage() override = default;

	void render(float deltaTime) override;
};