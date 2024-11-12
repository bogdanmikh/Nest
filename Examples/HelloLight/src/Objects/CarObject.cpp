//
// Created by Bogdan on 06.08.2024.
//

#include "CarObject.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <fstream>

CarObject::CarObject() {}

void CarObject::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
    m_shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(
        m_shaderHandle, "Models/car/scene.gltf", {"material.diffuse", "material.specular"}
    );
}

void CarObject::updateLight() {
    auto camera = Nest::Application::get()->getWorldCamera();

    /// Позиция источника света в пространстве. Это вектор, задающий координаты источника света (x,
    /// y, z).
    m_light.position = {camera->getPosition(), 0};

    /// Направление, в котором источник света излучает свет. Для направленных источников света
    /// (например, прожекторов) это вектор, показывающий, куда направлен свет.
    m_light.direction = {camera->getFront(), 0};

    /// Угол среза (cut-off angle) для светящихся конусов, который определяет, на каком углу от
    /// направления света начинается уменьшение яркости. Обычно измеряется в градусах.
    m_light.cutOff.r = glm::cos(glm::radians(12.5f));

    /// Внешний угол (outer cut-off angle), который определяет конец области, где свет постепенно
    /// затухает. Это значение также измеряется в градусах и используется для создания градиентного
    /// затухания света по краям конуса.
    m_light.outerCutOff.r = glm::cos(glm::radians(17.5f));

    /// Значение окружающего света (ambient light) — это цвет света, который равномерно освещает
    /// объекты, даже если они не находятся в прямой видимости источника света.
    //    m_light.ambient = {1.0, 1.0, 1.0, 0.0f};
    m_light.ambient = {0.1f, 0.1f, 0.1f, 0.0f};

    /// Значение диффузного света (diffuse light) — это цвет света, который рассеивается по
    /// поверхности объекта, создавая видимые тени и цвета. Это основной источник видимого света,
    /// взаимодействующего с объектами.

    m_light.diffuse = {0.8f, 0.8f, 0.8f, 0.0f};

    /// Значение спекулярного света (specular light) — это цвет света, который создает яркие блики
    /// на поверхности объектов. Он зависит от угла между направлением взгляда и отражённым светом.
    m_light.specular = {1.0f, 1.0f, 1.0f, 0.0f};

    /// Постоянный коэффициент затухания света (constant attenuation factor) —
    /// этот параметр определяет, как сильно свет ослабляется на расстоянии от источника.
    /// Постоянный компонент затухания не зависит от расстояния.
    m_light.constant.r = 1.0f;

    /// Линейный коэффициент затухания света (linear attenuation factor) — этот параметр влияет на
    /// затухание света в зависимости от расстояния. Он определяет, как сильно свет затухает с
    /// увеличением расстояния.
    m_light.linear.r = 0.09f;

    /// Квадратичный коэффициент затухания света (quadratic attenuation factor) — этот параметр
    /// также влияет на затухание света, но он учитывает квадрат расстояния до источника. Это
    /// приводит к более быстрому уменьшению яркости по мере удаления от источника.
    m_light.quadratic.r = 0.032f;

    m_shininess.r = 32.0f;
}

void CarObject::onUpdate(double deltaTime) {
    updateLight();
    auto camera = Nest::Application::get()->getWorldCamera();
    auto &transform = m_model.getTransform();
    transform.setPosition({0, 0, 0});
    transform.setScale({0.1, 0.1, 0.1});

    Bird::setUniform(
        m_shaderHandle, "vec4CameraPos", &camera->getPosition()[0], Bird::UniformType::Vec4
    );

    Bird::setUniform(m_shaderHandle, "material.shininess", &m_shininess, Bird::UniformType::Vec4);

    Bird::setUniform(m_shaderHandle, "lightPosition", &m_light.position, Bird::UniformType::Vec4);
    Bird::setUniform(m_shaderHandle, "lightDirection", &m_light.direction, Bird::UniformType::Vec4);
    Bird::setUniform(m_shaderHandle, "lightCutOff", &m_light.cutOff, Bird::UniformType::Vec4);
    Bird::setUniform(
        m_shaderHandle, "lightOuterCutOf", &m_light.outerCutOff, Bird::UniformType::Vec4
    );
    Bird::setUniform(m_shaderHandle, "lightAmbient", &m_light.ambient, Bird::UniformType::Vec4);
    Bird::setUniform(m_shaderHandle, "lightDiffuse", &m_light.diffuse, Bird::UniformType::Vec4);
    Bird::setUniform(m_shaderHandle, "lightSpecular", &m_light.specular, Bird::UniformType::Vec4);
    Bird::setUniform(m_shaderHandle, "lightConstant", &m_light.constant, Bird::UniformType::Vec4);
    Bird::setUniform(m_shaderHandle, "lightLinear", &m_light.linear, Bird::UniformType::Vec4);
    m_model.draw();
}

void CarObject::onImGuiRender() {}

void CarObject::onDetach() {}
