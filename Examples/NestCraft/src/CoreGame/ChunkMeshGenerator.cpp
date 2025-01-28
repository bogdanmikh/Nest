#include "ChunkMeshGenerator.hpp"

#include <iostream>

namespace ChunkMeshGenerator {

glm::vec2 getUV(uint8_t tileIndex) {
    float uvSize = 1.0f / 16.f;
    float u = tileIndex % 16 * uvSize;
    float v = tileIndex / 16 * uvSize;
    u += 0.0005f;
    v += 0.0005f;
    return {u, v};
}

Nest::StaticMesh *generateMesh(
    Bird::TextureHandle textureHandle,
    Bird::ProgramHandle programHandle,
    ChunksStorage *chunksStorage,
    int chunkIndexX,
    int chunkIndexY,
    int chunkIndexZ,
    bool ambientOcclusion
) {
    Chunk &chunk = chunksStorage->chunks
                       [chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                        chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ];
    Vertex *vertices = (Vertex *)F_ALLOC(
        Foundation::getAllocator(),
        sizeof(Vertex) * Chunk::SIZE_X * Chunk::SIZE_Y * Chunk::SIZE_Z * 24
    );
    for (int i = 0; i < Chunk::SIZE_X * Chunk::SIZE_Y * Chunk::SIZE_Z * 24; ++i) {
        vertices[i] = Vertex();
    }
    uint32_t *indices = (uint32_t *)F_ALLOC(
        Foundation::getAllocator(),
        sizeof(uint32_t) * Chunk::SIZE_X * Chunk::SIZE_Y * Chunk::SIZE_Z * 36
    );
    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;
    for (int voxelIndexX = 0; voxelIndexX < Chunk::SIZE_X; voxelIndexX++) {
        for (int voxelIndexY = 0; voxelIndexY < Chunk::SIZE_Y; voxelIndexY++) {
            for (int voxelIndexZ = 0; voxelIndexZ < Chunk::SIZE_Z; voxelIndexZ++) {
                int x = voxelIndexX + chunkIndexX * Chunk::SIZE_X;
                int y = voxelIndexY + chunkIndexY * Chunk::SIZE_Y;
                int z = voxelIndexZ + chunkIndexZ * Chunk::SIZE_Z;

                Voxel *currentVoxel = chunk.get(voxelIndexX, voxelIndexY, voxelIndexZ);
                if (currentVoxel == nullptr || currentVoxel->isAir()) {
                    continue;
                }

                float uvSize = 1.f / 16.f;
                uvSize -= 0.001f;
                glm::vec2 uv = getUV(uint8_t(currentVoxel->type));

                float light;
                float a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;

                // Front
                if (isAir(x, y, z + 1, chunksStorage)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 1.0f;
                    if (ambientOcclusion) {
                        // top
                        a = (isAir(x, y + 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom
                        b = (isAir(x, y - 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // left
                        c = (isAir(x + 1, y, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right
                        d = (isAir(x - 1, y, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top left
                        e = (isAir(x + 1, y + 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top right
                        f = (isAir(x - 1, y + 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom left
                        g = (isAir(x + 1, y - 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom right
                        h = (isAir(x - 1, y - 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }

                    vertices[verticesCount++] =
                        Vertex(x, y, z + 1.0f, uv.x, uv.y + uvSize, light * (1.f - b - d - h));
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        light * (1.f - b - c - g)
                    ); // 1
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y,
                        light * (1.f - a - c - e)
                    ); // 2
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x,
                        uv.y,
                        light * (1.f - a - d - f)
                    ); // 3
                }
                // Back
                if (isAir(x, y, z - 1, chunksStorage)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.75f;
                    if (ambientOcclusion) {
                        // top
                        a = (isAir(x, y + 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom
                        b = (isAir(x, y - 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right
                        c = (isAir(x - 1, y, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // left
                        d = (isAir(x + 1, y, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top right
                        e = (isAir(x - 1, y + 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom right
                        f = (isAir(x - 1, y - 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top left
                        g = (isAir(x + 1, y + 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom left
                        h = (isAir(x + 1, y - 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }

                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        light * (1.f - b - c - f)
                    ); // 4
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z,
                        uv.x + uvSize,
                        uv.y,
                        light * (1.f - a - c - e)
                    ); // 5
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z,
                        uv.x,
                        uv.y,
                        light * (1.f - a - d - g)
                    ); // 6
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z,
                        uv.x,
                        uv.y + uvSize,
                        light * (1.f - b - d - h)
                    ); // 7
                }
                // Top
                if (isAir(x, y + 1, z, chunksStorage)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.95f;
                    if (ambientOcclusion) {
                        // left
                        a = (isAir(x + 1, y + 1, z, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right
                        b = (isAir(x - 1, y + 1, z, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // back
                        c = (isAir(x, y + 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // front
                        d = (isAir(x, y + 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // left back
                        e = (isAir(x + 1, y + 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // left front
                        f = (isAir(x + 1, y + 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right back
                        g = (isAir(x - 1, y + 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right front
                        h = (isAir(x - 1, y + 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z,
                        uv.x,
                        uv.y + uvSize,
                        light * (1.f - b - d - h)
                    ); // 8
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        light * (1.f - b - c - g)
                    ); // 11
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y,
                        light * (1.f - a - c - e)
                    ); // 10
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z,
                        uv.x,
                        uv.y,
                        light * (1.f - a - d - f)
                    ); // 9
                }
                // Bottom
                if (isAir(x, y - 1, z, chunksStorage)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.85f;
                    if (ambientOcclusion) {
                        // left
                        a = (isAir(x + 1, y - 1, z, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right
                        b = (isAir(x - 1, y - 1, z, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // back
                        c = (isAir(x, y - 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // front
                        d = (isAir(x, y - 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // left back
                        e = (isAir(x + 1, y - 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // left front
                        f = (isAir(x + 1, y - 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right back
                        g = (isAir(x - 1, y - 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right front
                        h = (isAir(x - 1, y - 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }
                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z,
                        uv.x,
                        uv.y + uvSize,
                        light * (1.f - b - d - h)
                    ); // 12
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        light * (1.f - a - d - f)
                    ); // 13
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y,
                        light * (1.f - a - c - e)
                    ); // 14
                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z + 1.0f,
                        uv.x,
                        uv.y,
                        light * (1.f - b - c - g)
                    ); // 15
                }
                // Right
                if (isAir(x - 1, y, z, chunksStorage)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.9f;
                    if (ambientOcclusion) {
                        // top
                        a = (isAir(x - 1, y + 1, z, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom
                        b = (isAir(x - 1, y - 1, z, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // front
                        c = (isAir(x - 1, y, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // back
                        d = (isAir(x - 1, y, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top front
                        e = (isAir(x - 1, y + 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom front
                        f = (isAir(x - 1, y - 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top back
                        g = (isAir(x - 1, y + 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom back
                        h = (isAir(x - 1, y - 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }

                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z,
                        uv.x,
                        uv.y + uvSize,

                        light * (1.f - b - d - h)
                    ); // 16
                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y + uvSize,

                        light * (1.f - b - c - f)
                    ); // 17
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y,

                        light * (1.f - a - c - e)
                    ); // 18
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z,
                        uv.x,
                        uv.y,

                        light * (1.f - a - d - g)
                    ); // 19
                }
                // Left
                if (isAir(x + 1, y, z, chunksStorage)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.8f;
                    if (ambientOcclusion) {
                        // top
                        a = (isAir(x + 1, y + 1, z, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom
                        b = (isAir(x + 1, y - 1, z, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // front
                        c = (isAir(x + 1, y, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // back
                        d = (isAir(x + 1, y, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top front
                        e = (isAir(x + 1, y + 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top back
                        f = (isAir(x + 1, y + 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom front
                        g = (isAir(x + 1, y - 1, z + 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom back
                        h = (isAir(x + 1, y - 1, z - 1, chunksStorage) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }

                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z,
                        uv.x + uvSize,
                        uv.y + uvSize,

                        light * (1.f - b - d - h)
                    ); // 20
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z,
                        uv.x + uvSize,
                        uv.y,

                        light * (1.f - a - d - f)
                    ); // 23
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x,
                        uv.y,

                        light * (1.f - a - c - e)
                    ); // 22
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z + 1.0f,
                        uv.x,
                        uv.y + uvSize,

                        light * (1.f - b - c - g)
                    ); // 21
                }
            }
        }
    }

    Foundation::Memory verticesMemory = Foundation::Memory(vertices);
    Foundation::Memory indicesMemory = Foundation::Memory(indices);

    Bird::VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec2();
    layoutData.pushFloat(1);

    Bird::VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);

    Nest::MeshData meshData(
        vertexLayout, verticesMemory, verticesCount * sizeof(Vertex), indicesMemory, indicesCount
    );
    auto *mesh = F_NEW(Foundation::getAllocator(), Nest::StaticMesh);
    mesh->create(meshData, {"texture1", textureHandle}, programHandle);
    return mesh;
}

bool isAir(int localX, int localY, int localZ, ChunksStorage *chunksStorage) {
    Voxel *voxel = chunksStorage->getVoxel(localX, localY, localZ);
    if (voxel == nullptr) {
        return true;
    }
    return voxel->isAir();
}

void addFaceIndices(uint32_t offset, uint32_t &indicesCount, uint32_t *indices) {
    indices[indicesCount++] = offset;
    indices[indicesCount++] = offset + 1;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 3;
    indices[indicesCount++] = offset;
}

} // namespace ChunkMeshGenerator