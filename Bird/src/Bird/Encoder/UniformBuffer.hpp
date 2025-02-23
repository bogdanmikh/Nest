#pragma once

#include "Bird/Base.hpp"
#include "Bird/Encoder/Uniform.hpp"
#include "Uniform.hpp"

#include <cstdint>
#include <queue>

namespace Bird {

class UniformBuffer {
public:
    bool isEmpty() {
        return m_buffer.empty();
    }
    void writeUniform(Uniform &uniform) {
        m_buffer.emplace_back(uniform);
    }

    Uniform readUniform() {
        Uniform uniform = m_buffer.back();
        m_buffer.pop_back();
        return uniform;
    }

    void reset() {
        m_buffer.clear();
    }
private:
    std::deque<Uniform> m_buffer;
};

} // namespace Bird