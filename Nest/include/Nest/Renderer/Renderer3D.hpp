#pragma once

#include "Nest/GameLogic/Components/DynamicMesh.hpp"
#include "Nest/GameLogic/Components/StaticMesh.hpp"
#include "Nest/GameLogic/Components/TransformComponent.hpp"

namespace Nest {

// TODO: - 1. Вычислять матрицу модели только один раз для статических мешей
// TODO: - 2. Batch rendering

class Renderer3D final {
public:
    struct Statistics {
        uint32_t drawCalls = 0;
    };

    struct DrawData {
        Renderer3D::Statistics stats;
    };

    Renderer3D();
    Renderer3D(Renderer3D &&other);
    ~Renderer3D() = default;
    void begin();
    void submit(TransformComponent *transform, StaticMesh *mesh);
    void submitToFB(TransformComponent *transform, StaticMesh *mesh, Bird::ViewId viewId);
    void submit(TransformComponent *transform, DynamicMesh *mesh);
    void end();
    Statistics getStats();
    void setViewProj(glm::mat4 viewProj);

    Renderer3D &operator=(Renderer3D &&other);

private:
    DrawData m_drawData;
    glm::mat4 m_viewProj;
};

} // namespace Nest
