#pragma once

#include "Bird/Base.hpp"
#include "Bird/Encoder/Uniform.hpp"
#include "Uniform.hpp"

#include <cstdint>

namespace Bird {

const static uint32_t s_uniformTypeSize[UniformType::Count + 1] = {
    sizeof(int32_t),       // Sampler
    4 * sizeof(float),     // Vec4
    3 * 3 * sizeof(float), // Mat3
    4 * 4 * sizeof(float), // Mat4
};

class UniformBuffer {
public:
    UniformBuffer(uint32_t size)
        : m_size(size)
        , m_pos(0) {
        m_data = (uint8_t *)F_ALLOC(Foundation::getAllocator(), size);
    }

    ~UniformBuffer() {
        F_FREE(Foundation::getAllocator(), m_data);
    }

    void writeUniform(Uniform &uniform) {
        align(__alignof(Uniform));
        Uniform *savedUniform = (Uniform *)&m_data[m_pos];
        write(&uniform, sizeof(Uniform));
        if (uniform.data) {
            savedUniform->size = s_uniformTypeSize[uniform.type] * uniform.count;
            savedUniform->alignment = alignof(std::max_align_t);
            align(savedUniform->alignment);
            void *data = savedUniform->data;
            savedUniform->data = &m_data[m_pos];
            write(data, savedUniform->size);
        }
    }

    Uniform *readUniform() {
        align(__alignof(Uniform));
        Uniform &uniform = *(Uniform *)read(sizeof(Uniform));
        if (m_pos == 0 || uniform.handle.id == BIRD_INVALID_HANDLE) {
            return nullptr;
        }
        align(uniform.alignment);
        skip(uniform.size);
        return &uniform;
    }

    // Finish writing. Start reading
    void finishWriting() {
        Uniform uniform(BIRD_INVALID_HANDLE, nullptr, nullptr, UniformType::Sampler, 0);
        writeUniform(uniform);
        m_pos = 0;
    }

    void reset() {
        m_pos = 0;
    }

private:
    void write(const void *_data, uint32_t _size) {
        memcpy(&m_data[m_pos], _data, _size);
        m_pos += _size;
        NEST_ASSERT(m_pos < m_size, "NEED MORE SPACE FOR BUFFER");
    }

    void *read(uint32_t _size) {
        uint32_t pos = m_pos;
        m_pos += _size;
        return &m_data[pos];
    }

    void skip(uint32_t size) {
        m_pos += size;
    }

    void align(uint32_t _alignment) {
        const uint32_t mask = _alignment - 1;
        const uint32_t pos = (m_pos + mask) & (~mask);
        m_pos = pos;
    }

    uint32_t m_size;
    uint32_t m_pos;
    uint8_t *m_data;
};

} // namespace Bird