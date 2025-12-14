#pragma once
#include <memory>
#include <stack>
#include <string>

#include "App/EditorPage.h"
#include "Cube/Resource/RUID.h"
#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"
#include "Scene/EditorCamera.h"
#include "Views/Node.h"
#include "Views/SceneSelectPanel.h"
#include "imgui/imgui.h"

constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;

struct ProjectConfig {
	std::string name;
	std::string rootPath; // project root directory
	std::string projectDataDirectory;
	std::string assetsDirectory;
	std::string sceneDirectory;
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
    std::shared_ptr<Node> resRoot;  // TODO: Unique_ptr
	std::deque<std::shared_ptr<Node>> resStack;
	EditorCamera editorCamera;
	std::unordered_map<std::filesystem::path, Cube::RUID> assetMetaCache;

private:
	void writeToConfigFile(const std::string& configFilePath) const;
	void load();
	void save();
	void importAssets();

	ProjectConfig config;
    std::vector<SceneData> scenes;
};
