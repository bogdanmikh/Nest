#pragma once

#include <Bird/Bird.hpp>
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
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        // bone indexes which will influence this vertex
        int m_BoneIDs[4];
        // weights from each bone
        float m_Weights[4];
    };

    void create(Bird::ProgramHandle shader, const char *path, const char *nameModel3D);
    TransformComponent &getTransform();

    void draw();
private:
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    DynamicMesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<TextureBinding>
    loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    void rendererMesh();

    struct TextureData {
        Bird::TextureHandle textureHandle;
        std::string path;
    };

    std::string m_directory;
    std::string m_nameModel3D;
    std::vector<TextureData> texturesLoaded;
    Bird::ProgramHandle m_shader;
    std::vector<DynamicMesh> m_meshes;
    TransformComponent m_transformComponent;

    glm::mat4 m_viewProj;
};

} // namespace Nest