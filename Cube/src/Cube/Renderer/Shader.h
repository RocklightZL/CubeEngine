#pragma once
#include "Cube/Math/Type.h"

#include <string>
#include <glad/glad.h>

namespace Cube {

    // file to string
    std::string readFileToString(const std::string& filePath);

    class Shader {
    public:
        Shader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
        ~Shader();

        void bind();
        static void unbind();

        uint32_t getId() const;

        // …Ë÷√Uniform
        void setFloat(const std::string& name, float val);
        void setInt(const std::string& name, int val);
        void setVec2(const std::string& name, Vec2 val);
        void setVec3(const std::string& name, Vec3 val);
        void setVec4(const std::string& name, Vec4 val);
        void setMat3(const std::string& name, Mat3 val);
        void setMat4(const std::string& name, Mat4 val);

    private:
        uint32_t compileShader(GLenum type, const std::string src);

        uint32_t id;
    };
}  // namespace Cube