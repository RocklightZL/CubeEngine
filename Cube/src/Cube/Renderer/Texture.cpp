#include "pch.h"
#include "Texture.h"

#include <stb_image/stb_image.h>

namespace Cube {

    Texture2D::Texture2D(const std::string& filePath) {
        stbi_set_flip_vertically_on_load(1);
        int channels;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
        if(!data) {
            CB_CORE_ERROR("Failed to load image: {}", filePath);
            CB_ASSERT(true);
        }
        GLenum internalFormat = 0, dataFormat = 0;
        switch(channels) {
        case 3:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
        default:
            CB_CORE_ERROR("Unsupported image format");
            CB_ASSERT(true);
        }
        glGenTextures(1, &id);
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    
    Texture2D::Texture2D(int width, int height, void* data) : width(width), height(height){
        // white texture
        glGenTextures(1, &id);
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &id);
    }

    void Texture2D::bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

    int Texture2D::getWidth() const {
        return width;
    }
    int Texture2D::getHeight() const {
        return height;
    }
    GLuint Texture2D::getId() const { return id; }

    TextureAlas::TextureAlas(const std::string& filePath) : Texture2D(filePath) {}

    TextureAlas::TextureAlas(const std::string& filePath, const std::string& metaDataPath) : Texture2D(filePath) {
        // 元数据，用于解析纹理图集
    }

    void TextureAlas::addSubTexture(const std::string& name, const SubTexture& subTexture) {
        subTextures.insert(std::make_pair(name, subTexture));
    }

    const SubTexture& TextureAlas::getSubTexture(const std::string& name) {
        auto res = subTextures.find(name);
        if(res == subTextures.end()) {
            CB_CORE_ERROR("Failed to find subTexture: {}", name);
        }
        return res->second;
    }

}  // namespace Cube