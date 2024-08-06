//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Bird/VertexBufferLayoutData.hpp"
#include "Bird/Base.hpp"

#include <string>
#include <unordered_map>

namespace Bird {

class OpenGLShader {
public:
    OpenGLShader();
    void create(ProgramCreate create);
    void terminate();
    void bind();
    void unbind();
    void setUniformFloat(const char *name, float *value, int count);
    void setUniformVec4(const char *name, float *value, int count);
    void setUniformMat3(const char *name, float *value, int count);
    void setUniformMat4(const char *name, float *value, int count);
    void setUniformInt(const char *name, int *value, int count);
    void bindAttributes(VertexBufferLayoutData &layout, intptr_t baseVertex);

private:
    uint32_t m_id;
    std::unordered_map<std::string, int> m_uniformLocationCache;
    static void checkCompileErrors(uint32_t shader, const std::string &type);
    int getUniformLocation(const std::string &name);
};

} // namespace Bird
