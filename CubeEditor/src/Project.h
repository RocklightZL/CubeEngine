#pragma once
#include <memory>
#include <stack>
#include <string>

#include "App/EditorPage.h"
#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"
#include "Scene/EditorCamera.h"
#include "Views/AssetExplorer.h"
#include "Views/SceneSelectPanel.h"
#include "imgui/imgui.h"

constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;

struct ProjectConfig {
	std::string name;
	std::string rootPath; // project root directory
	std::string projectDataDirectory;
	std::string assetsDirectory;
	std::string sceneDirectory;
	std::string assetPathMapFilePath;
};

struct SceneData {
    Cube::Scene* scene = nullptr;
	bool isSaved = false;
};

class Project {
public:
	friend SceneSelectPanel;
	friend EditorPage;

    Project(const std::string& name, const std::string& rootPath);
	Project(const std::string& configFilePath);
	virtual ~Project();

	const std::vector<SceneData>& getScenes() const;
	void addScene(Cube::Scene* scene);
	bool hasScene(const std::string& sceneName) const;

	const ProjectConfig& getConfig() const;

	// global data
	SceneData* selectedScene = nullptr;
    Cube::Entity* selectedEntity = nullptr;
	EditorCamera editorCamera;
    AssetExplorer assetExplorer;

private:
	void writeToConfigFile(const std::string& configFilePath) const;
	void load();
	void save();

	ProjectConfig config;
    std::vector<SceneData> scenes;
};
