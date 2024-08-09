#include <thread>
#include "BlocksCreation.hpp"
#include "ChunksRenderer.hpp"

ChunksRenderer::ChunksRenderer() {
    chunksStorage = NEW(Foundation::getAllocator(), ChunksStorage);
    LOG_INFO("WORLD GENERATED");
    blocksCreation = NEW(Foundation::getAllocator(), BlocksCreation);
    blocksCreation->init();
    blocksCreation->setCamera(Nest::Application::get()->getWorldCamera());
    blocksCreation->setChunksStorage(chunksStorage);
}

ChunksRenderer::~ChunksRenderer() {
    delete blocksCreation;
    delete chunksStorage;
}

void ChunksRenderer::onAttach() {
    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Nest::DynamicMesh *mesh =
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

void ChunksRenderer::onUpdate(double deltaTime) {
    /*if (Nest::Events::isJustKeyPressed(Key::Q)) {
        auto *data = NEW_ARRAY(Foundation::getAllocator(), unsigned char, ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ);
        chunksStorage->saveWorld(data);
        if (!NestFiles::writeBinaryFile(
                "world.bin", (const char *)data, ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ
            )) {
            LOG_ERROR("WORLD::NOT_SAVED");
        }
        delete[] data;
    }
    if (Nest::Events::isJustKeyPressed(Key::E)) {
        auto *data = NEW_ARRAY(Foundation::getAllocator(), unsigned char, ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ);
        if (!NestFiles::readBinaryFile(
                "world.bin", (char *)data, ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ
            )) {
            LOG_ERROR("WORLD::NOT_LOAD");
        }
        chunksStorage->loadWorld(data);
        delete[] data;
        init();
    }
     */
    blocksCreation->update(deltaTime);
    draw();
}

void ChunksRenderer::draw() {
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        chunksStorage->chunks[i].getMesh()->draw();
    }
}
