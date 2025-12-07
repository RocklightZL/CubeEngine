#pragma once

class View {
public:
    View() = default;
    virtual ~View() = default;

    virtual void render(float deltaTime) = 0;
};