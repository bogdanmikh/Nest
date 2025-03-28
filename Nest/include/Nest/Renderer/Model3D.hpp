#pragma once
#include "Nest/Base/Base.hpp"
#include "Nest/GameLogic/Components/StaticMesh.hpp"
#include "Nest/GameLogic/Components/TransformComponent.hpp"
#include "Nest/Renderer/Renderer3D.hpp"
#include <Bird/Bird.hpp>

#define USE_ASSIMP 1

#if USE_ASSIMP
#    include <assimp/Importer.hpp>
#    include <assimp/postprocess.h>
#    include <assimp/scene.h>

namespace Nest {

struct CreateInfoModel3D {
    std::string nameDiffuse = "texture_diffuse";
    std::string nameSpecular = "specular";
};

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

    void create(
        Bird::ProgramHandle shader,
        Path pathToModel,
        const CreateInfoModel3D &createInfoModel3D = {}
    );
    TransformComponent &getTransform();

    void draw();

    void setShader(Bird::ProgramHandle shader);
    void setViewId(Bird::ViewId viewId);

    int getCountTextures() {
        return m_meshes.front()->m_textureBinding.size();
    }

    ~Model3D();

private:
    void loadModel(const std::string &path);
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
    std::vector<StaticMesh *> m_meshes;

    CreateInfoModel3D m_createInfo;

    TransformComponent m_transformComponent;
    Bird::ViewId m_viewId;
    glm::mat4 m_viewProj;
};

} // namespace Nest
#else
namespace Nest {

struct CreateInfoModel3D {
    std::string nameDiffuse = "texture_diffuse";
    std::string nameSpecular = "specular";
};

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

    void create(
        Bird::ProgramHandle shader,
        Path pathToModel,
        const CreateInfoModel3D &createInfoModel3D = {}
    ) {}

    TransformComponent &getTransform() {
        return m_transformComponent;
    }

    void draw() {}

    ~Model3D() {}

private:
    TransformComponent m_transformComponent;
};
} // namespace Nest
#endif