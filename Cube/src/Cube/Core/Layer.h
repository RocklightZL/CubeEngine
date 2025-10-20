#pragma once

#include <intrin.h> // 去掉就过不了编译，不知道为什么

namespace Cube {
    class Layer {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void onAttach() {}  // 入层栈时调用
        virtual void onDetach() {}  // 出栈时调用
        virtual void onEvent() {}
        virtual void onUpdate(float deltaTime) {}
    };
}  // namespace Cube