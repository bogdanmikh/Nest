#include "Panda/Renderer/Renderer2D.hpp"

namespace Panda {

Renderer2D::Renderer2D()
    : m_viewId(0)
    , m_viewProj(1.f)
    , m_drawData() {
    m_drawData.vbSize = 0;
    m_drawData.indicesCount = 0;
    m_drawData.vertices =
        (Vertex2D *)ALLOC(Foundation::getAllocator(), sizeof(Vertex2D) * MAX_VERTICES_COUNT);
    m_drawData.indices =
        (uint16_t *)ALLOC(Foundation::getAllocator(), sizeof(uint16_t) * MAX_INDICES_COUNT);
    Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
        "default-shaders/renderer2d/renderer2d_vertex.glsl",
        "default-shaders/renderer2d/renderer2d_fragment.glsl"
    );
    m_drawData.shader = Miren::createProgram(programAsset.getMirenProgramCreate());
    Miren::VertexBufferLayoutData layoutData;
    // Position
    layoutData.pushVec3();
    // Texture coordinates
    layoutData.pushVec2();
    // Texture Index
    layoutData.pushFloat(1);
    // Color
    layoutData.pushVec4();
    m_drawData.layout = Miren::createVertexLayout(layoutData);
    m_drawData.textureSlotIndex = 1;
    Foundation::Memory whiteTextureData = Foundation::Memory::alloc(sizeof(uint32_t));
    *(uint32_t *)whiteTextureData.data = 0xffffffff;
    m_drawData.whiteTexture = Foundation::makeShared<Texture>(whiteTextureData, 1, 1);
    m_drawData.textures[0] = m_drawData.whiteTexture;
    for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++) {
        m_drawData.samplers[i] = i;
    }
    Miren::setUniform(
        m_drawData.shader, "u_textures", m_drawData.samplers, Miren::UniformDataType::IntArray
    );
}

Renderer2D::~Renderer2D() {
    if (m_drawData.shader.isValid()) {
        Miren::deleteProgram(m_drawData.shader);
    }
    if (m_drawData.layout.isValid()) {
        Miren::deleteVertexLayout(m_drawData.layout);
    }
    for (int i = 0; i < MAX_TEXTURE_SLOTS; ++i) {
        m_drawData.textures[i] = nullptr;
    }
    m_drawData.whiteTexture = nullptr;
    if (m_drawData.vertices) {
        FREE(Foundation::getAllocator(), m_drawData.vertices);
    }
    if (m_drawData.indices) {
        FREE(Foundation::getAllocator(), m_drawData.indices);
    }
}

void Renderer2D::begin() {
    m_drawData.stats.quadCount = 0;
    m_drawData.stats.drawCalls = 0;
    m_drawData.verticesCount = 0;
    m_drawData.vbSize = 0;
    m_drawData.indicesCount = 0;
    m_drawData.ibSize = 0;
    m_drawData.textureSlotIndex = 1;
}

void Renderer2D::drawRect(RectData rect) {
    glm::mat4 identity = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(rect.center.x, rect.center.y, rect.center.z);
    glm::vec3 scale = glm::vec3(rect.size.width, rect.size.height, 1.f);
    glm::mat4 transform =
        glm::translate(identity, position) *
        glm::rotate(identity, glm::radians(rect.rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::scale(identity, scale);
    drawRect(transform, rect);
}

void Renderer2D::drawRect(glm::mat4 &transform, RectData rect) {
    uint16_t indicesOffset = m_drawData.verticesCount;
    uint32_t &verticesCount = m_drawData.verticesCount;
    glm::vec4 positions[4];
    positions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    positions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    positions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    positions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
    glm::vec2 textureCoords[4];
    textureCoords[0] = {0.0f, 0.0f};
    textureCoords[1] = {1.0f, 0.0f};
    textureCoords[2] = {1.0f, 1.0f};
    textureCoords[3] = {0.0f, 1.0f};

    float textureIndex = 0.0f;
    if (rect.texture != nullptr) {
        for (uint32_t i = 1; i < m_drawData.textureSlotIndex; i++) {
            if ((*rect.texture).getHandle().id == (*m_drawData.textures[i]).getHandle().id) {
                textureIndex = (float)i;
                break;
            }
        }
        if (textureIndex == 0.0f) {
            // TODO: Check if next batch is needed
            textureIndex = (float)m_drawData.textureSlotIndex;
            m_drawData.textures[m_drawData.textureSlotIndex] = rect.texture;
            m_drawData.textureSlotIndex++;
        }
    }

    for (int i = 0; i < 4; i++) {
        m_drawData.vertices[verticesCount].pos = rect.transform * transform * positions[i];
        m_drawData.vertices[verticesCount].textureIndex = textureIndex;
        m_drawData.vertices[verticesCount].color = rect.color;
        m_drawData.vertices[verticesCount].textureCoords = textureCoords[i];
        verticesCount++;
    }
    uint32_t &indicesCount = m_drawData.indicesCount;
    m_drawData.indices[indicesCount++] = indicesOffset + 0;
    m_drawData.indices[indicesCount++] = indicesOffset + 1;
    m_drawData.indices[indicesCount++] = indicesOffset + 2;
    m_drawData.indices[indicesCount++] = indicesOffset + 2;
    m_drawData.indices[indicesCount++] = indicesOffset + 3;
    m_drawData.indices[indicesCount++] = indicesOffset + 0;

    m_drawData.vbSize += sizeof(Vertex2D) * 4;
    m_drawData.ibSize += sizeof(uint16_t) * 6;
    PND_ASSERT(verticesCount < MAX_VERTICES_COUNT, "VERTICES LIMIT REACHED");
    PND_ASSERT(indicesCount < MAX_INDICES_COUNT, "INDICES LIMIT REACHED");
    m_drawData.stats.quadCount += 1;
}

Renderer2D::Statistics Renderer2D::getStats() {
    return m_drawData.stats;
}

void Renderer2D::end() {
    if (m_drawData.verticesCount == 0) {
        return;
    }
    Miren::setShader(m_drawData.shader);
    Miren::setUniform(
        m_drawData.shader, "projViewMtx", (void *)&m_viewProj, Miren::UniformDataType::Mat4
    );

    Miren::TransientVertexBuffer tvb;
    Miren::allocTransientVertexBuffer(&tvb, m_drawData.vbSize);
    memcpy(tvb.data, m_drawData.vertices, m_drawData.vbSize);

    Miren::TransientIndexBuffer tib;
    Miren::allocTransientIndexBuffer(
        &tib, m_drawData.indicesCount, Miren::BufferElementType::UnsignedShort
    );
    memcpy(tib.data, m_drawData.indices, m_drawData.ibSize);

    Miren::setState(0);
    for (int i = 0; i < m_drawData.textureSlotIndex; i++) {
        Miren::setTexture(m_drawData.textures[i]->getHandle(), i);
    }
    Miren::setVertexLayout(m_drawData.layout);
    Miren::setVertexBuffer(tvb.handle, tvb.startVertex);
    Miren::setIndexBuffer(tib.handle, tib.startIndex, m_drawData.indicesCount);
    Miren::submit(m_viewId);
}

void Renderer2D::setViewId(Miren::ViewId id) {
    m_viewId = id;
}

void Renderer2D::setViewProj(glm::mat4 viewProj) {
    m_viewProj = viewProj;
}

} // namespace Panda
