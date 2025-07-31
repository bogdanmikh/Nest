//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Bird/VertexBufferLayoutData.hpp"
#include "Bird/Base.hpp"

#include <string>
#include <unordered_map>
#include "VulkanBase.hpp"

struct VulkanShaderDelegate {
    virtual ~VulkanShaderDelegate() = default;
    virtual StateCacheT<VkDescriptorSetLayout> &getDescriptorSetLayoutCache() = 0;
    virtual VkCommandBuffer getCommandBuffer() = 0;
};

namespace Bird {

class VulkanShader {
public:
    void create(Foundation::Memory *memory);
    void terminate();
    VkShaderModule m_module;

private:
    VkDevice m_device;
};

class VulkanProgram {
public:
    void create(ProgramCreate create);
    void terminate();
    void setUniformFloat(const char *name, float *value, int count);
    void setUniformVec4(const char *name, float *value, int count);
    void setUniformMat3(const char *name, float *value, int count);
    void setUniformMat4(const char *name, float *value, int count);
    void setUniformInt(const char *name, int *value, int count);

    VkShaderModule getVertexModule();
    VkShaderModule getFragmentModule();

    VkDevice m_device;
    VulkanShaderDelegate *m_delegate;

    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;

private:
    VulkanShader m_vertex;
    VulkanShader m_fragment;
};

} // namespace Bird
