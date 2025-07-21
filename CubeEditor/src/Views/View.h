#pragma once

namespace Cube {

    class View {
    public:
        View() = default;
        virtual ~View() = default;

        virtual void render(float deltaTime) = 0;
    };

}