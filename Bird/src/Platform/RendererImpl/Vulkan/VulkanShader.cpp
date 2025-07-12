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

void VulkanShader::bindAttributes(VertexBufferLayoutData &layout, intptr_t baseVertex) {
    // Конвертируем VertexBufferLayoutData в Vulkan vertex input bindings и attributes
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = layout.m_stride;
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(layout.m_elementsCount);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < layout.m_elementsCount; ++i) {
        const auto &element = layout.m_elements[i];

        attributeDescriptions[i].binding = 0;
        attributeDescriptions[i].location = i;
        attributeDescriptions[i].offset = offset;

        switch (element.type) {
            case BufferElementType::Float:
                attributeDescriptions[i].format = element.count == 1   ? VK_FORMAT_R32_SFLOAT
                                                  : element.count == 2 ? VK_FORMAT_R32G32_SFLOAT
                                                  : element.count == 3
                                                      ? VK_FORMAT_R32G32B32_SFLOAT
                                                      : VK_FORMAT_R32G32B32A32_SFLOAT;
                break;
            case BufferElementType::UnsignedInt:
                attributeDescriptions[i].format = element.count == 1   ? VK_FORMAT_R32_UINT
                                                  : element.count == 2 ? VK_FORMAT_R32G32_UINT
                                                  : element.count == 3
                                                      ? VK_FORMAT_R32G32B32_UINT
                                                      : VK_FORMAT_R32G32B32A32_UINT;
                break;
            case BufferElementType::Int:
                attributeDescriptions[i].format = element.count == 1   ? VK_FORMAT_R32_SINT
                                                  : element.count == 2 ? VK_FORMAT_R32G32_SINT
                                                  : element.count == 3
                                                      ? VK_FORMAT_R32G32B32_SINT
                                                      : VK_FORMAT_R32G32B32A32_SINT;
                break;
            case BufferElementType::UnsignedShort:
                attributeDescriptions[i].format =
                    element.normalized ? (element.count == 1   ? VK_FORMAT_R16_UNORM
                                          : element.count == 2 ? VK_FORMAT_R16G16_UNORM
                                          : element.count == 3 ? VK_FORMAT_R16G16B16_UNORM
                                                               : VK_FORMAT_R16G16B16A16_UNORM)
                                       : (element.count == 1   ? VK_FORMAT_R16_UINT
                                          : element.count == 2 ? VK_FORMAT_R16G16_UINT
                                          : element.count == 3 ? VK_FORMAT_R16G16B16_UINT
                                                               : VK_FORMAT_R16G16B16A16_UINT);
                break;
            case BufferElementType::UnsignedByte:
                attributeDescriptions[i].format =
                    element.normalized ? (element.count == 1   ? VK_FORMAT_R8_UNORM
                                          : element.count == 2 ? VK_FORMAT_R8G8_UNORM
                                          : element.count == 3 ? VK_FORMAT_R8G8B8_UNORM
                                                               : VK_FORMAT_R8G8B8A8_UNORM)
                                       : (element.count == 1   ? VK_FORMAT_R8_UINT
                                          : element.count == 2 ? VK_FORMAT_R8G8_UINT
                                          : element.count == 3 ? VK_FORMAT_R8G8B8_UINT
                                                               : VK_FORMAT_R8G8B8A8_UINT);
                break;
        }

        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VulkanShader::setUniformFloat(const char *name, float *value, int count) {}

void VulkanShader::setUniformVec4(const char *name, float *value, int count) {}

void VulkanShader::setUniformMat3(const char *name, float *value, int count) {}

void VulkanShader::setUniformMat4(const char *name, float *value, int count) {}

void VulkanShader::setUniformInt(const char *name, int *value, int count) {}

} // namespace Bird
