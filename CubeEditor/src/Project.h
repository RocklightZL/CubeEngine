#pragma once
#include "App/EditorLayer.h"
#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"
#include "Views/SceneSelectPanel.h"

#include <string>

namespace Cube {

    struct ProjectConfig {
		std::string name;
		std::string rootPath; // project root directory
		std::string projectDataDirectory;
		std::string resourcesDirectory;
		std::string sceneDirectory;
    };

    struct SceneData {
        Scene* scene = nullptr;
		bool isSaved = false;
    };

	class Project {
	public:
		friend SceneSelectPanel;
		friend EditorLayer;

        Project(const std::string& name, const std::string& rootPath);
		Project(const std::string& configFilePath);
		virtual ~Project();

		const std::vector<SceneData>& getScenes() const;
		void addScene(Scene* scene);
		bool hasScene(const std::string& sceneName) const;

		const ProjectConfig& getConfig() const;

		void save();
		// global data
		SceneData* selectedScene = nullptr;
		Entity* selectedEntity = nullptr;

	private:
		void writeToConfigFile(const std::string& configFilePath) const;
		void load();

		ProjectConfig config;
		std::vector<SceneData> scenes;
	};

}