#pragma once

#include <Bird/Bird.hpp>
#include "Nest/Base/Base.hpp"
#include "Nest/GameLogic/Components/StaticMesh.hpp"
#include "Nest/GameLogic/Components/TransformComponent.hpp"
#include "Nest/Renderer/Renderer3D.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Nest {

class Model3D {
public:
    struct ModelVertex {
        // position
        glm::vec3 Position;
        // texCoords
        glm::vec2 TexCoords;
        // normal
        glm::vec3 Normal;
    };

    void create(Bird::ProgramHandle shader, Path pathToModel);
    TransformComponent &getTransform();

    void draw();

    ~Model3D();

private:

    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    StaticMesh *processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<TextureBinding>
    loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    void rendererMesh();

    struct TextureData {
        Bird::TextureHandle textureHandle;
        std::string path;
    };

    Path m_pathToModel3D;
    Bird::ProgramHandle m_shader;
    std::vector<StaticMesh*> m_meshes;

    std::vector<std::vector<int>> m_slots;

    TransformComponent m_transformComponent;
    glm::mat4 m_viewProj;
};

} // namespace Nest