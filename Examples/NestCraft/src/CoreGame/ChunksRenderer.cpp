#include "ChunksRenderer.hpp"
#include "BlocksCreation.hpp"

ChunksRenderer::~ChunksRenderer() {}

void ChunksRenderer::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vst.glsl", "Shaders/fst.glsl");
    m_shader = createProgram(programAsset.getBirdProgramCreate());

    Nest::TextureAsset textureAsset = Nest::AssetLoader::loadTexture("Textures/BlocksTile.png");

    Bird::TextureCreate textureCreate = textureAsset.getBirdTextureCreate();
    textureCreate.m_numMips = 4;
    textureCreate.m_minFiltering = Bird::NEAREST_MIPMAP_LINEAR;
    textureCreate.m_magFiltering = Bird::NEAREST;
    m_texture = createTexture(textureCreate);

    m_chunksStorage = F_NEW(Foundation::getAllocator(), ChunksStorage);
    LOG_INFO("WORLD GENERATED");
    m_blocksCreation = F_NEW(Foundation::getAllocator(), BlocksCreation);
    m_blocksCreation->init(m_shader, m_texture);
    m_blocksCreation->setCamera(Nest::Application::get()->getWorldCamera());
    m_blocksCreation->setChunksStorage(m_chunksStorage);

    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {

                Nest::StaticMesh *mesh;
                mesh = ChunkMeshGenerator::generateMesh(
                    m_texture, m_shader, m_chunksStorage, indexX, indexY, indexZ, true
                );
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
    /*if (Events->isJustKeyPressed(Key::Q)) {
        auto *data = NEW_ARRAY(Foundation::getAllocator(), unsigned char,
    ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ); m_chunksStorage->saveWorld(data);
    if (!NestFiles::writeBinaryFile( "world.bin", (const char *)data,
    ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ
            )) {
            LOG_ERROR("WORLD::NOT_SAVED");
        }
        delete[] data;
    }
    if (Events->isJustKeyPressed(Key::E)) {
        auto *data = NEW_ARRAY(Foundation::getAllocator(), unsigned char,
    ChunksStorage::SIZE_XYZ * Chunk::SIZE_XYZ); if (!NestFiles::readBinaryFile(
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
    static auto camera = Nest::Application::get()->getWorldCamera();
    m_renderer3D.setViewProj(camera->getProjectionMatrix() * camera->getViewMatrix());
    m_renderer3D.begin();
    static Nest::TransformComponent transform;
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        //        Bird::setShader(m_shader);
        Bird::setShader(m_chunksStorage->chunks[i].getMesh()->getShaderHandle());

        static auto time = Nest::Application::get()->getWindow()->getTime();
        time = Nest::Application::get()->getWindow()->getTime();
        static auto mousePos = {Nest::Input::getMousePositionX(), Nest::Input::getMousePositionY()};
        mousePos = {Nest::Input::getMousePositionX(), Nest::Input::getMousePositionY()};
        static auto resolution = Nest::Application::get()->getWindow()->getSize();
        resolution = Nest::Application::get()->getWindow()->getSize();

        static auto projViewMtx = camera->getProjectionMatrix() * camera->getViewMatrix();
        projViewMtx = camera->getProjectionMatrix() * camera->getViewMatrix();

        static auto cameraPos = camera->getPosition();
        cameraPos = camera->getPosition();

        static auto model = transform.getTransform();
        model = transform.getTransform();

        Bird::setUniform(m_shader, "iTimeVec4", &time,
                         Bird::UniformType::Vec4); /// float
        Bird::setUniform(
            m_shader,
            "iResolutionVec4",
            &resolution,
            Bird::UniformType::Vec4
        ); /// vec2
        Bird::setUniform(m_shader, "iMouseVec4", &mousePos,
                         Bird::UniformType::Vec4); /// vec2
        Bird::setUniform(m_shader, "iCameraPosVec4", &cameraPos, Bird::UniformType::Vec4);
        static glm::vec3 color;
        color = m_menu.getColor();
        //        LOG_INFO("Color: {}, {}, {}", color.x, color.y, color.z);
        Bird::setUniform(m_shader, "iColorVec4", &color, Bird::UniformType::Vec4);
        //        Bird::setTexture();
        m_renderer3D.submitToFB(&transform, m_chunksStorage->chunks[i].getMesh(), m_viewId);
    }
    m_renderer3D.end();
}
void ChunksRenderer::onDetach() {
    for (int i = 0; i < ChunksStorage::SIZE_XYZ; ++i) {
        m_chunksStorage->chunks[i].getMesh()->deleteTextures();
    }
    F_DELETE(Foundation::getAllocator(), m_blocksCreation);
    F_DELETE(Foundation::getAllocator(), m_chunksStorage);
}
