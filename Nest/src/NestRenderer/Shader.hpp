#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);
    ~Shader();
    void use();
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec2(const std::string &name, const glm::vec2 &value);
    void setVec2(const std::string &name, float x, float y);
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setVec3(const std::string &name, float x, float y, float z);
    void setVec4(const std::string &name, const glm::vec4 &value);
    void setVec4(const std::string &name, float x, float y, float z, float w);
    void setMat2(const std::string &name, const glm::mat2 &mat);
    void setMat3(const std::string &name, const glm::mat3 &mat);
    void setMat4(const std::string &name, const glm::mat4 &mat);

private:
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    void checkCompileErrors(uint32_t shader, std::string type);
    int getUniformLocation(const std::string &name);
};
