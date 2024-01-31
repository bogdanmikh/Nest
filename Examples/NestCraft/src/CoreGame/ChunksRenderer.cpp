#include <thread>
#include "BlocksCreation.hpp"
#include "ChunksRenderer.hpp"

ChunksRenderer::~ChunksRenderer() {
    delete blocksCreation;
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
    blocksCreation = new BlocksCreation();
    blocksCreation->init();
    blocksCreation->setCamera(Application::getInstance()->getCamera());
    blocksCreation->setChunksStorage(chunksStorage);
    int index = 0;

    for (int x = 0; x < ChunksStorage::SIZE_X; ++x) {
        for (int y = 0; y < ChunksStorage::SIZE_Y; ++y) {
            for (int z = 0; z < ChunksStorage::SIZE_Z; ++z) {
                Mesh* mesh = ChunkMeshGenerator::generateMesh(chunksStorage, x, y, z, true);
                mesh->addTexture("/home/bogdan/Projects/Nest/Examples/NestCraft/res/textures/BlocksTile.png");
                meshes[index] = mesh;
                index++;
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
        meshes[i]->draw();
    }
}
