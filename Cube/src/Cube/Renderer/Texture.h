#pragma once
#include <string>
#include <glad/glad.h>

namespace Cube {

    class Texture2D {
    public:
        Texture2D(const std::string& filePath);
        ~Texture2D();
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

}