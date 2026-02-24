#include "pch.h"
#include "Context.h"

#include "Renderer.h"

namespace Cube {

    Context::Context() {}

    Context::~Context() {
        delete defaultShader;
        delete whiteTex;
    }

}  // namespace Cube