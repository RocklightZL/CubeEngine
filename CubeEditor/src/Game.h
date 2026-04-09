#pragma once

#include <string>

class EditorApp;
class Project;

namespace Cube {
	class Scene;
}

void gameThreadFunction(EditorApp* app, Project* project, Cube::Scene* scene, bool* isGameOver);