#include "pch.h"
#include "Texture.h"

#include <fstream>
#include <stb_image.h>
#include <json.hpp>

namespace Cube {

    Texture2D::Texture2D(const AssetMeta& assetMeta) : ResourceBase(assetMeta) {
        std::string filePath = assetMeta.sourcePath;

        // TODO: use metaData
        if(!assetMeta.metaData.empty()){}

        stbi_set_flip_vertically_on_load(1);
        int channels;
        uint8_t* originalData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
        if(!originalData) {
            CB_CORE_ERROR("Failed to load image: {}", filePath);
            CB_ASSERT(0);
        }
        GLenum internalFormat = 0, dataFormat = 0;
        int pixelSize = 3;
        switch(channels) {
        case 3:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            pixelSize = 3;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            pixelSize = 4;
            break;
        default:
            CB_CORE_ERROR("Unsupported image format");
            CB_ASSERT(true);
        }

        // alignment
        int rowSize = width * pixelSize;
        int alignedSize = (rowSize + 3) / 4 * 4;
        uint8_t* data = originalData;
        if(rowSize != alignedSize) {
            data = new uint8_t[(size_t)(alignedSize * height)];
            for(int i = 0; i < height; i++) {
                memcpy(data + i * alignedSize, originalData + i * rowSize, rowSize);
                memset(data + i * alignedSize + rowSize, 0, alignedSize - rowSize);
            }
        }

        glGenTextures(1, &id);
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(originalData);
        if(rowSize != alignedSize) {
            delete[] data;
        }
    }

    Texture2D::Texture2D(const std::string& filePath){
        stbi_set_flip_vertically_on_load(1);
        int channels;
        uint8_t* originalData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
        if(!originalData) {
            CB_CORE_ERROR("Failed to load image: {}", filePath);
            CB_ASSERT(0);
        }
        GLenum internalFormat = 0, dataFormat = 0;
        int pixelSize = 3;
        switch(channels) {
        case 3:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            pixelSize = 3;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            pixelSize = 4;
            break;
        default:
            CB_CORE_ERROR("Unsupported image format");
            CB_ASSERT(true);
        }

        // alignment
        int rowSize = width * pixelSize;
        int alignedSize = (rowSize + 3) / 4 * 4;
        uint8_t* data = originalData;
        if(rowSize != alignedSize) {
            data = new uint8_t[(size_t)(alignedSize * height)];
            for(int i = 0; i < height; i++) {
                memcpy(data + i * alignedSize, originalData + i * rowSize, rowSize);
                memset(data + i * alignedSize + rowSize, 0, alignedSize - rowSize);
            }
        }

        glGenTextures(1, &id);
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(originalData);
        if(rowSize != alignedSize) {
            delete[] data;
        }
    }
    
    Texture2D::Texture2D(int width, int height, uint8_t* originalData) : width(width), height(height){
        // alignment
        int rowSize = width * 4;
        int alignedSize = (rowSize + 3) / 4 * 4;
        uint8_t* data = originalData;
        if(rowSize != alignedSize) {
            data = new uint8_t[(size_t)(alignedSize * height)];
            for(int i = 0; i < height; i++) {
                memcpy(data + i * alignedSize, originalData + i * rowSize, rowSize);
                memset(data + i * alignedSize + rowSize, 0, alignedSize - rowSize);
            }
        }

        glGenTextures(1, &id);
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if(rowSize != alignedSize) {
            delete[] data;
        }
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &id);
    }

    void Texture2D::bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

    // only for RGBA
    void Texture2D::updateData(int x, int y, int w, int h, const unsigned char* data) {
        bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    TextureAtlas::TextureAtlas(const std::string& filePath) : Texture2D(filePath), filePath(filePath) {}

    /*
     * 元数据，用于解析纹理图集
     * 约定json格式：
     * {
         "textureWidth": 1024,
         "textureHeight": 1024,
         "subTextures": [
           {
             "name": "player_idle",
             "x": 0,
             "y": 0,
             "width": 64,
             "height": 64
           },
           ...
         ]
       }
     */
    TextureAtlas::TextureAtlas(const std::string& filePath, const std::string& metaDataPath) : Texture2D(filePath), filePath(filePath), metaDataPath(metaDataPath) {
        std::ifstream file(metaDataPath);
        if(!file.is_open()) {
            CB_CORE_ERROR("Failed to open metadata file: {}", metaDataPath);
            CB_ASSERT(true);
        }
        nlohmann::json jsonData;
        file >> jsonData;
        file.close();

        int textureWidth = jsonData["textureWidth"];
        int textureHeight = jsonData["textureHeight"];

        // 检查纹理尺寸是否与实际加载的纹理尺寸一致
        if(textureWidth != getWidth() || textureHeight != getHeight()) {
            CB_CORE_ERROR("Texture size mismatch: expected {}x{}, got {}x{}", textureWidth, textureHeight, getWidth(), getHeight());
            CB_ASSERT(true);
        }
        
        for(const auto& subTexture : jsonData["subTextures"]) {
            SubTexture st;
            st.name = subTexture["name"];
            st.position = glm::vec2(subTexture["x"], subTexture["y"]);
            st.size = glm::vec2(subTexture["width"], subTexture["height"]);
            st.uvMin = glm::vec2(st.position.x / (float)textureWidth, st.position.y / (float)textureHeight);
            st.uvMax = glm::vec2((st.position.x + st.size.x) / (float)textureWidth, (st.position.y + st.size.y) / (float)textureHeight);
            addSubTexture(st.name, st);
        }
    }

    void TextureAtlas::addSubTexture(const std::string& name, const SubTexture& subTexture) {
        subTextures.insert(std::make_pair(name, subTexture));
    }

    const SubTexture& TextureAtlas::getSubTexture(const std::string& name) {
        auto res = subTextures.find(name);
        if(res == subTextures.end()) {
            CB_CORE_ERROR("Failed to find subTexture: {}", name);
        }
        return res->second;
    }

    const std::string& TextureAtlas::getFilePath() const { return filePath; }
    const std::string& TextureAtlas::getMetaDataPath() const { return metaDataPath; }

}  // namespace Cube