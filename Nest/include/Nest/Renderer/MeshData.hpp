#pragma once

#include <glm/glm.hpp>
#include <NestRen/NestRen.hpp>

namespace Nest {

struct MeshData {
    NestRen::VertexLayoutHandle layoutHandle;
    Foundation::Memory vertexBuffer;
    uint32_t vertexBufferSize;
    Foundation::Memory indices;
    uint32_t indicesCount;

    MeshData(
        NestRen::VertexLayoutHandle layoutHandle,
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

} // namespace Panda