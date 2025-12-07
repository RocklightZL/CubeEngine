#pragma once
#include <memory>
#include <vector>

#include "../Views/View.h"
#include "Page.h"

class EditorPage : public Page {
public:
    EditorPage();
    ~EditorPage() override = default;

    void render(float deltaTime) override;

private:
    std::vector<std::unique_ptr<View>> views;
};