#pragma once
#include <fstream>
#include <json.hpp>

#include "RUID.h"
#include "Cube/Core/Log.h"

namespace Cube {

    struct AssetMeta {
        RUID ruid = 0;
	    std::string sourcePath;
        nlohmann::json metaData = {};

        nlohmann::json toJson() const {
           nlohmann::json j;
            j["ruid"] = ruid;
            j["sourcePath"] = sourcePath;
            j["metaData"] = metaData;

            switch(getResourceType(ruid)) {
            case ResourceType::Unknown:         j["type"] = "Unknown";          break;
            case ResourceType::Texture:         j["type"] = "Texture";          break;
            case ResourceType::AnimationClip:   j["type"] = "AnimationClip";    break;
            case ResourceType::Font:            j["type"] = "Font";             break;
            default:                            j["type"] = "Undefined";        break;
            }

            return j;
        }

        void fromJson(const nlohmann::json& j) {
            ruid = j["ruid"];
            sourcePath = j["sourcePath"];
            metaData = j["metaData"];
        }

        void writeToFile(const std::string& path) const {
            std::ofstream file(path);
            if(!file.is_open()) {
                CB_CORE_ERROR("Failed to open file: {}", path);
                return;
            }
            file << toJson().dump(4);
            file.close();
        }
    };

}