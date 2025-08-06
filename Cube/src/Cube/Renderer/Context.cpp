#include "pch.h"
#include "Context.h"

#include "Renderer.h"

namespace Cube {

    Context::~Context() {
        delete whiteTex;
    }
}  // namespace Cube