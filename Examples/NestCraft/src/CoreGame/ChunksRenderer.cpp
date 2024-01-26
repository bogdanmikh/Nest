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

void ChunksRenderer::draw(Window* window, Camera *camera) {
    const uint32_t sizeThreads = ChunksStorage::SIZE_XYZ;

//    std::thread threads[sizeThreads];
    std::optional<VoxelRaycastData> voxelRaycastData = chunksStorage->bresenham3D(
            camera->getPosition().x,
            camera->getPosition().y,
            camera->getPosition().z,
            camera->getFront().x,
            camera->getFront().y,
            camera->getFront().z, 10);
    bool canCreateBlock = window->isKeyPressed(Key::E) && !lastDownE;
    if (!window->isKeyPressed(Key::E)) lastDownE = false;
    else lastDownE = true;

    bool canDeleteBlock = window->isKeyPressed(Key::Q) && !lastDownQ;
    if (!window->isKeyPressed(Key::Q)) lastDownQ = false;
    else lastDownQ = true;

    if (canCreateBlock && voxelRaycastData.has_value()) {
        std::cout << "Block created!" << std::endl;
        chunksStorage->getChunk(
        int(voxelRaycastData->end.x + voxelRaycastData->normal.x),
        int(voxelRaycastData->end.y + voxelRaycastData->normal.y),
        int(voxelRaycastData->end.z + voxelRaycastData->normal.z))->needUpdate = true;
        chunksStorage->setVoxel(int(voxelRaycastData->end.x + voxelRaycastData->normal.x),
                                int(voxelRaycastData->end.y + voxelRaycastData->normal.y),
                                int(voxelRaycastData->end.z + voxelRaycastData->normal.z), VoxelType(11));
    } else if (canDeleteBlock && voxelRaycastData.has_value()) {
        std::cout << "Block deleted!" << std::endl;
        chunksStorage->getChunk(
                int(voxelRaycastData->end.x + voxelRaycastData->normal.x),
                int(voxelRaycastData->end.y + voxelRaycastData->normal.y),
                int(voxelRaycastData->end.z + voxelRaycastData->normal.z))->needUpdate = true;
        chunksStorage->setVoxel(voxelRaycastData->end.x,
                                voxelRaycastData->end.y,
                                voxelRaycastData->end.z, VoxelType::NOTHING);
    }
    int index = 0;

//    std::thread threads[sizeThreads];

    for (int x = 0; x < ChunksStorage::SIZE_X; ++x) {
        for (int y = 0; y < ChunksStorage::SIZE_Y; ++y) {
            for (int z = 0; z < ChunksStorage::SIZE_Z; ++z) {
                Chunk *chunk = chunksStorage->getChunk(x * Chunk::SIZE_X,
                                                       y * Chunk::SIZE_Y,
                                                       z * Chunk::SIZE_Z);
                if (chunk->needUpdate) {
                    delete meshes[index];
                    meshes[index] = chunkMeshGenerator.generateMesh(chunksStorage, x, y, z, true);
                    chunk->needUpdate = false;
                }
                meshes[index]->draw();
                index++;
            }
        }
    }
//    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
//        threads[i].join();
//    }
}