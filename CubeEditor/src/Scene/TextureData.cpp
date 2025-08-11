#include "TextureData.h"

#include "Cube/Core/Log.h"

#include <fstream>
#include <json.hpp>

namespace Cube {

    TextureData::TextureData(const std::string& filePath, const std::string& imagePath) : filePath(filePath), imagePath(imagePath){
        std::ifstream file(filePath);
        if(!file.is_open()) {
            CB_EDITOR_ERROR("TextureData::TextureData: Failed to open file {}", filePath);
            return;
        }
        nlohmann::json data;
        file >> data;
        file.close();
        size = {data["meta"]["size"]["w"], data["meta"]["size"]["h"]};
        for(auto& frame : data["frames"]) {
            SubTexture st;
            st.name = frame["filename"];
            st.position = {frame["frame"]["x"], frame["frame"]["y"]};
            st.size = {frame["frame"]["w"], frame["frame"]["h"]};
            st.uvMin = glm::vec2(st.position.x, size.y - st.position.y - st.size.y) / size;
            st.uvMax = st.uvMin + st.size / size;
            textures.push_back(st);
        }
    }
}  // namespace Cube