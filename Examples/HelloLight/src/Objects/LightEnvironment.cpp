//
// Created by Bogdan on 24.03.2025.
//

#include "LightEnvironment.hpp"
#include "LightObject.hpp"

static glm::mat4 calculateLightSpaceMatrix(
    const glm::vec3 &lightPos,  // Позиция источника света
    const glm::vec3 &lightDir,  // Направление источника света
    float nearPlane = 0.1f,     // Ближняя плоскость усечения
    float farPlane = 100.0f     // Дальняя плоскость усечения
) {
    // 1. Матрица вида (view) из позиции света
    glm::mat4 lightView = glm::lookAt(
        lightPos,            // Позиция источника света
        lightPos + lightDir, // Направление света (куда он смотрит)
        glm::vec3(0.0f, 1.0f, 0.0f) // Вектор "вверх" (обычно Y)
    );

    // 2. Матрица проекции (перспективная или ортографическая)
    // Для SpotLight лучше использовать перспективную проекцию
    float fov = glm::radians(90.0f); // Угол обзора (можно регулировать)
    float aspectRatio = 1.0f; // Соотношение сторон (обычно 1:1 для теней)

    glm::mat4 lightProjection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

    // 3. Итоговая lightSpaceMatrix
    return lightProjection * lightView;
}

static glm::mat4 calculateOrthoLightSpaceMatrix(
    const glm::vec3 &lightPos,
    const glm::vec3 &lightDir,
    float orthoSize = 20.0f, // Размер области видимости
    float nearPlane = 0.1f,
    float farPlane = 100.0f
) {
    // 1. Матрица вида (view)
    glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
    lightView = glm::lookAt(lightPos, glm::vec3(0.), glm::vec3(0.0f, 1.0f, 0.0f));

    // 2. Ортографическая проекция
    glm::mat4 lightProjection = glm::ortho(
        -orthoSize,
        orthoSize, // left, right
        -orthoSize,
        orthoSize, // bottom, top
        nearPlane,
        farPlane // near, far
    );

    // 3. Итоговая lightSpaceMatrix
    return lightProjection * lightView;
}

void LightEnvironment::initObjects() {
    InfoObject info;
    // old lada
    info.position = {0, 0, 0};
    info.degrees = {0, 0, 0};
    info.scale = {0.01, 0.01, 0.01};
    info.pathToModel = "Models/old_lada/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    m_managerObjects.add(info);

    // asphalt
    info.position = {0, -0.2, 7};
    info.degrees = {-90, 90, 0};
    info.scale = {25, 25, 25};
    info.pathToModel = "Models/asphalt/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    m_managerObjects.add(info);

    // street lamp
    info.position = {-8, -1, 2};
    info.degrees = {-90, 180, 0};
    info.scale = {0.04, 0.04, 0.04};
    info.pathToModel = "Models/street_lamp/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    //    m_managerObjects.add(info);

    // dps
    info.position = {15, 4.4, 0};
    info.degrees = {0, 0, 0};
    info.scale = {0.5, 0.5, 0.5};
    info.pathToModel = "Models/lada_2109_dps/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    //            m_managerObjects.add(info);

    info.position = {0, 0, 0};
    info.degrees = {0, 0, 0};
    info.scale = {0.1, 0.1, 0.1};
    info.pathToModel = "Models/Sponza/sponza.obj";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    //            m_managerObjects.add(info);
}

void LightEnvironment::initLights() {
    DirLight dirLight;
    dirLight.direction = {-0.2f, -1.0f, -0.3f, 0.f};
    dirLight.ambient = {0.0f, 0.0f, 0.0f, 0.f};
    dirLight.diffuse = {0.05f, 0.05f, 0.05, 0};
    dirLight.specular = {0.2f, 0.2f, 0.2, 0};
    m_managerLights.add(dirLight);

    SpotLight spotLight;
    spotLight.position = {-2.0f, 4.0f, -1.0f, 0};
    spotLight.position = {-3.4, 10, 2.8, 0};
    spotLight.direction = {0.18, -0.87, -0.45, 0};
    spotLight.ambient = {1., 1., 1., 0};
    spotLight.diffuse = {1.0f, 1.0f, 1.0, 0};
    spotLight.specular = {1.0f, 1.0f, 1.0, 0};
    spotLight.constant.r = 0.001f;
    spotLight.linear.r = 0.009f;
    spotLight.quadratic.r = 0.0032f;
    spotLight.cutOff.r = glm::cos(glm::radians(2.0f));
    spotLight.outerCutOff.r = glm::cos(glm::radians(40.5f));
    m_managerLights.add(spotLight);
}

void LightEnvironment::onAttach() {
    // init
    initObjects();
    initLights();

    // depth viewport
    m_depthViewport.init();
    m_debugDepthViewport.init();

    // Shaders
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstDepth.glsl", "Shaders/fstDepth.glsl");
    m_depthShader = createProgram(programAsset.getBirdProgramCreate());

    programAsset = Nest::AssetLoader::loadProgram(
        "Shaders/vstModel3DSpot.glsl", "Shaders/fstModel3DSpot.glsl"
    );
    m_lightShader = createProgram(programAsset.getBirdProgramCreate());

    // Post effect
    m_debugDepthEffect.setPathToShaders("Shaders/vstDebug.glsl", "Shaders/fstDebug.glsl");
    m_debugDepthEffect.onAttach();

    m_debugDepthEffect.setChannel0(m_depthViewport.getDepthHandle());
    m_debugDepthEffect.setViewId(m_debugDepthViewport.getViewId());
}

