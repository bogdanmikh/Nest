#include "Nest/Renderer/Model3D.hpp"
#include "Nest/Application/Application.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#if USE_ASSIMP

namespace Nest {

Model3D::~Model3D() {
    for (uint32_t i = 0; i < m_meshes.size(); i++) {
        m_meshes[i]->deleteTextures();
        F_DELETE(Foundation::getAllocator(), m_meshes[i]);
    }
    m_meshes.clear();
}

TransformComponent &Model3D::getTransform() {
    return m_transformComponent;
}

void Model3D::create(
    Bird::ProgramHandle shader, Path pathToModel, const CreateInfoModel3D &createInfoModel3D
) {
    m_pathToModel3D = pathToModel;
    m_shader = shader;
    m_createInfo = createInfoModel3D;
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(pathToModel, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_CRITICAL("ERROR::ASSIMP::{}", importer.GetErrorString());
    }
    processNode(scene->mRootNode, scene);
    m_slots.resize(m_meshes.size());
}

void Model3D::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.emplace_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

StaticMesh *Model3D::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<ModelVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureBinding> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        ModelVertex vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // орбаботка индексов
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // обработка материала
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<TextureBinding> diffuseMaps =
            loadMaterialTextures(material, aiTextureType_DIFFUSE, m_createInfo.nameDiffuse);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<TextureBinding> specularMaps =
            loadMaterialTextures(material, aiTextureType_SPECULAR, m_createInfo.nameSpecular);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    //    LOG_INFO("Size: {}", vertices.size());
    //    for (int i = 0; i < vertices.size(); ++i) {
    //        auto vertex = vertices[i];
    //        auto pos = vertex.Position;
    //        LOG_INFO("X: {}, Y: {}, Z: {}", pos.x, pos.y, pos.z);
    //    }

    Foundation::Memory verticesMemory =
        Foundation::Memory::copying(vertices.data(), sizeof(ModelVertex) * vertices.size());
    Foundation::Memory indicesMemory =
        Foundation::Memory::copying(indices.data(), sizeof(uint32_t) * indices.size());

    Bird::VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec2();
    layoutData.pushVec3();

    Bird::VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);

    Nest::MeshData meshData(
        vertexLayout,
        verticesMemory,
        vertices.size() * sizeof(ModelVertex),
        indicesMemory,
        indices.size()
    );
    Nest::StaticMesh *staticMesh = F_NEW(Foundation::getAllocator(), Nest::StaticMesh);
    staticMesh->create(meshData, textures, m_shader);
    return staticMesh;
}

std::vector<TextureBinding>
Model3D::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<TextureBinding> textures;
    uint32_t countTextures = mat->GetTextureCount(type);
    textures.reserve(countTextures);

    Path directoryModel;
    Path lastPath;
    for (const auto &comp : m_pathToModel3D) {
        directoryModel /= comp;
        if (lastPath == "Models") {
            break;
        }
        lastPath = comp;
    }
    for (unsigned int i = 0; i < countTextures; i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::filesystem::path texturePath =
            std::filesystem::current_path() / directoryModel / std::string(str.C_Str());
        if (!std::filesystem::exists(texturePath) ||
            std::filesystem::exists(texturePath) == std::filesystem::is_directory(texturePath)) {
            LOG_ERROR("Path not exists: {}", texturePath.string());
            continue;
        }
        //  LOG_INFO("Path: {}", texturePath.string());

        Nest::TextureAsset textureAsset = Nest::AssetLoader::loadTexture(texturePath);
        Bird::TextureCreate textureCreate = textureAsset.getBirdTextureCreate();
        textureCreate.m_numMips = 4;
        textureCreate.m_minFiltering = Bird::NEAREST_MIPMAP_LINEAR;
        textureCreate.m_magFiltering = Bird::LINEAR;
        TextureBinding texture;
        texture.texture = createTexture(textureCreate);
        texture.name = typeName;
        textures.emplace_back(texture);
    }
    return textures;
}

void Model3D::draw() {
    static auto camera = Nest::Application::get()->getWorldCamera();
    m_viewProj = camera->getProjectionMatrix() * camera->getViewMatrix();
    for (int i = 0; i < m_meshes.size(); ++i) {
        auto *mesh = m_meshes[i];
        NEST_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
        Bird::setShader(mesh->m_shaderHandle);
        mesh->m_model = m_transformComponent.getTransform();
        Bird::setUniform(
            mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Bird::UniformType::Mat4
        );
        Bird::setUniform(mesh->m_shaderHandle, "projViewMtx", &m_viewProj, Bird::UniformType::Mat4);
        if (m_slots[i].size() != mesh->m_textureBinding.size() &&
            mesh->m_textureBinding.size() > 0) {
            // int a = mesh->m_textureBinding.size(), b = m_slots[i].size();
            m_slots[i].resize(mesh->m_textureBinding.size());
            for (int j = 0; j < mesh->m_textureBinding.size(); j++) {
                m_slots[i][j] = j;
            }
        }

        for (int j = 0; j < mesh->m_textureBinding.size(); j++) {
            Bird::setTexture(mesh->m_textureBinding[j].texture, j);
            Bird::setUniform(
                mesh->m_shaderHandle,
                mesh->m_textureBinding[j].name.c_str(),
                &m_slots[i][j],
                Bird::UniformType::Sampler
            );
        }

        NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
        Bird::setVertexBuffer(mesh->m_vertexBufferHandle);
        NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
        Bird::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
        Bird::submit(0);
    }
}

} // namespace Nest

#endif