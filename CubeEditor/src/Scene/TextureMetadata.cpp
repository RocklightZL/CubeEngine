#include "TextureMetadata.h"
#include "Cube/Core/Log.h"

#include <json.hpp>
#include <fstream>

namespace Cube {

    TextureMetadata::TextureMetadata(const glm::vec2& size) : size(size){}

     TextureMetadata::TextureMetadata(const std::string& filePath) {
        std::ifstream file(filePath);
        if(!file.is_open()) {
            CB_EDITOR_ERROR("TextureMetadata::TextureMetadata: Fail to open file {}", filePath);
            return;
        }
        nlohmann::json data;
        file >> data;
        file.close();
        size.x = data["width"];
        size.y = data["height"];
        for(auto& region : data["regions"]) {
            regions.push_back({{region[0], region[1]}, {region[2], region[3]}});
        }
    }

    void TextureMetadata::toJson(const std::string& filePath) const {
        nlohmann::json data;
        data["width"] = size.x;
        data["height"] = size.y;
        data["regions"] = nlohmann::json::array();
        for(const auto& region : regions) {
            data["regions"].push_back({region.uvMin.x, region.uvMin.y, region.uvMax.x, region.uvMax.y});
        }
        std::ofstream file(filePath);
        if(!file.is_open()) {
            CB_EDITOR_ERROR("TextureMetadata::toJson: Fail to open file {}", filePath);
            return;
        }
        file << data.dump(4);
        file.close();
    }

    void TextureMetadata::addRegion(const glm::vec2& pos, const glm::vec2& regionSize) { regions.push_back({pos / size, (pos + regionSize) / size}); }

    TextureRegion TextureMetadata::getRegion(const glm::vec2& pos) const {
        glm::vec2 texPos = pos / size;
        for(const auto& region : regions) {
            if(texPos.x >= region.uvMin.x && texPos.y >= region.uvMin.y && texPos.x < region.uvMax.x && texPos.y < region.uvMax.y) {
                return region;
            }
        }
        CB_EDITOR_ERROR("TextureMetadata::getRegion: region not found");
        return {{0, 0}, {0, 0}};
    }

}  // namespace Cube