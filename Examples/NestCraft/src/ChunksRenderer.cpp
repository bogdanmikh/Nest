#include <thread>

#include "ChunksRenderer.hpp"

ChunksRenderer::~ChunksRenderer() {
    delete chunksStorage;
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        delete meshes[i];
    }
    delete[] meshes;
}

void ChunksRenderer::init() {
    chunksStorage = new ChunksStorage();
    std::cout << "WORLD GENERATED" << std::endl;
    meshes = new Mesh*[ChunksStorage::SIZE_XYZ];
    int index = 0;

    const uint32_t sizeThreads = ChunksStorage::SIZE_XYZ;

//    std::thread threads[sizeThreads];

    for (int x = 0; x < ChunksStorage::SIZE_X; ++x) {
        for (int y = 0; y < ChunksStorage::SIZE_Y; ++y) {
            for (int z = 0; z < ChunksStorage::SIZE_Z; ++z) {
//                threads[index] = std::thread([&]() {
                    Mesh* mesh = chunkMeshGenerator.generateMesh(chunksStorage, x, y, z, true);
                    mesh->addTexture("/home/bogdan/Projects/Nest/Examples/NestCraft/res/textures/BlocksTile.png");
                    meshes[index] = mesh;
//                });
                index++;
            }
        }
    }
    std::cout << "MESHES GENERATED" << std::endl;
//    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
//        threads[i].join();
//    }
}

void ChunksRenderer::draw() {
    const uint32_t sizeThreads = ChunksStorage::SIZE_XYZ;

    std::thread threads[sizeThreads];
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        Chunk *chunk = chunksStorage->getChunkIndex(i);
        if (!chunk) {
            std::cout << "\nError:Chunk IS NULL\n";
            continue;
        }
//        threads[i] = std::thread([&]() {
            meshes[i]->draw();
//        });
    }
//    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
//        threads[i].join();
//    }
}