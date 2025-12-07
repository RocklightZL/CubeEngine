#include "Project.h"

#include "Cube/Core/Log.h"

#include <json.hpp>
#include <fstream>
#include <filesystem>

using namespace Cube;

Project::Project(const std::string& name, const std::string& rootPath) {
    config.name = name;
    config.rootPath = rootPath;
    config.projectDataDirectory = rootPath + "/.cube";
    config.resourcesDirectory = rootPath + "/Resources";
    config.sceneDirectory = rootPath + "/Scenes";

    std::filesystem::create_directories(config.projectDataDirectory);
    std::filesystem::create_directories(config.sceneDirectory);
    std::filesystem::create_directories(config.resourcesDirectory);

    writeToConfigFile(rootPath + "/" + name + ".cbproj");

    resRoot = std::make_shared<Node>();
    resRoot->name = "root";
    resRoot->isGroup = true;
    resStack.push_back(resRoot);
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

    config.name = data["name"];
    config.rootPath = data["rootPath"];
    config.projectDataDirectory = data["projectDataDirectory"];
    config.resourcesDirectory = data["resourcesDirectory"];
    config.sceneDirectory = data["sceneDirectory"];
    // TODO: 应该每次根据根目录自动生成，而不是记录在配置文件中，否则当整个项目文件夹移动后路径会失效。
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

    std::ofstream resFile(config.projectDataDirectory + "/resources.cache");
    if(!resFile.is_open()) {
        CB_ERROR("Project::save: failed to open file: {}", config.projectDataDirectory + "/resources.cache");
        CB_ASSERT(false);
        return;
    }
    resFile << resRoot->toJson().dump(4);
    resFile.close();
}

void Project::writeToConfigFile(const std::string& configFilePath) const {
    nlohmann::json data;
    data["name"] = config.name;
    data["rootPath"] = config.rootPath;
    data["projectDataDirectory"] = config.projectDataDirectory;
    data["resourcesDirectory"] = config.resourcesDirectory;
    data["sceneDirectory"] = config.sceneDirectory;

    std::ofstream file(configFilePath);
    if(!file.is_open()) {
        CB_ERROR("Project::Project: Failed to open file: {}", configFilePath);
        CB_ASSERT(false);
        return;
    }
    file << data.dump(4);
}

void Project::load() {
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

    // resources.cache
    std::ifstream resFile(config.projectDataDirectory + "/resources.cache");
    if(!resFile.is_open()) {
        CB_ERROR("Project::load: failed to open file: {}", config.projectDataDirectory + "/resources.cache");
        CB_ASSERT(false);
        return;
    }
    nlohmann::json resData;
    resFile >> resData;
    resRoot = std::make_shared<Node>();
    resRoot->fromJson(resData, resRoot);
    resStack.push_back(resRoot);
    resFile.close();
}
