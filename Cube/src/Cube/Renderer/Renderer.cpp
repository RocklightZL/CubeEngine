#include "pch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace Cube {

	void Renderer::setViewport(int width, int heght) {
	    glViewport(0, 0, width, heght);
	}

}