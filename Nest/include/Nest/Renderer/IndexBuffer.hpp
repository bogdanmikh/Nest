#pragma once

class IndexBuffer {
private:
    unsigned int m_RendererID = 0;
    unsigned int m_Size = 0;

public:
    IndexBuffer() = default;
    IndexBuffer(unsigned int *data, unsigned int count);
    void create(unsigned int *data, unsigned int count);
    void destroy();

    void bind() const;
    void unbind() const;
    unsigned int getSize() const;
};
