#include "ChunksRenderer.hpp"

ChunksRenderer::ChunksRenderer(uint32_t w, uint32_t h, uint32_t d) : w(w), h(h), d(d) {}

ChunksRenderer::~ChunksRenderer() {
    delete chunksStorage;
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        delete meshes[i];
    }
    delete[] meshes;
}

void ChunksRenderer::init() {
    chunksStorage = new ChunksStorage();
    meshes = new Mesh*[ChunksStorage::SIZE_XYZ];
    std::cout << "WORLD GENERATED!" << std::endl;
    int index = 0;
    for (int x = 0; x < ChunksStorage::SIZE_X; ++x) {
        for (int y = 0; y < ChunksStorage::SIZE_Y; ++y) {
            for (int z = 0; z < ChunksStorage::SIZE_Z; ++z) {
                Mesh *mesh = chunkMeshGenerator.generateMesh(chunksStorage, x, y, z);
                mesh->addTexture("/home/bogdan/Projects/Nest/Examples/NestCraft/res/textures/BlocksTile.png");
                meshes[index++] = mesh;
            }
        }
    }
}

void ChunksRenderer::draw() {
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        Chunk *chunk = chunksStorage->getChunkIndex(i);
        if (!chunk) {
            std::cout << "\nError:Chunk IS NULL\n";
            continue;
        }
        meshes[i]->draw();
//            std::cout << "Chunk " << i << " draw: " << chunk->getPosX() * chunkManager->getSizeX() << " " <<
//                chunk->getPosY() * chunkManager->getSizeY() << " " <<
//                chunk->getPosZ() * chunkManager->getSizeZ() << std::endl;
    }
}