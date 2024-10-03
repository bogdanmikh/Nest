#include "Nest/Renderer/Model3D.hpp"

namespace Nest {

Model3D::~Model3D() {
    for (uint32_t i = 0; i < m_meshes.size(); i++) {
        DELETE(Foundation::getAllocator(), m_meshes[i]);
    }
    m_meshes.clear();
}

TransformComponent &Model3D::getTransform() {
    return m_transformComponent;
}

void Model3D::create(Bird::ProgramHandle shader, Path pathToModel) {
    m_shader = shader;
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(pathToModel, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_CRITICAL("ERROR::ASSIMP::{}", importer.GetErrorString());
    }

    processNode(scene->mRootNode, scene);
}

void Model3D::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

DynamicMesh *Model3D::processMesh(aiMesh *mesh, const aiScene *scene) {
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
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // обработка материала
//    if (mesh->mMaterialIndex >= 0) {
//        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
//        std::vector<TextureBinding> diffuseMaps =
//            loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//        std::vector<TextureBinding> specularMaps =
//            loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//    }
    Foundation::Memory verticesMemory = Foundation::Memory(vertices.data());
    Foundation::Memory indicesMemory = Foundation::Memory(indices.data());

    Bird::VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec3();
    layoutData.pushVec2();

    Bird::VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);

    Nest::MeshData meshData(
        vertexLayout, verticesMemory, vertices.size() * sizeof(ModelVertex), indicesMemory, indices.size()
    );
    Nest::DynamicMesh *dynamicMesh = NEW(Foundation::getAllocator(), Nest::DynamicMesh);
    dynamicMesh->create(meshData, textures, m_shader);
    return dynamicMesh;
}

void Model3D::draw() {
    for (int i = 0; i < m_meshes.size(); ++i) {
        auto *mesh = m_meshes[i];
        NEST_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
        //    Bird::setShader(mesh->m_shaderHandle);
        mesh->m_model = getTransform().getTransform();
        Bird::setUniform(
            mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Bird::UniformType::Mat4
        );
        Bird::setUniform(
            mesh->m_shaderHandle, "projViewMtx", (void *)&m_viewProj, Bird::UniformType::Mat4
        );
        for (int j = 0; j < mesh->m_bindings.size(); j++) {
            Bird::setTexture(mesh->m_bindings[j].texture, j);
            Bird::setUniform(
                mesh->m_shaderHandle,
                mesh->m_bindings[j].name.c_str(),
                &j,
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