void LightEnvironment::onUpdate(double deltaTime) {
    generateDepthTexture();
    generateDebugDepthTexture();
    draw();
}

void LightEnvironment::onImGuiRender() {
        ImGui::Begin("Mode");
        ImGui::Image(
            (ImTextureID)(intptr_t)m_debugDepthViewport.getTextureHandle().id, {200, 200}
        );
        auto &pos = m_managerLights.getSpotLights().back().position;
        glm::vec3 newPos;
        newPos.x = pos.x;
        newPos.y = pos.y;
        newPos.z = pos.z;
        ImGui::DragFloat3("Pos: ", &newPos[0]);
        pos.x = newPos.x;
        pos.y = newPos.y;
        pos.z = newPos.z;
        ImGui::End();
}

void LightEnvironment::onDetach() {
    m_debugDepthEffect.onDetach();

    deleteProgram(m_depthShader);
    deleteProgram(m_lightShader);
}

void LightEnvironment::generateDepthTexture() {
    m_depthViewport.update();
    m_managerObjects.setViewId(m_depthViewport.getViewId());
    m_managerObjects.setShader(m_depthShader);
    setDepthUniforms();
    m_managerObjects.draw();
}

void LightEnvironment::generateDebugDepthTexture() {
    m_debugDepthViewport.update();
    m_debugDepthEffect.onUpdate(0);
}

void LightEnvironment::draw() {
    auto camera = Nest::Application::get()->getWorldCamera();

    auto &dirLights = m_managerLights.getDirLights();
    auto &pointLights = m_managerLights.getPointLights();
    auto &spotLights = m_managerLights.getSpotLights();

    glm::vec4 numLights;
    numLights.x = m_managerLights.getDirLights().size();
    numLights.y = m_managerLights.getPointLights().size();
    numLights.z = m_managerLights.getSpotLights().size();

    glm::vec4 shininess;
    shininess.x = 32.;

    Bird::setUniform(m_lightShader, "lightSpaceMatrix", &m_lightSpaceMatrix, Bird::UniformType::Mat4);

    Bird::setUniform(m_lightShader, "material.shininess", &shininess, Bird::UniformType::Vec4);
    Bird::setUniform(m_lightShader, "numLights", &numLights, Bird::UniformType::Vec4);
    Bird::setUniform(
        m_lightShader, "cameraPosVec4", &camera->getPosition()[0], Bird::UniformType::Vec4
    );
    Bird::setUniform(
        m_lightShader, "cameraFrontVec4", &camera->getFront()[0], Bird::UniformType::Vec4
    );

    int slot = 2;
    Bird::setTexture(m_debugDepthViewport.getTextureHandle(), slot);
    Bird::setUniform(m_lightShader, "shadowMap", &slot, Bird::UniformType::Sampler);

    for (int i = 0; i < spotLights.size(); ++i) {
        setSpotLightUniforms(m_lightShader, spotLights[i], i);
    }

    m_managerObjects.setShader(m_lightShader);
    m_managerObjects.setViewId(0);

    m_managerObjects.draw();
}

void LightEnvironment::setDepthUniforms() {
    const SpotLight &spotLight = m_managerLights.getSpotLights()[0];

    glm::vec3 lightPos = {spotLight.position.x, spotLight.position.y, spotLight.position.z};
    glm::vec3 lightDir = {spotLight.direction.x, spotLight.direction.y, spotLight.direction.z};

    glm::vec4 nearPlane;
    nearPlane.x = 1.;
    glm::vec4 farPlane;
    farPlane.x = 5.5;

    m_lightSpaceMatrix = calculateOrthoLightSpaceMatrix(lightPos, lightDir, 20, nearPlane.x, farPlane.x);
//    m_lightSpaceMatrix = calculateLightSpaceMatrix(lightPos, lightDir, nearPlane.x, farPlane.x);

    Bird::setShader(m_depthShader);
    Bird::setUniform(m_depthShader, "lightSpaceMatrix", &m_lightSpaceMatrix, Bird::UniformType::Mat4);
    Bird::setUniform(m_depthShader, "nearPlaneVec4", &nearPlane, Bird::UniformType::Vec4);
    Bird::setUniform(m_depthShader, "farPlaneVec4", &farPlane, Bird::UniformType::Vec4);
}

void LightEnvironment::setSpotLightUniforms(
    Bird::ProgramHandle programHandle, SpotLight &light, int index
) {
    std::stringstream beginStr;
    beginStr << "spotLightsVec4[" << index << "].";

    Bird::setUniform(
        programHandle,
        (beginStr.str() + "position").c_str(),
        &light.position,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "direction").c_str(),
        &light.direction,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "cutOff").c_str(), &light.cutOff, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "outerCutOff").c_str(),
        &light.outerCutOff,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "constant").c_str(),
        &light.constant,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "linear").c_str(), &light.linear, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "quadratic").c_str(),
        &light.quadratic,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "ambient").c_str(), &light.ambient, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "diffuse").c_str(), &light.diffuse, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "specular").c_str(),
        &light.specular,
        Bird::UniformType::Vec4
    );
}