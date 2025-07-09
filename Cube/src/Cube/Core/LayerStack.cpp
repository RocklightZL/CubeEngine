#include "pch.h"
#include "LayerStack.h"

namespace Cube {

    LayerStack::~LayerStack() {
        for(Layer* layer : layers) {
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        layers.push_back(layer);
        layer->onAttach();
    }

    void LayerStack::pushOverLayer(Layer* layer) {
        layers.push_front(layer);
        layer->onAttach();
    }

    void LayerStack::popLayer(Layer* layer) {
        auto it = std::find(layers.begin(), layers.end(), layer);
        if(it != layers.end()) {
            layers.erase(it);
            layer->onDetach();
            delete layer;
        }
    }
    const std::deque<Layer*>& LayerStack::getData() const {
        return layers;
    }

}  // namespace Cube