#include <thread>
#include "BlocksCreation.hpp"
#include "ChunksRenderer.hpp"

ChunksRenderer::~ChunksRenderer() {}

void ChunksRenderer::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vst.glsl", "Shaders/fst.glsl");
    m_shader = createProgram(programAsset.getBirdProgramCreate());

    m_chunksStorage = NEW(Foundation::getAllocator(), ChunksStorage);
    LOG_INFO("WORLD GENERATED");
    m_blocksCreation = NEW(Foundation::getAllocator(), BlocksCreation);
    m_blocksCreation->init(m_shader);
    m_blocksCreation->setCamera(Nest::Application::get()->getWorldCamera());
    m_blocksCreation->setChunksStorage(m_chunksStorage);

    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Nest::StaticMesh *mesh =
                    ChunkMeshGenerator::generateMesh(m_shader, m_chunksStorage, indexX, indexY, indexZ, true);
                m_chunksStorage
                    ->chunks
                        [indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                         indexX * ChunksStorage::SIZE_X + indexZ]
                    .setMesh(mesh);
            }
        }
    }
    LOG_INFO("MESHES GENERATED");
}

void ChunksRenderer::onUpdate(double deltaTime) {
    /*if (Nest::Events::isJustKeyPressed(Key::Q)) {
        auto *data = NEW_ARRAY(Foundation::getAllocator(), unsigned char, ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ);
        m_chunksStorage->saveWorld(data);
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
        m_chunksStorage->loadWorld(data);
        delete[] data;
        init();
    }
     */
    m_blocksCreation->update(deltaTime);
    draw();
}

void ChunksRenderer::draw() {
    m_renderer3D.begin();
    static Nest::TransformComponent transform;
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        m_renderer3D.submit(&transform, m_chunksStorage->chunks[i].getMesh());
    }
    m_renderer3D.end();
}
void ChunksRenderer::onDetach() {
    FREE(Foundation::getAllocator(), m_blocksCreation);
    FREE(Foundation::getAllocator(), m_chunksStorage);
}
