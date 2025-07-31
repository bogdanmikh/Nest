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
    VulkanShader();
    void create(ProgramCreate create);
    void terminate();
    void setUniformFloat(const char *name, float *value, int count);
    void setUniformVec4(const char *name, float *value, int count);
    void setUniformMat3(const char *name, float *value, int count);
    void setUniformMat4(const char *name, float *value, int count);
    void setUniformInt(const char *name, int *value, int count);

    VkDevice m_device;
    VkShaderModule m_vertex;
    VkShaderModule m_fragment;
    VulkanShaderDelegate *m_delegate;

    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;
private:
};

} // namespace Bird
