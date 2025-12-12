#pragma once
#include <json.hpp>

#include "RUID.h"

namespace Cube {

    struct AssetMeta {
        RUID ruid;
	    std::string sourcePath;
        nlohmann::json metaData;

        nlohmann::json toJson() const {
           nlohmann::json j;
            j["ruid"] = ruid;
            j["sourcePath"] = sourcePath;
            j["metaData"] = metaData;

            switch(getResourceType(ruid)) {
            case ResourceType::Unknown:         j["type"] = "Font";             break;
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
    };

}