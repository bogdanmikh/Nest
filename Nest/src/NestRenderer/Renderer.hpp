#pragma once

class Renderer {
public:
    static void init();
    static void clear();
    static void setClearColor(float r, float g, float b, float a);
    static void setRenderBufferSize(int w, int h);
    static void drawArrays(int count);
    static void drawIndexed(int count);
};