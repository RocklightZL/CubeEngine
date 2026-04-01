#pragma once

#include <string>

class EditorApp;

namespace Cube {
	class Scene;
}

void gameThreadFunction(EditorApp* app, Cube::Scene* scene, const std::string& sceneDirectory, bool* isGameOver);