//
// Created by Bogdan on 29.09.2024.
//

#pragma once

#include <Nest.hpp>

class PostprocessingEffect : public Nest::Entity {
public:
    PostprocessingEffect();
    void setFBTexture(Bird::TextureHandle textureHandle);
    void setViewId(Bird::ViewId viewId);
    void setPathToShaders(const std::string &vertexPath, const std::string &fragmentPath);
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;

private:
    std::string m_vertexPath;
    std::string m_fragmentPath;

    Bird::ViewId m_viewId;
    Bird::TextureHandle m_textureHandle;
    Bird::VertexBufferHandle m_vertexBuffer;
    Bird::IndexBufferHandle m_indexBuffer;
    Bird::ProgramHandle m_shader;

    // uniforms
    glm::vec2 mousePos;
    glm::vec2 resolution;
    float time;

    int slot = 0;
};
