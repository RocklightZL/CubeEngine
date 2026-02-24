#include "Project.h"

#include <filesystem>
#include <fstream>
#include <json.hpp>

#include "Cube/Core/Log.h"
#include "Cube/Resource/ResourceManager.h"
#include "Cube/Utils/Utils.h"

using namespace Cube;

Project::Project(const std::string& name, const std::string& rootPath) {
    config.name = name;
    config.rootPath = rootPath;
    config.projectDataDirectory = rootPath + "/.cube";
    config.assetsDirectory = rootPath + "/Assets";
    config.sceneDirectory = rootPath + "/Scenes";
    config.assetPathMapFilePath = config.rootPath + "/AssetMap.json";

    std::filesystem::create_directories(config.projectDataDirectory);
    std::filesystem::create_directories(config.sceneDirectory);
    std::filesystem::create_directories(config.assetsDirectory);
    assetExplorer.normalInit();
    writeToConfigFile(rootPath + "/" + name + ".cbproj");
}

Project::Project(const std::string& configFilePath) {
    nlohmann::json data;
    std::ifstream file(configFilePath);
    if(!file.is_open()) {
        CB_ERROR("Project::Project: Failed to open file: {}", configFilePath);
        CB_ASSERT(false);
        return;
    }
    file >> data;
    file.close();

    config.name = data["name"];
    config.rootPath = Utils::getParentPath(configFilePath);
    config.projectDataDirectory = config.rootPath + "/.cube";
    config.assetsDirectory = config.rootPath + "/Assets";
    config.sceneDirectory = config.rootPath + "/Scenes";
    config.assetPathMapFilePath = config.rootPath + "/AssetMap.json";
    load();
}

Project::~Project() {
    save();

    for(auto s : scenes) {
        delete s.scene;
    }
}

const std::vector<SceneData>& Project::getScenes() const { return scenes; }

void Project::addScene(Scene* scene) {
    scenes.push_back({scene, false});
    selectedScene = &scenes.back();
}

bool Project::hasScene(const std::string& sceneName) const {
    auto it = std::find_if(scenes.begin(), scenes.end(), [sceneName](SceneData s) { return s.scene->getName() == sceneName; });
    return it != scenes.end();
}

const ProjectConfig& Project::getConfig() const {
    return config;
}

void Project::save() {

    nlohmann::json data;
    data["scenes"] = nlohmann::json::array();
    for(auto& s : scenes) {
        data["scenes"].push_back(s.scene->getName());
    }
    data["selectedScene"] = selectedScene ? selectedScene->scene->getName() : "";

    std::ofstream file(config.projectDataDirectory + "/scenes.cache");
    if(!file.is_open()) {
        CB_ERROR("Project::save: failed to open file: {}", config.projectDataDirectory + "/scenes.cache");
        CB_ASSERT(false);
        return;
    }
    file << data.dump(4);
    file.close();

    assetExplorer.saveToFile(config.projectDataDirectory + "/resources.cache", config.assetPathMapFilePath);
}

void Project::writeToConfigFile(const std::string& configFilePath) const {
    nlohmann::json data;
    data["name"] = config.name;

    std::ofstream file(configFilePath);
    if(!file.is_open()) {
        CB_ERROR("Project::Project: Failed to open file: {}", configFilePath);
        CB_ASSERT(false);
        return;
    }
    file << data.dump(4);
}

void Project::load() {
    // resources.cache
    assetExplorer.loadFromFile(config.projectDataDirectory + "/resources.cache", config.assetPathMapFilePath);

    // scenes.cache
    std::ifstream file(config.projectDataDirectory + "/scenes.cache");
    if(!file.is_open()) {
        CB_ERROR("Project::load: failed to open file: {}", config.projectDataDirectory + "/scenes.cache");
        CB_ASSERT(false);
        return;
    }
    nlohmann::json data;
    file >> data;
    for(auto& s : data["scenes"]) {
        Scene* scene = new Scene(config.sceneDirectory + "/" + s.get<std::string>() + ".scene");
        scenes.push_back({scene, true});
    }
    std::string t(data["selectedScene"]);
    for(auto& s : scenes) {
        if(t == s.scene->getName()) {
            selectedScene = &s;
        }
    }
    file.close();
}
