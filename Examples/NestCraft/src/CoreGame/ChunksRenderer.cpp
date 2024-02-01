#include <thread>
#include "BlocksCreation.hpp"
#include "ChunksRenderer.hpp"

ChunksRenderer::~ChunksRenderer() {
    delete blocksCreation;
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        delete chunksStorage->getChunkIndex(i)->getMesh();
    }
    delete chunksStorage;
}

void ChunksRenderer::init() {
    chunksStorage = new ChunksStorage();
    std::cout << "WORLD GENERATED" << std::endl;
    blocksCreation = new BlocksCreation();
    blocksCreation->init();
    blocksCreation->setCamera(Application::getInstance()->getCamera());
    blocksCreation->setChunksStorage(chunksStorage);
    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Mesh *mesh = ChunkMeshGenerator::generateMesh(chunksStorage,indexX,
                                                              indexY, indexZ, true);
                chunksStorage
                        ->chunks[indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                                 indexX * ChunksStorage::SIZE_X + indexZ]
                        .setMesh(mesh);
                chunksStorage
                        ->chunks[indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                                 indexX * ChunksStorage::SIZE_X + indexZ]
                        .getMesh()->addTexture("Examples/NestCraft/res/textures/BlocksTile.png");
            }
        }
    }
    std::cout << "MESHES GENERATED" << std::endl;
}

void ChunksRenderer::update(double deltaTime) {
    blocksCreation->update(deltaTime);
    draw();
}

void ChunksRenderer::draw() {
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        chunksStorage->chunks[i].getMesh()->draw();
    }
}
