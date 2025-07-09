#pragma once
#include <deque>
#include "Layer.h"

namespace Cube {
    class LayerStack {
    public:
        LayerStack() = default;
        ~LayerStack();
        void pushLayer(Layer* layer);
        void pushOverLayer(Layer* layer);
        void popLayer(Layer* layer);

        const std::deque<Layer*>& getData() const;
    private:
        std::deque<Layer*> layers;
    };
}