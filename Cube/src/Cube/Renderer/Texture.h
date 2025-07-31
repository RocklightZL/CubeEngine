#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace Cube {

    class Texture2D {
    public:
        Texture2D(const std::string& filePath);
        Texture2D(int width, int height, void* data);
        virtual ~Texture2D();
        void bind(unsigned int slot = 0) const;
        void unbind();

        int getWidth() const;
        int getHeight() const;
        GLuint getId() const;
        const std::string& getFilePath() const;
    private:
        int width;
        int height;
        GLuint id;
        std::string filePath;
    };

    struct SubTexture {
        std::string name;
        glm::vec2 uvMin;
        glm::vec2 uvMax;
        glm::vec2 position;
        glm::vec2 size;
    };

    class TextureAtlas : public Texture2D{
    public:
        TextureAtlas(const std::string& filePath);
        TextureAtlas(const std::string& filePath, const std::string& metaDataPath);
        ~TextureAtlas() = default;

        void addSubTexture(const std::string& name, const SubTexture& subTexture);
        const SubTexture& getSubTexture(const std::string& name);
        const std::string& getFilePath() const;
        const std::string& getMetaDataPath() const;

    private:
        std::string filePath;
        std::string metaDataPath;
        std::unordered_map<std::string, SubTexture> subTextures;
    };
}