#include "WaveEffect.hpp"

WaveEffect::WaveEffect() {}

void WaveEffect::onAttach() {
    using namespace Bird;
    Nest::Path vertexShaderPath, fragmentShaderPath;

    //    vertexShaderPath = "Shaders/vst.glsl";
    //    fragmentShaderPath = "Shaders/fst.glsl";
    //    Nest::ProgramAsset programAsset =
    //        Nest::AssetLoader::loadProgram(vertexShaderPath, fragmentShaderPath);
#if USE_VULKAN
    vertexShaderPath = "Shaders/vst.spv";
    fragmentShaderPath = "Shaders/fst.spv";
#else
    vertexShaderPath = "Shaders/vst330.glsl";
    fragmentShaderPath = "Shaders/fst330.glsl";
#endif
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram(vertexShaderPath.string(), fragmentShaderPath.string());
    m_shader = createProgram(programAsset.getBirdProgramCreate());

    // clang-format off
    float vertices[8] = {
        // x   y
        -1.0, -1.0,
        1.0, -1.0,
        -1.0, 1.0,
        1.0, 1.0,
    };
    // clang-format on

    Foundation::Memory verticesMemory = Foundation::Memory::copying(vertices, sizeof(float) * 8);

    uint32_t indices[6] = {0, 1, 2, 1, 3, 2};
    Foundation::Memory indicesMemory = Foundation::Memory::copying(indices, sizeof(uint32_t) * 6);

    VertexBufferLayoutData layoutData;
    layoutData.pushVec2();
    VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    m_vertexBuffer = createVertexBuffer(verticesMemory, 8 * sizeof(float), vertexLayout);
    m_indexBuffer = createIndexBuffer(indicesMemory, BufferElementType::UnsignedInt, 6);

    Bird::setViewClear(0, 0x000000FF);

    colors[0] = {0.2f, 0.8f, 0.3f};  // Ярко-зеленый (основной круг 1)
    colors[1] = {0.5f, 0.2f, 0.8f};  // Фиолетовый (основной круг 2)
    colors[2] = {0.1f, 0.6f, 0.5f};  // Морской волны (основной круг 3)

    colors[3] = {0.4f, 0.9f, 0.6f};  // Светло-зеленый (градиент)
    colors[4] = {0.7f, 0.3f, 0.9f};  // Сиреневый (градиент)
    colors[5] = {0.2f, 0.8f, 0.8f};  // Бирюзовый (градиент)

    // Параметры вращения (x,y смещение, z - скорость)
    rotation[0] = {0.1f, -0.05f, 0.15f};  // Медленное вращение
    rotation[1] = {-0.2f, 0.1f, -0.1f};   // Обратное вращение
    rotation[2] = {0.05f, 0.15f, 0.2f};   // Средняя скорость

    direction[0] = Direction::LEFT;
    direction[1] = Direction::RIGHT;
    direction[2] = Direction::LEFT;
}

void WaveEffect::onUpdate(double deltaTime) {
    time = Nest::Application::get()->getWindow()->getTime();
    mousePos = {Nest::Input::getMousePositionX(), Nest::Input::getMousePositionX()};
    resolution = Nest::Application::get()->getWindow()->getSize();
    // Плавные аудио-параметры (используйте шум Перлина или синусоиды)
    static float audioTime = 0.0f;
    audioTime += 0.1f;
    audio[0] = 0.5f + 0.4f * sin(audioTime * 1.0f);       // Низкие частоты
    audio[1] = 0.4f + 0.3f * sin(audioTime * 1.7f + 1.0f); // Средние частоты
    audio[2] = 0.3f + 0.2f * sin(audioTime * 2.3f + 2.0f); // Высокие частоты

    background = {0, 0, 0, 1};

    for (int i = 0; i < 3; ++i) {
        float coeff = Nest::getRandomFloat(0, 0.5) * deltaTime;
        if (direction[i] == Direction::RIGHT) {
            rotation[i].x += Nest::getRandomFloat(0, maxSpeed) * coeff;
            rotation[i].y += Nest::getRandomFloat(0, maxSpeed) * coeff;
            rotation[i].z += Nest::getRandomFloat(0, maxSpeed) * coeff;
        } else {
            rotation[i].x -= Nest::getRandomFloat(0, maxSpeed) * coeff;
            rotation[i].y -= Nest::getRandomFloat(0, maxSpeed) * coeff;
            rotation[i].z -= Nest::getRandomFloat(0, maxSpeed) * coeff;
        }
        if (rotation[i].x > 1 || rotation[i].y > 1 || rotation[i].z > 1) {
            direction[i] = Direction::LEFT;
        } else if (rotation[i].x < 0 || rotation[i].y < 0 || rotation[i].z < 0) {
            direction[i] = Direction::RIGHT;
        }
        float d = Nest::getRandomFloat(0, 1);
        if (d > 0.5 && d < 0.6) {
            direction[i] = static_cast<Direction>(((int)direction[i] + 1) % Count);
        }
    }

    // Параметры реакции - плавно затухают
    react[0] = std::max(0.0f, react[0] - 0.01f);
    react[1] = std::max(0.0f, react[1] - 0.008f);
    react[2] = std::max(0.0f, react[2] - 0.012f);
    if (Nest::Input::isMouseButtonPressed(Nest::MouseButton::LEFT)) {
        interaction = 1;
    } else {
        interaction = 0;
    }
    Bird::setShader(m_shader);

    Bird::setUniform(m_shader, "vScreenSize", &resolution, Bird::UniformType::Vec2);
    Bird::setUniform(m_shader, "vTime", &resolution, Bird::UniformType::Float);
    static float scale = 0.5;
    Bird::setUniform(m_shader, "vScale", &scale, Bird::UniformType::Float);
    Bird::setUniform(m_shader, "vColorBackground", &background, Bird::UniformType::Vec4);
    for (int i = 0; i < 6; ++i) {
        std::stringstream nameUniform;
        nameUniform << "vColor[" << i << "]";
        Bird::setUniform(m_shader, nameUniform.str().c_str(), &colors[i], Bird::UniformType::Vec3);
    }

    for (int i = 0; i < 3; ++i) {
        std::stringstream nameUniform;
        nameUniform << "vRotation[" << i << "]";
        Bird::setUniform(m_shader, nameUniform.str().c_str(), &rotation[i], Bird::UniformType::Vec3);
    }

    for (int i = 0; i < 3; ++i) {
        std::stringstream nameUniform;
        nameUniform << "vAudio[" << i << "]";
        Bird::setUniform(m_shader, nameUniform.str().c_str(), &audio[i], Bird::UniformType::Float);
    }

    for (int i = 0; i < 3; ++i) {
        std::stringstream nameUniform;
        nameUniform << "vReact[" << i << "]";
        Bird::setUniform(m_shader, nameUniform.str().c_str(), &react[i], Bird::UniformType::Float);
    }

    Bird::setUniform(m_shader, "vInteractionPoint", &mousePos, Bird::UniformType::Vec2);
    Bird::setUniform(m_shader, "vInteraction", &interaction, Bird::UniformType::Float);

    Bird::setIndexBuffer(m_indexBuffer, 0, 6);
    Bird::setVertexBuffer(m_vertexBuffer);
    Bird::submit(0);
}

void WaveEffect::onImGuiRender() {}

void WaveEffect::onDetach() {
    deleteProgram(m_shader);
}

Nest::TransformComponent &WaveEffect::getTransform() {
    return m_transformComponent;
}