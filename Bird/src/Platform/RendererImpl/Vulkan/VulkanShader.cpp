//
// Created by Admin on 11.02.2022.
//

#include "VulkanShader.hpp"
#include "Bird/Base.hpp"

#include <sstream>
#include <fstream>
#include <string>

namespace Bird {

VulkanShader::VulkanShader()
    : m_id(-1)
    , m_uniformLocationCache() {}

void VulkanShader::create(ProgramCreate create) {}

void VulkanShader::terminate() {}

void VulkanShader::checkCompileErrors(unsigned int shader, const std::string &type) {}
int VulkanShader::getUniformLocation(const std::string &name) {}

void VulkanShader::bindAttributes(VertexBufferLayoutData &layout, intptr_t baseVertex) {}

void VulkanShader::bind() {}

void VulkanShader::unbind() {}

void VulkanShader::setUniformFloat(const char *name, float *value, int count) {}

void VulkanShader::setUniformVec4(const char *name, float *value, int count) {}

void VulkanShader::setUniformMat3(const char *name, float *value, int count) {}

void VulkanShader::setUniformMat4(const char *name, float *value, int count) {}

void VulkanShader::setUniformInt(const char *name, int *value, int count) {}

} // namespace Bird
