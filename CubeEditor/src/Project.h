#pragma once
#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"

#include <string>

namespace Cube {

    struct ProjectConfig {
		std::string name;
		std::string rootPath; // project root directory
		std::string projectDataDirectory;
		std::string resourcesDirectory;
		std::string sceneDirectory;
    };

	class Project {
	public:
		Project(const std::string& name, const std::string& rootPath);
		Project(const std::string& configFilePath);
		virtual ~Project();

		const std::vector<Scene*>& getScenes() const;
		void addScene(Scene* scene);

		// global data
		Scene* selectedScene = nullptr;
		Entity* selectedEntity = nullptr;

	private:
		void writeToConfigFile(const std::string& configFilePath) const;

		ProjectConfig config;
		std::vector<Scene*> scenes;
	};

}