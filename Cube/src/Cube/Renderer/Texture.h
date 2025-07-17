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
    private:
        int width;
        int height;
        GLuint id;
    };

    struct SubTexture {
        std::string name;
        glm::vec2 uvMin;
        glm::vec2 uvMax;
        glm::vec2 position;
        glm::vec2 size;
    };

    class TextureAlas : public Texture2D{
    public:
        TextureAlas(const std::string& filePath);
        TextureAlas(const std::string& filePath, const std::string& metaDataPath);
        ~TextureAlas() = default;

        void addSubTexture(const std::string& name, const SubTexture& subTexture);
        const SubTexture& getSubTexture(const std::string& name);
    private:
        std::unordered_map<std::string, SubTexture> subTextures;
    };
}