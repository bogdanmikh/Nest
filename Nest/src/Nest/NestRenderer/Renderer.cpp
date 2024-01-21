#include "Nest/NestRenderer/Renderer.hpp"
#include "glad/glad.h"

#include <iostream>

const char *getGLErrorStr(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:
            return "No error";
        case GL_INVALID_ENUM:
            return "Invalid enum";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
            // case GL_STACK_OVERFLOW:    return "Stack overflow";
            // case GL_STACK_UNDERFLOW:   return "Stack underflow";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        default:
            return "Unknown error";
    }
}

void Renderer::checkForErrors() {
    while (true) {
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;
        std::cout << "OPENGL: " << getGLErrorStr(err) << std::endl;;
    }
}

void Renderer::init() {
    glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
//    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::setRenderBufferSize(int w, int h) {
    glViewport(0, 0, w, h);
}

void Renderer::drawArrays(int count) {
    glDrawArrays(GL_TRIANGLES, 0, count);
    checkForErrors();
}

void Renderer::drawIndexed(int count) {
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    checkForErrors();
}