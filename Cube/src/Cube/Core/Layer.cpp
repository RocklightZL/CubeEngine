#include "pch.h"
#include "Layer.h"

namespace Cube {

     Layer::Layer(const std::string& name) : name(name) {}

    std::string Layer::getName() const { return name; }

}  // namespace Cube