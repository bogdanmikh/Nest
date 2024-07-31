#include <thread>
#include "BlocksCreation.hpp"
#include "ChunksRenderer.hpp"

ChunksRenderer::ChunksRenderer() {
    chunksStorage = new ChunksStorage();
    LOG_INFO("WORLD GENERATED");
    blocksCreation = new BlocksCreation();
    blocksCreation->init();
    blocksCreation->setCamera(Application::getInstance()->getCamera());
    blocksCreation->setChunksStorage(chunksStorage);
}

ChunksRenderer::~ChunksRenderer() {
    delete blocksCreation;
    delete chunksStorage;
}

void ChunksRenderer::init() {
    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Mesh *mesh =
                    ChunkMeshGenerator::generateMesh(chunksStorage, indexX, indexY, indexZ, true);
                chunksStorage
                    ->chunks
                        [indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                         indexX * ChunksStorage::SIZE_X + indexZ]
                    .setMesh(mesh);
                chunksStorage
                    ->chunks
                        [indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                         indexX * ChunksStorage::SIZE_X + indexZ]
                    .getMesh()
                    ->addTexture("Textures/BlocksTile.png");
            }
        }
    }
    LOG_INFO("MESHES GENERATED");
}

void ChunksRenderer::update(double deltaTime) {
    if (Events::isJustKeyPressed(Key::Q)) {
        auto *data = new unsigned char[ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ];
        chunksStorage->saveWorld(data);
        if (!NestFiles::writeBinaryFile(
                "world.bin", (const char *)data, ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ
            )) {
            LOG_ERROR("WORLD::NOT_SAVED");
        }
        delete[] data;
    }
    if (Events::isJustKeyPressed(Key::E)) {
        auto *data = new unsigned char[ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ];
        if (!NestFiles::readBinaryFile(
                "world.bin", (char *)data, ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ
            )) {
            LOG_ERROR("WORLD::NOT_LOAD");
        }
        chunksStorage->loadWorld(data);
        delete[] data;
        init();
    }
    blocksCreation->update(deltaTime);
    draw();
}

void ChunksRenderer::draw() {
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        chunksStorage->chunks[i].getMesh()->draw();
    }
}
