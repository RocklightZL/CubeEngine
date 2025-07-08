#include "pch.h"
#include "Shader.h"
#include <fstream>
#include <sstream>

namespace Cube {

    std::string readFileToString(const std::string& filePath)  {
        std::ifstream file(filePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("cannot open file " + filePath);
            CB_ASSERT("Failed to open file!");
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    Shader::Shader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc) {
        uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
        uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);

        GLint success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success) {
            char log[512];
            glGetProgramInfoLog(id, 512, nullptr, log);
            CB_CORE_ERROR("shader link error: " + std::string(log));
            CB_ASSERT("Shader link error");
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader() {
        glDeleteProgram(id);
    }

    void Shader::bind() {
        glUseProgram(id);
    }

    void Shader::unbind() { glUseProgram(0); }

    uint32_t Shader::getId() const {return id;}

    uint32_t Shader::compileShader(GLenum type, const std::string src){
        uint32_t shader = glCreateShader(type);
        const char* s = src.c_str();
        glShaderSource(shader, 1, &s, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            std::string shaderType = (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader");
            CB_CORE_ERROR(shaderType + " compile error : " + std::string(log));
            CB_ASSERT("Shader compile error!");
        }
        return shader;
    }

    void Shader::setFloat(const std::string& name, float val) {
        glUniform1f(glGetUniformLocation(id, name.c_str()), val);
    }

    void Shader::setInt(const std::string& name, int val) {
        glUniform1i(glGetUniformLocation(id, name.c_str()), val);
    }

    void Shader::setVec2(const std::string& name, Vec2 val) {
        glUniform2f(glGetUniformLocation(id, name.c_str()), val.x, val.y);
    }

    void Shader::setVec3(const std::string& name, Vec3 val) {
        glUniform3f(glGetUniformLocation(id, name.c_str()), val.x, val.y, val.z);
    }

    void Shader::setVec4(const std::string& name, Vec4 val) {
        glUniform4f(glGetUniformLocation(id, name.c_str()), val.x, val.y, val.z, val.w);
    }

    void Shader::setMat3(const std::string& name, Mat3 val) {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, val.data);
    }

    void Shader::setMat4(const std::string& name, Mat4 val) {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, val.data);
    }

}  // namespace Cube