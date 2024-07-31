#include "Nest/Renderer/OpenGL/Shader.hpp"
#include "Nest/Logger/Logger.hpp"
#include "Nest/Renderer/OpenGL/ErrorsOpenGL.hpp"

#include <fstream>
#include <iostream>

#include <sstream>
#include <filesystem>

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {
    create(vertexPath, fragmentPath, geometryPath);
}

void Shader::create(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::filesystem::path currentPath = std::filesystem::current_path().parent_path();
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (geometryPath != nullptr) {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    } catch (std::ifstream::failure &e) { LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"); }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    GL_CALL(glShaderSource(vertex, 1, &vShaderCode, NULL));
    GL_CALL(glCompileShader(vertex));
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CALL(glShaderSource(fragment, 1, &fShaderCode, NULL));
    GL_CALL(glCompileShader(fragment));
    checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if (geometryPath != nullptr) {
        const char *gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        GL_CALL(glShaderSource(geometry, 1, &gShaderCode, NULL));
        GL_CALL(glCompileShader(geometry));
        checkCompileErrors(geometry, "GEOMETRY");
    }
    // shader Program
    m_rendererID = glCreateProgram();
    GL_CALL(glAttachShader(m_rendererID, vertex));
    GL_CALL(glAttachShader(m_rendererID, fragment));
    if (geometryPath != nullptr) {
        GL_CALL(glAttachShader(m_rendererID, geometry));
    }
    GL_CALL(glLinkProgram(m_rendererID));
    checkCompileErrors(m_rendererID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    GL_CALL(glDeleteShader(vertex));
    GL_CALL(glDeleteShader(fragment));
    if (geometryPath != nullptr) {
        GL_CALL(glDeleteShader(geometry));
    }
}

void Shader::destroy() {
    GL_CALL(glDeleteProgram(m_rendererID));
}

void Shader::use() {
    GL_CALL(glUseProgram(m_rendererID));
}

void Shader::setBool(const std::string &name, bool value) {
    GL_CALL(glUniform1i(getUniformLocation(name), (int)value));
}

void Shader::setInt(const std::string &name, int value) {
    GL_CALL(glUniform1i(getUniformLocation(name), value));
}

void Shader::setFloat(const std::string &name, float value) {
    GL_CALL(glUniform1f(getUniformLocation(name), value));
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) {
    GL_CALL(glUniform2fv(getUniformLocation(name), 1, &value[0]));
}

void Shader::setVec2(const std::string &name, float x, float y) {
    GL_CALL(glUniform2f(getUniformLocation(name), x, y));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) {
    GL_CALL(glUniform3fv(getUniformLocation(name), 1, &value[0]));
}

void Shader::setVec3(const std::string &name, float x, float y, float z) {
    GL_CALL(glUniform3f(getUniformLocation(name), x, y, z));
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) {
    GL_CALL(glUniform4fv(getUniformLocation(name), 1, &value[0]));
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) {
    GL_CALL(glUniform4f(getUniformLocation(name), x, y, z, w));
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) {
    GL_CALL(glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) {
    GL_CALL(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) {
    GL_CALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

int Shader::getUniformLocation(const std::string &name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1) {
        LOG_WARN("WARNING:SHADER:UNIFORM {} not found", name);
    }
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::checkCompileErrors(uint32_t shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success) {
            GL_CALL(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
            LOG_ERROR(
                "ERROR::SHADER_COMPILATION_ERROR of type: {}\n{}\n -- "
                "--------------------------------------------------- -- ",
                type,
                infoLog
            );
        }
    } else {
        GL_CALL(glGetProgramiv(shader, GL_LINK_STATUS, &success));
        if (!success) {
            GL_CALL(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
            LOG_ERROR(
                "ERROR::PROGRAM_LINKING_ERROR of type: {}\n{}\n -- "
                "--------------------------------------------------- -- ",
                type,
                infoLog
            );
        }
    }
}
