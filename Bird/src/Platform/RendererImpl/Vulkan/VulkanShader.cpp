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
    : m_uniformLocationCache() {}

void VulkanShader::create(ProgramCreate create) {
    const char *vertexSource = reinterpret_cast<const char *>(create.m_vertex.data);
    const char *fragmentSource = reinterpret_cast<const char *>(create.m_fragment.data);
    size_t vertexSourceSize = *reinterpret_cast<uint32_t *>(create.m_vertex.userData);
    size_t fragmentSourceSize = *reinterpret_cast<uint32_t *>(create.m_fragment.userData);
    free(create.m_vertex.userData);
    free(create.m_fragment.userData);

    VkShaderModuleCreateInfo vertShaderCreateInfo = {};
    vertShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertShaderCreateInfo.codeSize = vertexSourceSize;
    vertShaderCreateInfo.pCode = reinterpret_cast<const uint32_t *>(create.m_vertex.data);

    VK_CHECK(vkCreateShaderModule(m_device, &vertShaderCreateInfo, nullptr, &m_vertex));

    VkShaderModuleCreateInfo fragShaderCreateInfo = {};
    fragShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragShaderCreateInfo.codeSize = fragmentSourceSize;
    fragShaderCreateInfo.pCode = reinterpret_cast<const uint32_t *>(create.m_fragment.data);
    VK_CHECK(vkCreateShaderModule(m_device, &fragShaderCreateInfo, nullptr, &m_fragment));

    create.m_vertex.release();
    create.m_fragment.release();

    //    VkPipelineLayoutCreateInfo plci;
    //    plci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    //    plci.pNext = NULL;
    //    plci.flags = 0;
    //    plci.pushConstantRangeCount = 0;
    //    plci.pPushConstantRanges = NULL;
    //    plci.setLayoutCount = (dsl == VK_NULL_HANDLE ? 0 : 1);
    //    plci.pSetLayouts = &dsl;
    //
    //    VK_CHECK(vkCreatePipelineLayout(
    //        s_renderVK->m_device
    //        , &plci
    //        , s_renderVK->m_allocatorCb
    //        , &m_pipelineLayout
    //    ) );
}

void VulkanShader::terminate() {
    vkDestroyShaderModule(m_device, m_vertex, nullptr);
    vkDestroyShaderModule(m_device, m_fragment, nullptr);
}

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
