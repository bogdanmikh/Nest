//
// Created by Admin on 11.02.2022.
//

#include "VulkanShader.hpp"

#include <sstream>
#include <fstream>
#include <string>

uint64_t getDescriptorSetLayoutHashKey() {
    return 0;
}

namespace Bird {

void VulkanShader::create(Foundation::Memory *memory, const ShaderType &type) {
    m_device = *g_device;
    uint32_t sourceSize = *reinterpret_cast<uint32_t *>(memory->userData);
    const char *source = reinterpret_cast<const char *>(memory->data);

    VkShaderModuleCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.codeSize = sourceSize;
    shaderCreateInfo.pCode = reinterpret_cast<const uint32_t *>(source);

    VK_CHECK(vkCreateShaderModule(m_device, &shaderCreateInfo, g_allocatorCb, &m_module));

    free(memory->userData);
    memory->release();

    VkShaderStageFlagBits shaderStage = VK_SHADER_STAGE_ALL;

    if (type == ShaderType::VERTEX) {
        shaderStage = VK_SHADER_STAGE_VERTEX_BIT;
    } else if (type == ShaderType::FRAGMENT) {
        shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
    }
}

void VulkanShader::terminate() {
    vkDestroy(m_module);
}

void VulkanProgram::create(ProgramCreate create) {
    m_device = *g_device;

    m_vertex.create(&create.m_vertex, ShaderType::VERTEX);
    m_fragment.create(&create.m_fragment, ShaderType::FRAGMENT);
    // ?
    uint64_t hashKey = getDescriptorSetLayoutHashKey();
    m_descriptorSetLayout = m_delegate->getDescriptorSetLayoutCache().find(hashKey);

    m_delegate->getDescriptorSetLayoutCache().add(hashKey, m_descriptorSetLayout);

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = NULL;
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = NULL;
    pipelineLayoutCreateInfo.setLayoutCount = (m_descriptorSetLayout == VK_NULL_HANDLE ? 0 : 1);
    pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

    VK_CHECK(vkCreatePipelineLayout(
        *g_device, &pipelineLayoutCreateInfo, g_allocatorCb, &m_pipelineLayout
    ));
}

void VulkanProgram::terminate() {
    m_vertex.terminate();
    m_fragment.terminate();
    vkDestroy(m_pipelineLayout);
}

void VulkanProgram::setUniformFloat(const char *name, float *value, int count) {}

void VulkanProgram::setUniformVec4(const char *name, float *value, int count) {}

void VulkanProgram::setUniformMat3(const char *name, float *value, int count) {}

void VulkanProgram::setUniformMat4(const char *name, float *value, int count) {}

void VulkanProgram::setUniformInt(const char *name, int *value, int count) {}

VkShaderModule VulkanProgram::getVertexModule() {
    return m_vertex.m_module;
}

VkShaderModule VulkanProgram::getFragmentModule() {
    return m_fragment.m_module;
}

} // namespace Bird
