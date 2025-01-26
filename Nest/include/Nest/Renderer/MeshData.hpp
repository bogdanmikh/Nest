#pragma once

#include <Bird/Bird.hpp>
#include <glm/glm.hpp>

namespace Nest {

struct MeshData {
    Bird::VertexLayoutHandle layoutHandle;
    Foundation::Memory vertexBuffer;
    uint32_t vertexBufferSize;
    Foundation::Memory indices;
    uint32_t indicesCount;

    MeshData(
        Bird::VertexLayoutHandle layoutHandle,
        Foundation::Memory vertexBuffer,
        uint32_t vertexBufferSize,
        Foundation::Memory indices,
        uint32_t indicesCount
    )
        : layoutHandle(layoutHandle)
        , vertexBuffer(vertexBuffer)
        , vertexBufferSize(vertexBufferSize)
        , indices(indices)
        , indicesCount(indicesCount) {}
};

} // namespace Nest