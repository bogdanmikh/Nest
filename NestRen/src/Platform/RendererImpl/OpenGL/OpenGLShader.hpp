//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "NestRen/VertexBufferLayoutData.hpp"
#include "NestRen/Base.hpp"

#include <string>
#include <unordered_map>

namespace NestRen {

class OpenGLShader {
public:
    OpenGLShader();
    void create(ProgramCreate create);
    void terminate();
    void bind();
    void unbind();
    void setUniformMat4(const char *name, float *value);
    void setUniformInt(const char *name, int value);
    void setUniformIntArray(const char *name, int *value);
    void bindAttributes(VertexBufferLayoutData &layout, intptr_t baseVertex);

private:
    uint32_t m_id;
    std::unordered_map<std::string, int> m_uniformLocationCache;
    static void checkCompileErrors(uint32_t shader, const std::string &type);
    int getUniformLocation(const std::string &name);
};

} // namespace NestRen
