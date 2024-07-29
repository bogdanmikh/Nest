#pragma once

class VertexBuffer {
private:
    unsigned int m_RendererID;

public:
    VertexBuffer() = default;
    VertexBuffer(const void *data, unsigned int size);
    void create(const void *data, unsigned int size);
    void destroy();

    void bind() const;
    void unbind() const;
};
