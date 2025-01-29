//
// Created by Admin on 11.02.2022.
//

#include "OpenGLShader.hpp"
#include "Bird/Base.hpp"
#include "Bird/PlatformData.hpp"

#include "OpenGLBase.hpp"

#include <sstream>
#include <fstream>

namespace Bird {

OpenGLShader::OpenGLShader()
    : m_id(-1)
    , m_uniformLocationCache() {}

void OpenGLShader::create(ProgramCreate create) {
    NEST_ASSERT(m_id == -1, "PROGRAM ALREADY CREATED");
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    // LOG_INFO("\n\nVERTEX CODE: {}\n\n", vertexCode);
    // LOG_INFO("\n\nFRAGMENT CODE: {}\n\n", fragmentCode);
    const char *vertexSource = reinterpret_cast<const char *>(create.m_vertex.data);
    GL_CALL(glShaderSource(vertex, 1, &vertexSource, nullptr));
    GL_CALL(glCompileShader(vertex));
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentSource = reinterpret_cast<const char *>(create.m_fragment.data);
    GL_CALL(glShaderSource(fragment, 1, &fragmentSource, nullptr));
    GL_CALL(glCompileShader(fragment));
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    m_id = glCreateProgram();
    GL_CALL(glAttachShader(m_id, vertex));
    GL_CALL(glAttachShader(m_id, fragment));
    GL_CALL(glLinkProgram(m_id));
    checkCompileErrors(m_id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    GL_CALL(glDeleteShader(vertex));
    GL_CALL(glDeleteShader(fragment));

    create.m_vertex.release();
    create.m_fragment.release();
}

void OpenGLShader::terminate() {
    NEST_ASSERT(m_id != -1, "PROGRAM ALREADY DELETED");
    GL_CALL(glDeleteProgram(m_id));
    m_id = -1;
}

void OpenGLShader::checkCompileErrors(unsigned int shader, const std::string &type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            NEST_ASSERT_F(false, "SHADER_COMPILATION_ERROR of type: {}\n{}", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            NEST_ASSERT_F(false, "PROGRAM_LINKING_ERROR of type: {}\n{}", type, infoLog);
        }
    }
}
int OpenGLShader::getUniformLocation(const std::string &name) {
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_id, name.c_str());
    //    NEST_ASSERT_F(location != -1, "SHADER UNIFORM {} not found", name);
    m_uniformLocationCache[name] = location;
    return location;
}

void OpenGLShader::bindAttributes(VertexBufferLayoutData &layout, intptr_t baseVertex) {
    intptr_t pointer = baseVertex;
    for (int i = 0; i < layout.m_elementsCount; i++) {
        GL_CALL(glEnableVertexAttribArray(i));
        // glVertexAttribDivisor(i, 0);
        int type;
        switch (layout.m_elements[i].type) {
            case BufferElementType::Float:
                type = GL_FLOAT;
                break;
            case BufferElementType::UnsignedInt:
                type = GL_UNSIGNED_INT;
                break;
            case BufferElementType::Int:
                type = GL_INT;
                break;
            case BufferElementType::UnsignedByte:
                type = GL_UNSIGNED_BYTE;
                break;
            default:
                NEST_ASSERT(false, "Buffer element type is undefined");
                break;
        }
        if (type == GL_UNSIGNED_INT || type == GL_INT) {
            GL_CALL(glVertexAttribIPointer(
                i,
                layout.m_elements[i].count,
                type,
                layout.m_stride,
                reinterpret_cast<const void *>(pointer)
            ));
        } else {
            GL_CALL(glVertexAttribPointer(
                i,
                layout.m_elements[i].count,
                type,
                layout.m_elements[i].normalized ? GL_TRUE : GL_FALSE,
                layout.m_stride,
                reinterpret_cast<const void *>(pointer)
            ));
        }
        pointer += layout.m_elements[i].count *
                   VertexBufferElement::getSizeOfType(layout.m_elements[i].type);
    }
}

void OpenGLShader::bind() {
    GL_CALL(glUseProgram(m_id));
}

void OpenGLShader::unbind() {
    GL_CALL(glUseProgram(0));
}

void OpenGLShader::setUniformFloat(const char *name, float *value, int count) {
    GL_CALL(glUniform1fv(getUniformLocation(name), count, value));
}

void OpenGLShader::setUniformVec4(const char *name, float *value, int count) {
    GL_CALL(glUniform4fv(getUniformLocation(name), count, value));
}

void OpenGLShader::setUniformMat3(const char *name, float *value, int count) {
    GL_CALL(glUniformMatrix3fv(getUniformLocation(name), count, GL_FALSE, value));
}

void OpenGLShader::setUniformMat4(const char *name, float *value, int count) {
    GL_CALL(glUniformMatrix4fv(getUniformLocation(name), count, GL_FALSE, value));
}

void OpenGLShader::setUniformInt(const char *name, int *value, int count) {
    GL_CALL(glUniform1iv(getUniformLocation(name), count, value));
}

} // namespace Bird
