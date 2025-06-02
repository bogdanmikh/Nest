//
// Created by Admin on 11.02.2022.
//

#include "VulkanShader.hpp"

#include <sstream>
#include <fstream>
#include <string>

uint64_t getHashDescriptorSetLayout(VkDescriptorSetLayout &descriptorSetLayout) {
    return 0;
}

namespace Bird {

VulkanShader::VulkanShader()
    : m_uniformLocationCache() {}

void VulkanShader::create(ProgramCreate create) {
    m_device = *g_device;
    const char *vertexSource = reinterpret_cast<const char *>(create.m_vertex.data);
    const char *fragmentSource = reinterpret_cast<const char *>(create.m_fragment.data);
    uint32_t vertexSourceSize = *reinterpret_cast<uint32_t *>(create.m_vertex.userData);
    uint32_t fragmentSourceSize = *reinterpret_cast<uint32_t *>(create.m_fragment.userData);
    free(create.m_vertex.userData);
    free(create.m_fragment.userData);

    VkShaderModuleCreateInfo vertShaderCreateInfo = {};
    vertShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertShaderCreateInfo.codeSize = vertexSourceSize;
    vertShaderCreateInfo.pCode = reinterpret_cast<const uint32_t *>(create.m_vertex.data);

    VK_CHECK(vkCreateShaderModule(m_device, &vertShaderCreateInfo, g_allocatorCb, &m_vertex));

    VkShaderModuleCreateInfo fragShaderCreateInfo = {};
    fragShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragShaderCreateInfo.codeSize = fragmentSourceSize;
    fragShaderCreateInfo.pCode = reinterpret_cast<const uint32_t *>(create.m_fragment.data);
    VK_CHECK(vkCreateShaderModule(m_device, &fragShaderCreateInfo, g_allocatorCb, &m_fragment));

    create.m_vertex.release();
    create.m_fragment.release();

    // ?
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = NULL;
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = NULL;
    pipelineLayoutCreateInfo.setLayoutCount = (descriptorSetLayout == VK_NULL_HANDLE ? 0 : 1);
    pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;

    m_delegate->getDescriptorSetLayoutCache().add(
        getHashDescriptorSetLayout(descriptorSetLayout), descriptorSetLayout
    );

    VK_CHECK(vkCreatePipelineLayout(
        *g_device, &pipelineLayoutCreateInfo, g_allocatorCb, &m_pipelineLayout
    ));
}

void VulkanShader::terminate() {
    vkDestroy(m_vertex);
    vkDestroy(m_fragment);
    vkDestroy(m_pipelineLayout);
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
