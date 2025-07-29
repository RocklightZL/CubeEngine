#include "Project.h"

#include "Cube/Core/Log.h"
#include "Cube/Renderer/RenderSystem.h"
#include "Cube/Scene/SceneSerializer.h"

#include <json.hpp>
#include <fstream>
#include <filesystem>

namespace Cube {

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
    }

    Project::Project(const std::string& configFilePath) {
        nlohmann::json data;
        std::ifstream file(configFilePath);
        if(!file.is_open()) {
            CB_ERROR("Project::Project: Failed to open file: {}", configFilePath);
            CB_ASSERT(true);
            return;
        }
        file >> data;

        config.name = data["name"];
        config.rootPath = data["rootPath"];
        config.projectDataDirectory = data["projectDataDirectory"];
        config.resourcesDirectory = data["resourcesDirectory"];
        config.sceneDirectory = data["sceneDirectory"];

        load();

        resRoot = std::make_shared<Node>();
        resRoot->name = "root";
        resRoot->isGroup = true;
        currentNode = resRoot;
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
            CB_ASSERT(true);
            return;
        }
        file << data.dump(4);

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
            CB_ASSERT(true);
            return;
        }
        file << data.dump(4);
    }

    void Project::load() {
        std::ifstream file(config.projectDataDirectory + "/scenes.cache");
        if(!file.is_open()) {
            CB_ERROR("Project::load: failed to open file: {}", config.projectDataDirectory + "/scenes.cache");
            CB_ASSERT(true);
            return;
        }
        nlohmann::json data;
        file >> data;
        for(auto& s : data["scenes"]) {
            Scene* scene = new Scene();
            SceneSerializer::deserialize(scene, config.sceneDirectory + "/" + s.get<std::string>() + ".scene");
            scenes.push_back({scene, true});
        }
        std::string t(data["selectedScene"]);
        for(auto& s : scenes) {
            if(t == s.scene->getName()) {
                selectedScene = &s;
            }
        }
    }

}  // namespace Cube