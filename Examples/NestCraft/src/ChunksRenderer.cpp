#include "ChunksRenderer.hpp"

ChunksRenderer::ChunksRenderer(uint32_t w, uint32_t h, uint32_t d) : w(w), h(h), d(d) {}

ChunksRenderer::~ChunksRenderer() {
    delete chunkManager;
    delete[] meshes;
}

void ChunksRenderer::init() {
    chunkManager = new ChunkManager(w, h, d);
    meshes = new Mesh*[chunkManager->getSize()];
    for (int i = 0; i < chunkManager->getSize(); ++i) {
        Chunk *chunk = chunkManager->getChunkIndex(i);
        if (!chunk) {
            std::cout <<"Error:Chunk IS NULL\n";
            continue;
        }
//        std::cout << "Chunk " << i << " generating" << std::endl;
        Mesh *mesh = chunkMeshGenerator.generateMesh(chunk);
        mesh->addTexture("/home/bogdan/Projects/Nest/Examples/NestCraft/res/textures/BlocksTile.png");
        meshes[i] = mesh;
    }
}

void ChunksRenderer::draw() {
    for (int i = 0; i < chunkManager->getSize(); ++i) {
        Chunk *chunk = chunkManager->getChunkIndex(i);
        if (!chunk) {
            std::cout << "\nError:Chunk IS NULL\n";
            continue;
        }
        Mesh *mesh = meshes[i];
        mesh->draw();
//            std::cout << "Chunk " << i << " draw: " << chunk->getPosX() * chunkManager->getSizeX() << " " <<
//                chunk->getPosY() * chunkManager->getSizeY() << " " <<
//                chunk->getPosZ() * chunkManager->getSizeZ() << std::endl;
    }
}
