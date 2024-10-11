#include "Nest/Renderer/Model3D.hpp"
#include "Nest/Application/Application.hpp"

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
    m_slots.resize(m_meshes.size());
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
    LOG_INFO("Size: {}", indices.size());
    for (int i = 0; i < indices.size(); ++i) {
        auto index = indices[i];
        auto pos = vertices[index].Position;
        LOG_INFO("X: {}, Y: {}, Z: {}", pos.x, pos.y, pos.z);
    }
//    LOG_INFO("Size: {}", vertices.size());
//    for (int i = 0; i < vertices.size(); ++i) {
//        auto vertex = vertices[i];
//        auto pos = vertex.Position;
//        LOG_INFO("X: {}, Y: {}, Z: {}", pos.x, pos.y, pos.z);
//    }

    Foundation::Memory verticesMemory = Foundation::Memory::copying(vertices.data(), sizeof(ModelVertex) * vertices.size());
    Foundation::Memory indicesMemory = Foundation::Memory::copying(indices.data(), sizeof(uint32_t) * indices.size());

    Bird::VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec2();
    layoutData.pushVec3();

    Bird::VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);

    Nest::MeshData meshData(
        vertexLayout, verticesMemory, vertices.size() * sizeof(ModelVertex), indicesMemory, indices.size()
    );
    Nest::StaticMesh *staticMesh = NEW(Foundation::getAllocator(), Nest::StaticMesh);
    staticMesh->create(meshData, textures, m_shader);
    return staticMesh;
}

void Model3D::draw() {
    static auto camera = Nest::Application::get()->getWorldCamera();
    m_viewProj = camera->getProjectionMatrix() * camera->getViewMatrix();
    for (int i = 0; i < m_meshes.size(); ++i) {
        auto *mesh = m_meshes[i];
        NEST_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
        //    Bird::setShader(mesh->m_shaderHandle);
        mesh->m_model = m_transformComponent.getTransform();
        Bird::setUniform(
            mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Bird::UniformType::Mat4
        );
        Bird::setUniform(
            mesh->m_shaderHandle, "projViewMtx", &m_viewProj, Bird::UniformType::Mat4
        );
//        if (m_slots[i].size() != mesh->m_textureBinding.size()) {
//            m_slots[i].resize(mesh->m_textureBinding.size());
//            for (int j = 0; j < mesh->m_textureBinding.size(); j++) {
//                m_slots[i][j] = j;
//            }
//        }

//        for (int j = 0; j < mesh->m_textureBinding.size(); j++) {
//            Bird::setTexture(mesh->m_textureBinding[j].texture, j);
//            Bird::setUniform(
//                mesh->m_shaderHandle,
//                mesh->m_textureBinding[j].name.c_str(),
//                &m_slots[i][j],
//                Bird::UniformType::Sampler
//            );
//        }

        NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
        Bird::setVertexBuffer(mesh->m_vertexBufferHandle);
        NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
        Bird::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
        Bird::submit(0);
    }
}

} // namespace Nest