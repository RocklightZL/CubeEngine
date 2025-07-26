#include "Project.h"

#include "Cube/Core/Log.h"

#include <json.hpp>
#include <fstream>

namespace Cube {

    Project::Project(const std::string& name, const std::string& rootPath) {
        config.name = name;
        config.rootPath = rootPath;
        config.projectDataDirectory = rootPath + "/.cube";
        config.resourcesDirectory = rootPath + "/Resources";
        config.sceneDirectory = rootPath + "/Scenes";

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
    }

    Project::~Project() {
        for(auto s : scenes) {
            delete s;
        }
    }

    const std::vector<Scene*>& Project::getScenes() const { return scenes; }

    void Project::addScene(Scene* scene) {
        scenes.push_back(scene);
        selectedScene = scene;
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

}  // namespace Cube