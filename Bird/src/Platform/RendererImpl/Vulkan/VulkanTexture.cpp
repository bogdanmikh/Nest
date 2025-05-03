//
// Created by Admin on 11.02.2022.
//

#include "VulkanTexture.hpp"

namespace Bird {

VulkanTexture::VulkanTexture()
    : m_id(-1)
    , m_create()
    , m_format(0)
    , m_type(0) {}

void VulkanTexture::create(TextureCreate &create) {}

void VulkanTexture::resize(uint32_t width, uint32_t height) {}

void VulkanTexture::terminate() {}

void VulkanTexture::bind(unsigned int slot) {}

void VulkanTexture::unbind() {}

void VulkanTexture::readPixels(void *data) {}

} // namespace Bird
