#include "Nest/Renderer/Renderer.hpp"
#include "Nest/Logger/Logger.hpp"
#include "Nest/Renderer/ErrorsOpenGL.hpp"

void Renderer::checkForErrors() {
    while (true) {
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;
        LOG_ERROR("OPENGL: {} ", getGLErrorStr(err));
    }
}

void Renderer::init() {
    glEnable(GL_DEPTH_TEST);
    //    glEnable(GL_CULL_FACE);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_BLEND);
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